#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Engine.h"
#include "LogSystem.h"
#include "CameraManager.h"
#include "ResourceManager.h"
#include "texture2D.h"
#include "gl_Include.h"

namespace RenderEngine 
{
	void Model::loadModel(const std::string& path)
	{
        unsigned int post_processing_flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices |
            aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_ImproveCacheLocality;

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, post_processing_flags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG(std::string("ERROR::ASSIMP::") + import.GetErrorString());
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                if (mesh->mTangents)
                {
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;
                }
                // bitangent
                if (mesh->mBitangents)
                {
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vector;
                }
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        std::vector<std::shared_ptr<Texture2D>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", 0);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<std::shared_ptr<Texture2D>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", 1);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<std::shared_ptr<Texture2D>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", 2);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<std::shared_ptr<Texture2D>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", 3);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
	}

    std::vector<std::shared_ptr<Texture2D>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, unsigned int slot)
	{
        std::vector<std::shared_ptr<Texture2D>> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string file_name = str.C_Str();
            file_name = checkTextureFile(file_name.data(), type);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j]->getTexturePath().data(), file_name.data()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                auto texture = TextureFromFile(std::string(this->directory + "\\" + file_name.data()), typeName, slot);
                if (texture)
                {
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                }
            }
        }
        return textures;
	}

    std::string Model::checkTextureFile(const std::string& path, aiTextureType type)
    {
        std::ifstream iff(this->directory + "\\" + path);
        if (!iff.is_open())
        {
            switch (type)
            {
            case aiTextureType_DIFFUSE:
                iff.open(this->directory + "\\textures\\diffus.jpg" );
                if (iff.is_open())
                {
                    iff.close();
                    return "textures\\diffus.jpg";
                }
                break;
            case aiTextureType_SPECULAR:
                iff.open(this->directory + "\\textures\\metallic.jpg");
                if (iff.is_open())
                {
                    iff.close();
                    return "textures\\metallic.jpg";
                }
                break;
            case aiTextureType_HEIGHT:
                break;
            case aiTextureType_NORMALS:
                iff.open(this->directory + "\\textures\\normal.png");
                if (iff.is_open())
                {
                    iff.close();
                    return "textures\\normal.png";
                }
                break;
            case aiTextureType_SHININESS:
                iff.open(this->directory + "\\textures\\roughness.jpg");
                if (iff.is_open())
                {
                    iff.close();
                    return "textures\\roughness.jpg";
                }
                break;
            case aiTextureType_OPACITY:
                break;
            case aiTextureType_LIGHTMAP:
                break;
            default:
                break;
            }
        }
        return path;
    }

    std::shared_ptr<Texture2D> TextureFromFile(const std::string& path, const std::string& type, unsigned int slot)
    {
        std::shared_ptr<Texture2D> texture = RES->loadTexture(path, 4, type, GL_TEXTURE0 + slot);
        return texture->isInit() ? texture : nullptr;
    }
}