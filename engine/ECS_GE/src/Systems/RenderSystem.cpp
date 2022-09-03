#include "RenderSystem.h"
#include "Engine.h"
#include "CameraManager.h"
#include "DisplayString.h"
#include "FontManager.h"
#include "LightManager.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include <algorithm>
#include "Sprite.h"
#include "Cube.h"
#include "Model.h"
#include "FrameBuffer.h"

RenderSystem& RenderSystem::Instanse()
{
	static RenderSystem render_system;
	return render_system;
}

void RenderSystem::Render(std::shared_ptr<RenderEngine::Sprite> sprite, const PositionComponent& position,
	const CollisionComponent& collision)
{
    if (!sprite)
        return;
    const auto shader = RES->getShader("image_shader");
    if (!shader || !sprite->isValid())
    {
        return;
    }
    auto size = sprite->getCurrentAnimSize();
    if (collision.isValid())
    {
        size = collision.getSize();
    }

    CAMERA->UseShader(shader);
    shader->setMatrix4("modelMatrix", getTransformModel(position.getPosition().mX, position.getPosition().mY, size.mX * collision.getScale(), size.mY * collision.getScale(), position.getRotation()));
    shader->setFloat("layer", position.getLayer());
    shader->setInt("tex", sprite->getTexture2D()->getSlot());
    shader->setFloat("alpha", GetLastTransformAlpha());
    sprite->getTexture2D()->bind();

    draw(sprite->getVertexArray(), sprite->getIndexCoordsBuffer(), *shader);
}

void RenderSystem::Render(std::shared_ptr<RenderEngine::Image2D> image, const PositionComponent& position, const CollisionComponent& collision)
{
    if(!image)
        return;
	const auto shader = RES->getShader("image_shader");
    if(!shader || !image->isValid())
    {
        return;
    }
    auto size = image->getTextureSize();
    if (collision.isValid())
    {
        size = collision.getSize();
    }

    CAMERA->UseShader(shader);
	shader->setMatrix4("modelMatrix", getTransformModel(position.getPosition().mX, position.getPosition().mY, size.mX * collision.getScale(), size.mY * collision.getScale(), position.getRotation()));
	shader->setFloat("layer", position.getLayer());
    shader->setInt("tex", image->getTexture2D()->getSlot());
    shader->setFloat("alpha", GetLastTransformAlpha());
    image->getTexture2D()->bind();

	draw(image->getVertexArray(), image->getIndexCoordsBuffer(), *shader);
}

void RenderSystem::Render(std::shared_ptr<RenderEngine::Cube> cube, const PositionComponent3& position, const CollisionComponent3& collision)
{
    if (!cube)
        return;
    const auto shader = RES->getShader("default_3D");
    if (!shader)
    {
        return;
    }
    auto cam_pos = CAMERA->getCameraPosition();
    auto& cub_material = cube->getMaterial();
    CAMERA->UseShader(shader);
    shader->setMatrix4("modelMatrix", getTransformModel(position, collision));
    shader->setVec3("camera_position", cam_pos.cameraPos);
    shader->setInt("points_lights_count", LIGHT->getPointsLightsCount());
    setMaterial(shader, cube->getMaterial());
    //setMaterial(shader, material);
    auto &lights = LIGHT->getLights();
    unsigned point_index = 0;
    for (const auto& it : lights)
    {
        if (it.second->type == RenderEngine::LightType::ePoint)
            setPointLight(shader, it.second, point_index++);
        else if (it.second->type == RenderEngine::LightType::eDirection)
            setDirectionLight(shader, it.second);
    }

    draw(cube->getVertexArray(), *shader);
}


