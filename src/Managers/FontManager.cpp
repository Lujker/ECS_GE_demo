#include "FontManager.h"

#include <iostream>

#include "texture2D.h"

FontManager& FontManager::Instanse()
{
	static FontManager font_manager;
	return font_manager;
}

void FontManager::freeFontRes()
{
	if (!initial)
		return;
	FT_Done_Face(m_face);   // Завершение работы с шрифтом face
	FT_Done_FreeType(m_libRef); // Завершение работы FreeType
	initial = false;
}

bool FontManager::setFont(const std::string& path)
{
	if (FT_New_Face(m_libRef, (std::string{ "bin/Debug/res/fonts/" } + path + std::string{ ".ttf" }).c_str(), 0, &m_face))
		std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
	else
	{
		return true;
	}
	return false;
}

bool FontManager::init()
{
	if (FT_Init_FreeType(&m_libRef))
		std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	else 
		initial = true;
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

FontManager::~FontManager()
{
	freeFontRes();
}