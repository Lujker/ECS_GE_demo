#include "RenderSystem.h"
#include "CameraManager.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
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
    auto size = sprite->getTextureSize();
    if (collision.isValid())
    {
        size = collision.getSize();
    }

    shader->use();
    shader->setMatrix4("modelMatrix", RENDER.getTransformMatrix(position.getPosition().mX, position.getPosition().mY, size.mX, size.mY, position.getRotation()));
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

    shader->use();
	shader->setMatrix4("modelMatrix", RENDER.getTransformMatrix(position.getPosition().mX, position.getPosition().mY, size.mX, size.mY, position.getRotation()));
	shader->setFloat("layer", position.getLayer());
    shader->setInt("tex", image->getTexture2D()->getSlot());
    image->getTexture2D()->bind();

	draw(image->getVertexArray(), image->getIndexCoordsBuffer(), *shader);
}

void RenderSystem::Render(const FRect& rect)
{
    drawRect(rect.mX, rect.mY, rect.mWidth, rect.mHeight);
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
    auto shader_program = RES.getShader("default");
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

    shader_program->use();
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
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void RenderSystem::draw(const RenderEngine::VertexArray& vertexArray, const RenderEngine::IndexBuffer& indexBuffer, const RenderEngine::ShaderProgram& shader)
{
    shader.use();
    vertexArray.bind();
    indexBuffer.bind();

    glDrawElements(GL_TRIANGLES, indexBuffer.getCountElements(), GL_UNSIGNED_INT, nullptr);
}