void RenderSystem::Render(std::shared_ptr<RenderEngine::Model> model, const PositionComponent3& position, const CollisionComponent3& collision)
{
    if (!model)
        return;
    const auto shader = RES->getShader("assimp_3D");
    if (!shader)
    {
        return;
    }
    auto cam_pos = CAMERA->getCameraPosition();
    CAMERA->UseShader(shader);
    auto transform_model = getTransformModel(position, collision);
    //transform_model = glm::rotate(transform_model, glm::radians(static_cast<float>(-90.f)), glm::vec3(1.f, 0.f, 0.f));
    shader->setMatrix4("modelMatrix", transform_model);
    shader->setVec3("camera_position", cam_pos.cameraPos);
    shader->setInt("points_lights_count", LIGHT->getPointsLightsCount());
    auto& lights = LIGHT->getLights();
    unsigned point_index = 0;
    for (const auto& it : lights)
    {
        if (it.second->type == RenderEngine::LightType::ePoint)
            setPointLight(shader, it.second, point_index++);
        else if (it.second->type == RenderEngine::LightType::eDirection)
            setDirectionLight(shader, it.second);
    }
    for (const auto& it : model->meshes)
        draw(shader, it);
}

void RenderSystem::Render(std::shared_ptr<RenderEngine::Light> light)
{
    if (!light || light->type != RenderEngine::LightType::ePoint)
        return;
    const auto shader = RES->getShader("light_3D");
    if (!shader)
    {
        return;
    }
    CAMERA->UseShader(shader);
    shader->setMatrix4("modelMatrix", getTransformModel(dynamic_cast<RenderEngine::PointLight*>(light.get())->position, light->size));
    shader->setVec4("LightColor", { light->lightCube.getColor().getR(), light->lightCube.getColor().getG(), light->lightCube.getColor().getB(), light->lightCube.getColor().getAlpha() });

    draw(light->lightCube.getVertexArray(), *shader);
}

