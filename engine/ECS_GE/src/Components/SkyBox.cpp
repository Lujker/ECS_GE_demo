#include "SkyBox.h"
namespace RenderEngine
{
    SkyBox::SkyBox(const std::shared_ptr<Texture3D>& texture) : m_pTexture(texture)
    {
        init();
    }

    SkyBox::SkyBox(SkyBox&& skybox) noexcept:
        m_pTexture(std::move(skybox.m_pTexture)), 
        m_vertexArray(std::move(skybox.m_vertexArray)), 
        m_vertexCoordsBuffer(std::move(skybox.m_vertexCoordsBuffer))
    {}

    SkyBox& SkyBox::operator=(SkyBox&& skybox) noexcept
    {
        m_pTexture = std::move(skybox.m_pTexture);
        m_vertexArray = std::move(skybox.m_vertexArray);
        m_vertexCoordsBuffer = std::move(skybox.m_vertexCoordsBuffer);
        return *this;
    }

    std::shared_ptr<Texture3D> SkyBox::setTexture(const std::shared_ptr<Texture3D>& texture)
    {
        auto old_texture = m_pTexture;
        m_pTexture = texture;
        return old_texture;
    }

    void SkyBox::init()
    {
        constexpr GLfloat skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
        m_vertexCoordsBuffer.init(&skyboxVertices, sizeof(skyboxVertices));
        VertexBufferLayout vertexCoordsLayout;
        vertexCoordsLayout.addElementLayoutFloat(3, false);
        m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);
    }
}