#pragma once
#include "Position.h"
#include "Components/Image.h"
#include "Components/Collision.h"
#include "gl_Include.h"

#define RENDER RenderSystem::Instanse()
class RenderSystem
{
public:
	static RenderSystem& Instanse();
	static void Render(const RenderEngine::Image2D& image, const PositionComponent& position, const CollisionComponent& collision = {false});
	static void Render(const FRect& rect);
	static void setClearColor(float r, float g, float b, float alpha);
	static void setBlendMode(bool on);
	static void setDepthTest(bool on);
	static void clear();
	static void setViewport(unsigned int width, unsigned int height, unsigned int leftOffset = 0, unsigned int bottomOffset = 0);
	static glm::mat4 getTransformMatrix(const float x, const float y, const float width, const float height, float rotation = 0);
	static std::string getRendererStr();
	static std::string getVersionStr();
private:
	static void drawRect(const int x, const int y, const int width, const int height);
	static void draw(const RenderEngine::VertexArray& vertexArray, const RenderEngine::IndexBuffer& indexBuffer, const RenderEngine::ShaderProgram& shader);
	RenderSystem() = default;
	~RenderSystem() = default;
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem(RenderSystem&&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	RenderSystem& operator=(RenderSystem&&) = delete;
};