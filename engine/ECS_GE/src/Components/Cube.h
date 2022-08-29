#pragma once
#include <vector>
#include <memory>
#include "Image.h"
#include "Collor.h"

namespace RenderEngine {
	class Cube : public Image2D
	{
	public:
		Cube() = default;
		Cube(const std::shared_ptr<Texture2D>&pTexture);

		Cube(const std::shared_ptr<SpriteAtlas>&pAtlas,
			const SubTexture2D & sub_texture);

		~Cube() override = default;
		Cube(const Cube&);
		Cube& operator=(const Cube&);
		Cube(Cube&&) noexcept;
		Cube& operator=(Cube&&) noexcept;

		void SetSubTexture(const SubTexture2D& sub_texture) override;
		bool isMirrored() const override;
		void mirror(bool vertical, bool horizontal) override;
		void updateVertex(const void* data, const unsigned int data_size) override;
		bool isVerticalMirror() const override { return false; }
		bool isHorizontalMirror() const override { return false; }

	private:
		void init() override;
	};

	class LightCube
	{
	public:
		LightCube(const ColorComponent& color = ColorComponent{});
		~LightCube() = default;
		LightCube(const LightCube&) = delete;
		LightCube& operator=(const LightCube&) = delete;
		LightCube(LightCube&&) noexcept;
		LightCube& operator=(LightCube&&) noexcept;

		const VertexArray& getVertexArray() const { return m_vertexArray; }
		const VertexBuffer& getVertexCoordsBuffer() const { return m_vertexCoordsBuffer; }
		ColorComponent getColor() const { return m_color; }
		ColorComponent setColor(const ColorComponent& color);
	private:
		void init();
		ColorComponent	m_color;
		VertexArray		m_vertexArray;
		VertexBuffer	m_vertexCoordsBuffer;
	};
}