void RenderSystem::Render(std::shared_ptr<DisplayString> string, const PositionComponent& position, float scale, const ColorComponent& collor)
{
    if (!string || string->isEmpty())
        return;
    const auto shader = RES->getShader("freetype");
    CAMERA->UseShader(shader);
    shader->setVec3("textColor", { collor.getR(), collor.getG(), collor.getB() });
    shader->setFloat("layer", position.getLayer() + 1);
    shader->setFloat("alpha", collor.getAlpha() * GetLastTransformAlpha());
    Transform2D transform;
    if (auto t_r = LastTransform(); t_r.has_value())
        transform = *t_r;
    auto charList = string->getDisplayChars();
    double x = position.getPosition().mX, y = position.getPosition().mY;
    
    for(const auto& ch : charList)
    {
        const float y_offset = (ch.Bearing.y - static_cast<float>(ch.texture->getHeight())) * scale * transform.scaleY;
        const float x_offset = ch.Bearing.x * scale * transform.scaleX;
        const auto model = getTransformModel(
            x + x_offset, y + y_offset,
            ch.texture->getWidth() * scale,
            ch.texture->getHeight() * scale,
            position.getRotation());
        shader->setMatrix4("modelMatrix", model);
        ch.texture->bind();

        draw(string->getVertexArray(), string->getIndexCoordsBuffer(), *shader);
        x += (ch.Advance >> 6) * scale * transform.scaleX; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

void RenderSystem::Render(std::shared_ptr<DisplayString> string, const PositionComponent& position, const CollisionComponent& size, const ColorComponent& collor)
{
    if (!string || string->isEmpty())
        return;
    const auto shader = RES->getShader("freetype");
    CAMERA->UseShader(shader);
    shader->setVec3("textColor", { collor.getR(), collor.getG(), collor.getB() });
    shader->setFloat("layer", position.getLayer() + 1);
    shader->setFloat("alpha", collor.getAlpha() * GetLastTransformAlpha());

    auto charList = string->getDisplayChars();
    unsigned max_width = 0, max_height = 0;
    Transform2D transform;
    if (auto t_r = LastTransform(); t_r.has_value())
        transform = *t_r;
    for (const auto& ch : charList)
    {
        auto x = (ch.Advance >> 6) * size.getScale() * transform.scaleX;
        auto y = ch.texture->getHeight() * size.getScale() * transform.scaleY;
        if (x > max_width)
            max_width = x;
        if (y > max_height)
            max_height = y;
    }
    float rect_scale_x = size.getWidth() * transform.scaleX / static_cast<float>(max_width * charList.size());
    float rect_scale_y = max_height < size.getHeight() * transform.scaleY ? static_cast<float>(max_height) / size.getHeight() : size.getHeight() / static_cast<float>(max_height);

    double x = position.getPosition().mX + size.getXOffset(), y = position.getPosition().mY + size.getYOffset() * rect_scale_y;
    for (const auto& ch : charList)
    {
        const float y_offset = (ch.Bearing.y - static_cast<float>(ch.texture->getHeight())) * size.getScale() * rect_scale_y;
        const float x_offset = ch.Bearing.x * size.getScale();
        const auto model = getTransformModel(
            (x + x_offset), (y + y_offset),
            ch.texture->getWidth() * size.getScale() * rect_scale_x,
            ch.texture->getHeight() * size.getScale() * rect_scale_y,
            position.getRotation());
        shader->setMatrix4("modelMatrix", model);
        ch.texture->bind();

        draw(string->getVertexArray(), string->getIndexCoordsBuffer(), *shader);
        x += (ch.Advance >> 6) * size.getScale() * transform.scaleX * rect_scale_x; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

void RenderSystem::Render(const FRect& rect)
{
    drawRect(rect.mX, rect.mY, rect.mWidth, rect.mHeight);
}

void RenderSystem::Render(const Rect& rect)
{
    drawRect(rect.mX, rect.mY, rect.mWidth, rect.mHeight);
}

void RenderSystem::Render(const CollisionComponent& collision, const PositionComponent& position)
{
    if(!collision.getForm() || collision.getForm()->getPoints().size() < 3)
        return;
    auto drawable_points = collision.getForm()->getPoints();
    for(auto& it : drawable_points)
        it += position.getPosition();
    drawForm(drawable_points, collision.getScale());
}

void RenderSystem::setClearColor(float r, float g, float b, float alpha)
{
    clearCollorComponent = { r, g, b, alpha };
    glClearColor(r, g, b, alpha);
}

void RenderSystem::setBlendMode(bool on)
{
    if (on)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

void RenderSystem::setDepthTest(bool on)
{
    if (on)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void RenderSystem::setCullFace(bool on, int mode, int gl_dict)
{
    if (on)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(mode);
        glFrontFace(gl_dict);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

}

void RenderSystem::clear()
{
    if (isEnablePostProc())
    {
        m_framebuffer->bind();
        glEnable(GL_DEPTH_TEST);
    }
    glClearColor(clearCollorComponent.getR(), clearCollorComponent.getG(), clearCollorComponent.getB(), clearCollorComponent.getAlpha());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::PostRender()
{
    if (!isEnablePostProc())
        return;
    m_framebuffer->unbind();
    auto shader = RES->getShader("post_processing3D");
    shader->use();
    glDisable(GL_DEPTH_TEST);
    shader->setInt("screenTexture", 0);
    m_framebuffer->draw();
}

void RenderSystem::setMaterial(const std::shared_ptr<RenderEngine::ShaderProgram>& shader, const RenderEngine::Material& material)
{
    shader->setVec3("material.ambient", { material.ambient.x, material.ambient.y, material.ambient.z });
    shader->setVec3("material.diffuse", { material.diffuse.x, material.diffuse.y, material.diffuse.z });
    shader->setVec3("material.specular", { material.specular.x, material.specular.y, material.specular.z });
    shader->setFloat("material.shininess", material.shininess);
}

void RenderSystem::setMaterial(const std::shared_ptr<RenderEngine::ShaderProgram>& shader, const RenderEngine::TextureMaterial& material)
{
    if (material.m_pTexture)
    {
        shader->setInt("material.diffuse", material.m_pTexture->getSlot());
        material.m_pTexture->bind();
    }
    if (material.m_specularMap)
    {
        shader->setInt("material.specular", material.m_specularMap->getSlot());
        material.m_specularMap->bind();
    }
    shader->setFloat("material.shininess", material.shininess);
}

void RenderSystem::setPointLight(const std::shared_ptr<RenderEngine::ShaderProgram>& shader, const std::shared_ptr<RenderEngine::Light>& light, unsigned index)
{
    //!set default 
    PositionComponent3  l_pos{ 1.f, 1.f, 1.f };
    ColorComponent      l_col{ 1.f,1.f,1.f,1.f };
    Vector3             l_ambient{ 1.f, 1.f, 1.f };
    Vector3             l_diffuse{ 1.f, 1.f, 1.f };
    Vector3             l_specular{ 1.f, 1.f, 1.f };
    float               constant = 1.0f;
    float               linear = 0.0014f;
    float               quadratic = 0.000007f;
    //! read from struct 
    if (light)
    {
        l_col = light->lightCube.getColor();
        if (light->type == RenderEngine::LightType::ePoint)
        {
            l_pos = dynamic_cast<RenderEngine::PointLight*>(light.get())->position;
            constant = dynamic_cast<RenderEngine::PointLight*>(light.get())->constant;
            linear = dynamic_cast<RenderEngine::PointLight*>(light.get())->linear;
            quadratic = dynamic_cast<RenderEngine::PointLight*>(light.get())->quadratic;
        }
        l_ambient = light->ambient;
        l_diffuse = light->diffuse;
        l_specular = light->specular;
    }
    //! set to shader
    shader->setVec4(std::string{ "point_lights[" } + std::to_string(index) + "].color", { l_col.getR(), l_col.getG(), l_col.getB(), l_col.getAlpha() });
    shader->setVec3(std::string{ "point_lights[" } + std::to_string(index) + "].position", { l_pos.getPosition().mX, l_pos.getPosition().mY, l_pos.getPosition().mZ });
    //!��������
    shader->setVec3(std::string{ "point_lights[" } + std::to_string(index) + "].ambient", { l_ambient.x, l_ambient.y, l_ambient.z });
    shader->setVec3(std::string{ "point_lights[" } + std::to_string(index) + "].diffuse", { l_diffuse.x, l_diffuse.y, l_diffuse.z });
    shader->setVec3(std::string{ "point_lights[" } + std::to_string(index) + "].specular", { l_specular.x, l_specular.y, l_specular.z });
    //!���������
    shader->setFloat(std::string{ "point_lights[" } + std::to_string(index) + "].constant", constant);
    shader->setFloat(std::string{ "point_lights[" } + std::to_string(index) + "].linear", linear);
    shader->setFloat(std::string{ "point_lights[" } + std::to_string(index) + "].quadratic", quadratic);
}

void RenderSystem::setDirectionLight(const std::shared_ptr<RenderEngine::ShaderProgram>& shader, const std::shared_ptr<RenderEngine::Light>& light)
{
    //!set default 
    Vector3  l_direc{ 1.f, 1.f, 1.f };
    ColorComponent      l_col{ 1.f,1.f,1.f,1.f };
    Vector3             l_ambient{ 1.f, 1.f, 1.f };
    Vector3             l_diffuse{ 1.f, 1.f, 1.f };
    Vector3             l_specular{ 1.f, 1.f, 1.f };
    //! read from struct
    if (light)
    {
        l_col = light->lightCube.getColor();
        l_ambient = light->ambient;
        l_diffuse = light->diffuse;
        l_specular = light->specular;
        if (light->type == RenderEngine::LightType::eDirection)
        {
            l_direc = dynamic_cast<RenderEngine::DirectionLight*>(light.get())->direction;
        }
    }
    //! set to shader
    shader->setVec4("dirLight.color", { l_col.getR(), l_col.getG(), l_col.getB(), l_col.getAlpha() });
    shader->setVec3("dirLight.direction", { l_direc.x, l_direc.y, l_direc.z });
    //!��������
    shader->setVec3("dirLight.ambient", { l_ambient.x, l_ambient.y, l_ambient.z });
    shader->setVec3("dirLight.diffuse", { l_diffuse.x, l_diffuse.y, l_diffuse.z });
    shader->setVec3("dirLight.specular", { l_specular.x, l_specular.y, l_specular.z });
}

void RenderSystem::setViewport(unsigned int width, unsigned int height, unsigned int leftOffset, unsigned int bottomOffset)
{
    glViewport(leftOffset, bottomOffset, width, height);
}

glm::mat4 RenderSystem::getTransformMatrix(const float x, const float y, const float width, const float height, float rotation)
{
    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(x, y, 0.f));
    model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * height, 0.f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
    model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * height, 0.f));
    model = glm::scale(model, glm::vec3(width, height, 1.f));
    return model;
}

glm::mat4 RenderSystem::getTransformModel(const float x, const float y, const float width, const float height, float rotation)
{
    Transform2D def_Tr;
    if (!m_accumTransfStack.empty())
        def_Tr = m_accumTransfStack.top();

    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(x + def_Tr.transX, y + def_Tr.transY, 0.f));
    model = glm::translate(model, glm::vec3(0.5f * (width * def_Tr.scaleX), 0.5f * (height * def_Tr.scaleY), 0.f));
    model = glm::rotate(model, glm::radians(rotation + def_Tr.rotate), glm::vec3(0.f, 0.f, 1.f));
    model = glm::translate(model, glm::vec3(-0.5f * + (width * def_Tr.scaleX), -0.5f * (height * def_Tr.scaleY), 0.f));
    model = glm::scale(model, glm::vec3(width * def_Tr.scaleX, height * def_Tr.scaleY, 1.f));

    return model;
}

glm::mat4 RenderSystem::getTransformModel(const PositionComponent& position, const CollisionComponent& collision)
{
    Transform2D def_Tr;
    if (!m_accumTransfStack.empty())
        def_Tr = m_accumTransfStack.top();
    auto pos = position.getPosition();
    auto rotation = position.getRotation();
    auto width = collision.getWidth() * collision.getScale();
    auto height = collision.getHeight() * collision.getScale();

    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(pos.mX + def_Tr.transX, pos.mY + def_Tr.transY, 0.f));
    model = glm::translate(model, glm::vec3(0.5f * (width * def_Tr.scaleX), 0.5f * (height * def_Tr.scaleY), 0.f));
    model = glm::rotate(model, glm::radians(rotation + def_Tr.rotate), glm::vec3(0.f, 0.f, 1.f));
    model = glm::translate(model, glm::vec3(-0.5f * +(width * def_Tr.scaleX), -0.5f * (height * def_Tr.scaleY), 0.f));
    model = glm::scale(model, glm::vec3(width * def_Tr.scaleX, height * def_Tr.scaleY, 1.f));

    return model;
}

glm::mat4 RenderSystem::getTransformModel(const PositionComponent3& pos, const CollisionComponent3& coll)
{
    auto position = pos.getPosition();
    auto rotation = pos.getRotation();
    auto scale = coll.getScale();
    auto width = coll.getWidth();
    auto height = coll.getHeight();
    auto depth = coll.getDepth();
    auto x_offset = coll.getXOffset();
    auto y_offset = coll.getYOffset();
    auto z_offset = coll.getZOffset();

    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(position.mX, position.mY, position.mZ));
    model = glm::translate(model, glm::vec3(0.5f * (width * scale), 0.5f * (height * scale), 0.5f * (depth * scale)));

    model = glm::rotate(model, glm::radians(static_cast<float>(rotation.mX)), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(static_cast<float>(rotation.mY)), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(static_cast<float>(rotation.mZ)), glm::vec3(0.f, 0.f, 1.f));

    model = glm::translate(model, glm::vec3(-0.5f * (width * scale), -0.5f * (height * scale), -0.5f * (depth * scale)));
    model = glm::scale(model, glm::vec3(width * scale, height * scale, depth * scale));

    return model;
}

std::string RenderSystem::getRendererStr()
{
    return (char*)glGetString(GL_RENDERER);
}

std::string RenderSystem::getVersionStr()
{
    return (char*)glGetString(GL_VERSION);
}

void RenderSystem::PushGlobalTransform(Transform2D transform)
{
    if (!m_accumTransfStack.empty())
        transform += m_accumTransfStack.top();
    m_accumTransfStack.push(transform);
}

void RenderSystem::PopGlobalTransform()
{
    if (!m_accumTransfStack.empty())
        m_accumTransfStack.pop();
}

void RenderSystem::ClearGlobalTransform()
{
    if (!m_accumTransfStack.empty())
        m_accumTransfStack = std::stack<Transform2D>();
}

std::optional<Transform2D> RenderSystem::LastTransform()
{
    if (!m_accumTransfStack.empty())
        return m_accumTransfStack.top();
	return std::nullopt;
}

float RenderSystem::GetLastTransformAlpha()
{
    const auto last_transform = LastTransform();
    const float alpha = last_transform.has_value() ? last_transform->alpha : 1.f;
    return alpha;
}

bool RenderSystem::isEnablePostProc()
{
    return isEnablePostProcessing && m_framebuffer;
}

void RenderSystem::setEnablePostProc(bool enable)
{
    if (!isEnablePostProcessing && enable)
    {
        isEnablePostProcessing = enable;
        init();
    }
    else if (isEnablePostProcessing && !enable)
    {
        isEnablePostProcessing = enable;
        m_framebuffer.reset();
    }
}

void RenderSystem::draw(const std::shared_ptr<RenderEngine::ShaderProgram>& shader, const RenderEngine::Mesh& mesh)
{
    if (!shader)
        return;
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < mesh.textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        //std::string number;
        std::string name = mesh.textures[i]->getTextureType();
        //if (name == "material.diffuse")
        //    number = std::to_string(diffuseNr++);
        //else if (name == "material.specular")
        //    number = std::to_string(specularNr++); // transfer unsigned int to string
        if (name == "texture_normal" || name == "texture_height")
            continue;
 
        // now set the sampler to the correct texture unit
        shader->setInt(std::string("material." + RenderEngine::GetMaterialTextureName(name.c_str())), i);
        // and finally bind the texture
        mesh.textures[0]->bind();
    }
    shader->setFloat("material.shininess", 16.f);
    // draw mesh
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void RenderSystem::drawRect(const int x, const int y, const int width, const int height, ColorComponent collor)
{
    const auto shader_program = RES->getShader("default");
    if (!shader_program)
        return;
    static const GLfloat vertexes[] =
    {
        0.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, 0.f, 0.f
    };
    static const GLfloat colors[] =
    {
        collor.getR(), collor.getG(), collor.getB(), collor.getAlpha(),
        collor.getR(), collor.getG(), collor.getB(), collor.getAlpha(),
        collor.getR(), collor.getG(), collor.getB(), collor.getAlpha(),
        collor.getR(), collor.getG(), collor.getB(),  collor.getAlpha()
    };

    CAMERA->UseShader(shader_program);
    static RenderEngine::VertexArray vertexArray;
    static RenderEngine::VertexBuffer vertexBuffer;
    static RenderEngine::VertexBuffer colorsBuffer;

    vertexBuffer.bind();
    vertexBuffer.init(vertexes, sizeof(vertexes));

    colorsBuffer.bind();
    colorsBuffer.init(colors, sizeof(colors));

    vertexArray.bind();
    glEnableVertexAttribArray(0);
    vertexBuffer.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    colorsBuffer.bind();
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);

    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(glm::vec2{ x,y }, 0.f));
    model = glm::scale(model, glm::vec3(glm::vec2{ width,height }, 1.f));

    shader_program->setMatrix4("modelMatrix", model);

    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void RenderSystem::drawForm(const std::vector<FPoint>& points, float scale, ColorComponent color)
{
    const auto shader_program = RES->getShader("default");
    if (!shader_program)
        return;

    std::vector<GLfloat> vertexes;
    std::vector<GLfloat> colors;
    const auto max_y = std::max_element(points.begin(), points.end(), [&](const auto& left, const auto& right)
        {
            return left.mY < right.mY;
        })->mY;
    const auto max_x = std::max_element(points.begin(), points.end(), [&](const auto& left, const auto& right)
         {
             return left.mX < right.mX;
         })->mX;
    const auto min_y = std::min_element(points.begin(), points.end(), [&](const auto& left, const auto& right)
         {
             return left.mY < right.mY;
         })->mY;
    const auto min_x = std::min_element(points.begin(), points.end(), [&](const auto& left, const auto& right)
         {
             return left.mX < right.mX;
         })->mX;
    for(const auto& it : points)
    {
        auto norm_x = 1 + ((it.mX - min_x) / (max_x - min_x)) * (-1.f);
        auto norm_y = 1 + ((it.mY - min_y) / (max_y - min_y)) * (-1.f);
        vertexes.push_back(norm_x); vertexes.push_back(norm_y); vertexes.push_back(0);
        colors.push_back(color.getR()); colors.push_back(color.getG()); colors.push_back(color.getB()); colors.push_back(color.getAlpha());
    }

    CAMERA->UseShader(shader_program);
    static RenderEngine::VertexArray vertexArray;
    static RenderEngine::VertexBuffer vertexBuffer;
    static RenderEngine::VertexBuffer colorsBuffer;

    vertexBuffer.bind();
    vertexBuffer.init(vertexes.data(), vertexes.size() * sizeof(GLfloat));

    colorsBuffer.bind();
    colorsBuffer.init(colors.data(), colors.size() * sizeof(GLfloat));

    vertexArray.bind();
    glEnableVertexAttribArray(0);
    vertexBuffer.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    colorsBuffer.bind();
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);

    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(glm::vec2{ min_x, min_y }, 0.f));
    model = glm::scale(model, glm::vec3(glm::vec2{ (max_x - min_x) * scale, (max_y - min_y) * scale }, 1.f));

    shader_program->setMatrix4("modelMatrix", model);
    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP, 0, (int)vertexes.size()/3);
}

