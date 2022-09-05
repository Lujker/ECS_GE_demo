#pragma once
#include <optional>
#include <stack>
#include <glm/ext/matrix_float4x4.hpp>

#include "Collor.h"
#include "Position.h"
#include "Collision.h"
#include "gl_Include.h"
#include "ISystem.h"
#include "Image.h"
#include "Material.h"
#include <unordered_set>

class RenderSystem;
class DisplayString;

namespace RenderEngine
{
	class Sprite;
	class ShaderProgram;
	class Cube;
	class Model;
	class Mesh;
	class LightCube;
	struct Light;
	class FrameBuffer;
	class SkyBox;
	class Sphere;
}

struct Transform2D
{
	friend RenderSystem;
	Transform2D() = default;
	explicit Transform2D(float transX, float transY, float scaleX = 1.f, float scaleY = 1.f, float rotate = 0.f, float alpha = 1.f);
	~Transform2D() = default;
	void Translate(float transX, float transY);
	void Scale(float scaleX, float scaleY);
	void Rotate(float rotate);

	const Transform2D& operator*=(const Transform2D&);
	const Transform2D& operator+=(const Transform2D&);
	const Transform2D& operator-=(const Transform2D&);

	Transform2D operator*(const Transform2D&);
	Transform2D operator+(const Transform2D&);
	Transform2D operator-(const Transform2D&);
	
	float	transX = 0.f, transY = 0.f,
			scaleX = 1.f, scaleY = 1.f,
			rotate = 0.f, alpha = 1.;
};

class Transform2DGuard
{
public:
	explicit Transform2DGuard(const Transform2D&);
	~Transform2DGuard();
private:
	Transform2D transform;
};

class RenderSystem : public ISystem
{
public:
	friend class ServiceLocator;
	static RenderSystem& Instanse();
	void Render(const std::shared_ptr<RenderEngine::Sprite>& sprite, const PositionComponent& position, const CollisionComponent& collision = { false });
	void Render(const std::shared_ptr<RenderEngine::Image2D>& image, const PositionComponent& position, const CollisionComponent& collision = { false });
	void Render(const std::shared_ptr<RenderEngine::Cube>& cube, const PositionComponent3& position, const CollisionComponent3& collision = { false });
	void Render(const std::shared_ptr<RenderEngine::Model>& model, const PositionComponent3& position, const CollisionComponent3& collision = { false });
	void Render(const std::shared_ptr<RenderEngine::Light>& light);
	void Render(const std::shared_ptr<RenderEngine::SkyBox>& skyBox);
	void Render(const std::shared_ptr<RenderEngine::Sphere>& sphere, const PositionComponent3& position, const CollisionComponent3& collision = { false });
	void Render(const std::shared_ptr<DisplayString>& string, const PositionComponent& position, float scale = 1.f, const ColorComponent& collor = ColorComponent{0.5f,0.5f,0.5f});
	void Render(const std::shared_ptr<DisplayString>& string, const PositionComponent& position, const CollisionComponent& size = { false }, const ColorComponent& collor = ColorComponent{ 0.5f,0.5f,0.5f });
	void Render(const FRect& rect);
	void Render(const Rect& rect);
	void Render(const CollisionComponent& collision, const PositionComponent& position);
	void setClearColor(float r, float g, float b, float alpha);
	void setBlendMode(bool on);
	void setDepthTest(bool on);
	void setCullFace(bool on, int mode = GL_BACK, int gl_dict = GL_CCW);
	void clear();
	void PostRender();
	void setMaterial(const std::shared_ptr<RenderEngine::ShaderProgram>& shader, const RenderEngine::Material& material);
	void setMaterial(const std::shared_ptr<RenderEngine::ShaderProgram>& shader, const RenderEngine::TextureMaterial& material);
	void setPointLight(const std::shared_ptr<RenderEngine::ShaderProgram>& shader, const std::shared_ptr<RenderEngine::Light>& light, unsigned index);
	void setDirectionLight(const std::shared_ptr<RenderEngine::ShaderProgram>& shader, const std::shared_ptr<RenderEngine::Light>& light);
	void setViewport(unsigned int width, unsigned int height, unsigned int leftOffset = 0, unsigned int bottomOffset = 0);
	glm::mat4 getTransformMatrix(const float x, const float y, const float width, const float height, float rotation = 0);
	glm::mat4 getTransformModel(const float x, const float y, const float width, const float height, float rotation = 0);
	glm::mat4 RenderSystem::getTransformModel(const PositionComponent& position, const CollisionComponent& collision);
	glm::mat4 RenderSystem::getTransformModel(const PositionComponent3& position, const CollisionComponent3& collision);
	std::string getRendererStr();
	std::string getVersionStr();

	void PushGlobalTransform(Transform2D);
	void PopGlobalTransform();
	void ClearGlobalTransform();
	std::optional<Transform2D> LastTransform();
	float GetLastTransformAlpha();
	bool isEnablePostProc();
	void setEnablePostProc(bool enable);

	RenderSystem(const RenderSystem&) = delete;
	RenderSystem(RenderSystem&&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	RenderSystem& operator=(RenderSystem&&) = delete;
private:

	void draw(const std::shared_ptr<RenderEngine::ShaderProgram>& shader, const RenderEngine::Mesh& mesh);
	void drawRect(const int x, const int y, const int width, const int height, ColorComponent = {1,0,0,1});
	void drawForm(const std::vector<FPoint>& points, float scale, ColorComponent = { 1,0,0,1 });
	void draw(const RenderEngine::VertexArray& vertexArray, const RenderEngine::IndexBuffer& indexBuffer, const RenderEngine::ShaderProgram& shader);
	void draw(const RenderEngine::VertexArray& vertexArray, const RenderEngine::ShaderProgram& shader);
	void init();

	std::shared_ptr<RenderEngine::FrameBuffer> m_framebuffer = nullptr;
	ColorComponent clearCollorComponent = { 1.f,1.f,1.f,1.f };
	bool isEnablePostProcessing = false;
	std::stack<Transform2D> m_accumTransfStack;
	RenderSystem() = default;
	~RenderSystem() = default;
};
