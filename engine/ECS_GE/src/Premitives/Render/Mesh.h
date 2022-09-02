#pragma once
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <string>
#include <vector>
#include <memory>
#include "ShaderProgram.h"

#define MAX_BONE_INFLUENCE 4
namespace RenderEngine
{
    class Texture2D;

    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    std::string GetMaterialTextureName(const std::string& texture_type);
    class Mesh {
    public:
        /*  Mesh Data  */
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;
        /*  Functions  */
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);
        void Draw(const std::shared_ptr<RenderEngine::ShaderProgram>& shader);
        /*  Render data  */
        unsigned int VAO, VBO, EBO;
    private:
        /*  Functions    */
        void setupMesh();
    };
}