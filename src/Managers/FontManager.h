#pragma once
#include <ft2build.h>
#include <iostream>
#include <map>
#include <glm/vec2.hpp>

#include "gl_Include.h"
#include <glm/detail/type_vec3.hpp>
#include FT_FREETYPE_H


namespace RenderEngine
{
	class Texture2D;
}


#define FONT FontManager::Instanse()
class FontManager
{
public:
	static FontManager& Instanse();
	void freeFontRes();
	bool setFont(const std::string& path);
	bool init();
	bool isInit() const;
	const FT_Face& getFace();
private:
	FT_Face m_face;
	FT_Library m_libRef;
	bool initial = false;
	FontManager() = default;
	~FontManager();
	FontManager(const FontManager&) = delete;
	FontManager(FontManager&&) = delete;
	FontManager& operator=(const FontManager&) = delete;
	FontManager& operator=(FontManager&&) = delete;
};
