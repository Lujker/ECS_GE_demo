#pragma once
#include <ft2build.h>
#include <iostream>
#include <map>
#include <glm/vec2.hpp>

#include "gl_Include.h"
#include <glm/detail/type_vec3.hpp>
#include <optional>
#include FT_FREETYPE_H


struct Character;
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
	std::optional<Character> getCharacter(FT_ULong code_char);
	std::optional<Character> loadCharacter(FT_ULong code_char);
private:
	FT_Face m_face;
	FT_Library m_libRef;
	bool initial = false;
	std::map<GLchar, Character> char_map;
	FontManager() = default;
	~FontManager();
	FontManager(const FontManager&) = delete;
	FontManager(FontManager&&) = delete;
	FontManager& operator=(const FontManager&) = delete;
	FontManager& operator=(FontManager&&) = delete;
};
