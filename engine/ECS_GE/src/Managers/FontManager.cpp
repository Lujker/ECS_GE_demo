#include "FontManager.h"
#include "Engine.h"
#include "LogSystem.h"
#include "texture2D.h"

void FontManager::freeFontRes()
{
	if (!initial)
		return;
	FT_Done_Face(m_face);   // ���������� ������ � ������� face
	FT_Done_FreeType(m_libRef); // ���������� ������ FreeType
	initial = false;
}

bool FontManager::setFont(const std::string& font_name)
{
	if (FT_New_Face(m_libRef, (to_res_path + font_name + std::string{ ".ttf" }).c_str(), 0, &m_face))
		LOG("ERROR::FREETYPE: Failed to load font", LOG_TYPE::WAR);
	else
	{
		return true;
	}
	return false;
}

bool FontManager::init(std::string pathToExe)
{
	if (FT_Init_FreeType(&m_libRef))
		LOG("ERROR::FREETYPE: Could not init FreeType Library", LOG_TYPE::WAR);
	else
	{
		const size_t found = pathToExe.find_last_of("/\\");
		to_res_path = pathToExe.substr(0, found) + "/\\res/\\fonts/\\";
		initial = true;
	}
	return initial;
}

bool FontManager::isInit() const
{
	return initial;
}

const FT_Face& FontManager::getFace()
{
	return m_face;
}

std::optional<Character> FontManager::getCharacter(FT_ULong code_char)
{
	const auto it = char_map.find(code_char);
	if (it != char_map.end())
		return it->second;
	else
		return loadCharacter(code_char);
}

std::optional<Character> FontManager::loadCharacter(FT_ULong code_char)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		// Load character glyph
	if (FT_Load_Char(getFace(), code_char, FT_LOAD_RENDER))
	{
		LOG("ERROR::FREETYTPE: Failed to load Glyph", LOG_TYPE::WAR);
		return std::nullopt;
	}
	// Now store character for later use
	const auto gluph = getFace()->glyph;
	Character character = {
		std::make_shared<RenderEngine::Texture2D>(gluph->bitmap.width, gluph->bitmap.rows, gluph->bitmap.buffer, 1),
		glm::ivec2(gluph->bitmap_left, gluph->bitmap_top),
		gluph->advance.x
	};
	const auto it = char_map.insert(std::pair{ code_char, character });
	return it.first->second;
}

FontManager::~FontManager()
{
	freeFontRes();
}