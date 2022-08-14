#pragma once
#include <ft2build.h>
#include <iostream>
#include <map>
#include <glm/vec2.hpp>
#include "gl_Include.h"
#include <glm/detail/type_vec3.hpp>
#include <optional>
#include FT_FREETYPE_H

namespace RenderEngine
{
	class Texture2D;
}

struct Character {
	std::shared_ptr<RenderEngine::Texture2D> texture; // �������� �����
	glm::ivec2 Bearing;   // �������� ������� ����� ����� �����
	FT_Pos     Advance;   // �������������� �������� �� ������ ���������� �����
};

class FontManager
{
public:
	friend class ServiceLocator;
	void freeFontRes();
	bool setFont(const std::string& path);
	bool init(std::string pathToExe);
	bool isInit() const;
	const FT_Face& getFace();
	std::optional<Character> getCharacter(FT_ULong code_char);
	std::optional<Character> loadCharacter(FT_ULong code_char);
private:
	FT_Face m_face;
	FT_Library m_libRef;
	bool initial = false;
	std::map<GLchar, Character> char_map;
	std::string to_res_path;
	FontManager() = default;
	~FontManager();
	FontManager(const FontManager&) = delete;
	FontManager(FontManager&&) = delete;
	FontManager& operator=(const FontManager&) = delete;
	FontManager& operator=(FontManager&&) = delete;
};
