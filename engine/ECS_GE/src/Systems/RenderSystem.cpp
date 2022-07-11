#include "RenderSystem.h"

#include "CameraManager.h"
#include "DisplayString.h"
#include "FontManager.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include <algorithm>
#include "Sprite.h"

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
    const auto shader = RES.getShader("image_shader");
    if (!shader || !sprite->isValid())
    {
        return;
    }
    auto size = sprite->getCurrentAnimSize();
    if (collision.isValid())
    {
        size = collision.getSize();
    }

    CAMERA.UseShader(shader);
    shader->setMatrix4("modelMatrix", RENDER.getTransformMatrix(position.getPosition().mX, position.getPosition().mY, size.mX * collision.getScale(), size.mY * collision.getScale(), position.getRotation()));
    shader->setFloat("layer", position.getLayer());
    shader->setInt("tex", sprite->getTexture2D()->getSlot());
    sprite->getTexture2D()->bind();

    draw(sprite->getVertexArray(), sprite->getIndexCoordsBuffer(), *shader);
}

void RenderSystem::Render(std::shared_ptr<RenderEngine::Image2D> image, const PositionComponent& position, const CollisionComponent& collision)
{
    if(!image)
        return;
	const auto shader = RES.getShader("image_shader");
    if(!shader || !image->isValid())
    {
        return;
    }
    auto size = image->getTextureSize();
    if (collision.isValid())
    {
        size = collision.getSize();
    }

    CAMERA.UseShader(shader);
	shader->setMatrix4("modelMatrix", RENDER.getTransformMatrix(position.getPosition().mX, position.getPosition().mY, size.mX * collision.getScale(), size.mY * collision.getScale(), position.getRotation()));
	shader->setFloat("layer", position.getLayer());
    shader->setInt("tex", image->getTexture2D()->getSlot());
    image->getTexture2D()->bind();

	draw(image->getVertexArray(), image->getIndexCoordsBuffer(), *shader);
}

void RenderSystem::Render(std::shared_ptr<DisplayString> string, const PositionComponent& position, float scale, const ColorComponent& collor)
{
    if (!string || string->isEmpty())
        return;
    const auto shader = RES.getShader("freetype");
    CAMERA.UseShader(shader);
    shader->setVec3("textColor", { collor.getR(), collor.getG(), collor.getB() });
    shader->setFloat("layer", position.getLayer() + 1);
    shader->setFloat("alpha", collor.getAlpha());

    auto charList = string->getDisplayChars();
    unsigned x = position.getPosition().mX, y = position.getPosition().mY;
    
    for(const auto& ch : charList)
    {
        const float y_offset = (ch.Bearing.y - static_cast<float>(ch.texture->getHeight())) * scale;
        const float x_offset = ch.Bearing.x * scale;
        const auto model = getTransformMatrix(
            x + x_offset, y + y_offset,
            ch.texture->getWidth() * scale,
            ch.texture->getHeight() * scale,
            position.getRotation());
        shader->setMatrix4("modelMatrix", model);
        ch.texture->bind();

        draw(string->getVertexArray(), string->getIndexCoordsBuffer(), *shader);
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

void RenderSystem::Render(std::shared_ptr<DisplayString> string, const PositionComponent& position, const CollisionComponent& collision, const ColorComponent& collor)
{
    if (!string || string->isEmpty())
        return;
    const auto shader = RES.getShader("freetype");
    CAMERA.UseShader(shader);
    shader->setVec3("textColor", { collor.getR(), collor.getG(), collor.getB() });
    shader->setFloat("layer", position.getLayer() + 1);
    shader->setFloat("alpha", collor.getAlpha());

    auto charList = string->getDisplayChars();
    unsigned x = position.getPosition().mX + collision.getXOffset(), y = position.getPosition().mY + collision.getYOffset();
    for (const auto& ch : charList)
    {
        const float y_offset = (ch.Bearing.y - static_cast<float>(ch.texture->getHeight())) * collision.getScale();
        const float x_offset = ch.Bearing.x * collision.getScale();
        const auto model = getTransformMatrix(
            x + x_offset, y + y_offset,
            ch.texture->getWidth() * collision.getScale(),
            ch.texture->getHeight() * collision.getScale(),
            position.getRotation());
        shader->setMatrix4("modelMatrix", model);
        ch.texture->bind();

        draw(string->getVertexArray(), string->getIndexCoordsBuffer(), *shader);
        x += (ch.Advance >> 6) * collision.getScale(); // Bitshift by 6 to get value in pixels (2^6 = 64)
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

void RenderSystem::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

std::string RenderSystem::getRendererStr()
{
    return (char*)glGetString(GL_RENDERER);
}

std::string RenderSystem::getVersionStr()
{
    return (char*)glGetString(GL_VERSION);
}

void RenderSystem::drawRect(const int x, const int y, const int width, const int height)
{
    const auto shader_program = RES.getShader("default");
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
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f
    };

    CAMERA.UseShader(shader_program);
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(glm::vec2{ x,y }, 0.f));
    model = glm::scale(model, glm::vec3(glm::vec2{ width,height }, 1.f));

    shader_program->setMatrix4("modelMatrix", model);

    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void RenderSystem::drawForm(const std::vector<FPoint>& points, float scale)
{
    const auto shader_program = RES.getShader("default");
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
        colors.push_back(1); colors.push_back(0); colors.push_back(0);
    }

    CAMERA.UseShader(shader_program);
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

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