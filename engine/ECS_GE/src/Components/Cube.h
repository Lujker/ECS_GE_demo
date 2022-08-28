#pragma once
#include <vector>
#include <memory>
#include "Image.h"

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
}