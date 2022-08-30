#pragma once
#include "Vector.h"
#include <memory>

namespace RenderEngine
{
    class Texture2D;

    /*{ {1.0f, 0.5f, 0.31f}, {1.0f, 0.5f, 0.31f}, {0.5f, 0.5f, 0.5f}, 32.0f }, 32; check it material (cuprum)*/
    struct Material
    {
        Vector3     ambient = {1.f, 1.f, 1.f};
        Vector3     diffuse = { 1.f, 1.f, 1.f };
        Vector3     specular = { 1.f, 1.f, 1.f };
        float       shininess = 16.f;
    };

    struct TextureMaterial
    {
        std::shared_ptr<Texture2D>		m_pTexture = nullptr;
        std::shared_ptr<Texture2D>		m_specularMap = nullptr;
        float							shininess = 16.f;
    };
}