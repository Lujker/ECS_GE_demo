#pragma once
#include "Vector.h"

namespace RenderEngine
{
    struct Material
    {
        Vector3     ambient = {1.f, 1.f, 1.f};
        Vector3     diffuse = { 1.f, 1.f, 1.f };
        Vector3     specular = { 1.f, 1.f, 1.f };
        float       shininess = 16.f;
    };
}