void RenderSystem::draw(const RenderEngine::VertexArray& vertexArray, const RenderEngine::IndexBuffer& indexBuffer, const RenderEngine::ShaderProgram& shader)
{
    shader.use();
    vertexArray.bind();
    indexBuffer.bind();

    glDrawElements(GL_TRIANGLES, indexBuffer.getCountElements(), GL_UNSIGNED_INT, nullptr);
}

void RenderSystem::draw(const RenderEngine::VertexArray& vertexArray, const RenderEngine::ShaderProgram& shader)
{
    shader.use();
    vertexArray.bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RenderSystem::init()
{
    if (isEnablePostProcessing)
    {
        m_framebuffer.reset();
        m_framebuffer = std::make_shared<RenderEngine::FrameBuffer>(CAMERA->getActiveWindowRect().mWidth, CAMERA->getActiveWindowRect().mHeight);
    }
}

Transform2D::Transform2D(float transX, float transY, float scaleX, float scaleY, float rotate, float alpha):
	transX(transX), transY(transY), scaleX(scaleX), scaleY(scaleY), rotate(rotate), alpha(alpha)
{}

void Transform2D::Translate(float transX, float transY)
{
    this->transX = transX;
    this->transY = transY;
}

void Transform2D::Scale(float scaleX, float scaleY)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;
}

