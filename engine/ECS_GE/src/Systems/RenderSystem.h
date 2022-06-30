#pragma once
#include <glm/ext/matrix_float4x4.hpp>

#include "Collor.h"
#include "Position.h"
#include "Image.h"
#include "Collision.h"
#include "gl_Include.h"
#include "ISystem.h"

#define RENDER RenderSystem::Instanse()

class DisplayString;

namespace RenderEngine
{
	class Sprite;
	class ShaderProgram;
}

class RenderSystem : public ISystem
{
public:
	static RenderSystem& Instanse();
	void Render(std::shared_ptr<RenderEngine::Sprite> sprite, const PositionComponent& position, const CollisionComponent& collision = { false });
	void Render(std::shared_ptr<RenderEngine::Image2D> image, const PositionComponent& position, const CollisionComponent& collision = { false });
	void Render(std::shared_ptr<DisplayString> string, const PositionComponent& position, float scale = 1.f, const CollorComponent& collor = CollorComponent{1.f,1.f,1.f});
	void Render(const FRect& rect);
	void Render(const CollisionComponent& collision, const PositionComponent& position);
	void setClearColor(float r, float g, float b, float alpha);
	void setBlendMode(bool on);
	void setDepthTest(bool on);
	void clear();
	void setViewport(unsigned int width, unsigned int height, unsigned int leftOffset = 0, unsigned int bottomOffset = 0);
	glm::mat4 getTransformMatrix(const float x, const float y, const float width, const float height, float rotation = 0);
	std::string getRendererStr();
	std::string getVersionStr();
private:
	void drawRect(const int x, const int y, const int width, const int height);
	void drawForm(const std::vector<FPoint>& points, float scale);
	void draw(const RenderEngine::VertexArray& vertexArray, const RenderEngine::IndexBuffer& indexBuffer, const RenderEngine::ShaderProgram& shader);
	RenderSystem() = default;
	~RenderSystem() = default;
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem(RenderSystem&&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	RenderSystem& operator=(RenderSystem&&) = delete;
};
