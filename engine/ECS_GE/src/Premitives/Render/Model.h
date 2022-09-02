#pragma once 
#include "Mesh.h"
#include "assimp/scene.h"

namespace RenderEngine
{
    class Texture2D;

    std::shared_ptr<Texture2D> TextureFromFile(const std::string& path, const std::string& type, unsigned int slot = 0);
    class Model
    {
    public:
        friend class RenderSystem;
        /*  Методы   */
        Model(const std::string& path)
        {
            loadModel(path);
        }
    private:
        /*  Данные модели  */
    // model data 
        std::vector<std::shared_ptr<Texture2D>> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Mesh>                       meshes;
        std::string directory;
        bool gammaCorrection;
        /*  Методы   */
        void loadModel(const std::string& path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, unsigned int slot = 0);
        std::string checkTextureFile(const std::string& path, aiTextureType type);
    };
}