void Transform2D::Rotate(float rotate)
{
    this->rotate = rotate;
}

const Transform2D& Transform2D::operator*=(const Transform2D& transform)
{
    transX *= transform.transX;
    transY *= transform.transY;
    scaleX *= transform.scaleX;
    scaleY *= transform.scaleY;
    rotate *= transform.rotate;
    alpha  *= transform.alpha;
    return *this;
}

const Transform2D& Transform2D::operator+=(const Transform2D& transform)
{
    transX += transform.transX;
    transY += transform.transY;
    scaleX += transform.scaleX;
    scaleY += transform.scaleY;
    rotate += transform.rotate;
    alpha  += transform.alpha;
    return *this;
}

const Transform2D& Transform2D::operator-=(const Transform2D& transform)
{
    transX -= transform.transX;
    transY -= transform.transY;
    scaleX -= transform.scaleX;
    scaleY -= transform.scaleY;
    rotate -= transform.rotate;
    alpha  -= transform.alpha;
    return *this;
}

Transform2D Transform2D::operator*(const Transform2D& transform)
{
    Transform2D t_r = *this;
    t_r *= transform;
    return t_r;
}

Transform2D Transform2D::operator+(const Transform2D& transform)
{
    Transform2D t_r = *this;
    t_r += transform;
    return t_r;
}

Transform2D Transform2D::operator-(const Transform2D& transform)
{
    Transform2D t_r = *this;
    t_r -= transform;
    return t_r;
}

Transform2DGuard::Transform2DGuard(const Transform2D& transform)
{
    RENDER->PushGlobalTransform(transform);
}

Transform2DGuard::~Transform2DGuard()
{
    RENDER->PopGlobalTransform();
}
