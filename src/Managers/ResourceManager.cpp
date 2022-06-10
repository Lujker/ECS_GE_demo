#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "texture2D.h"
#include "Atlas2D.h"
#include "Sprite.h"
#include "FileSystemManager.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <algorithm>
#include "stb_image.h"

ResourceManager::shaderProgramMap ResourceManager::m_shaderPrograms;
ResourceManager::texturesMap ResourceManager::m_textures;
ResourceManager::levels ResourceManager::m_levels;
ResourceManager::atlasMap ResourceManager::m_atlases;
std::string ResourceManager::m_path;

void ResourceManager::loadShader(const rapidjson::Document::MemberIterator& shaders_iterator)
{
	for (const auto& current_shader : shaders_iterator->value.GetArray())
	{
		std::string name = current_shader["name"].GetString();
		std::string file_path_vertex = current_shader["vertex_path"].GetString();
		std::string file_path_fragment = current_shader["fragment_path"].GetString();
		RES.loadShaders(name, file_path_vertex, file_path_fragment);
	}
}

void ResourceManager::loadTexture2D(const rapidjson::Document::MemberIterator& textur_iterator)
{
	for (const auto& current_texture : textur_iterator->value.GetArray())
	{
		std::string name = current_texture["name"].GetString();
		std::string path = current_texture["path"].GetString();
		RES.loadTexture(name, path);
	}
}
//! need rewrite
void ResourceManager::loadAtlas(const rapidjson::Document::MemberIterator& atlas_iterator)
{
	for (const auto& current_atlas : atlas_iterator->value.GetArray())
	{
		std::string name = current_atlas["name"].GetString();
		std::string path = current_atlas["path"].GetString();
		unsigned sub_texture_width = current_atlas["sub_texture_width"].GetUint();
		unsigned sub_texture_height = current_atlas["sub_texture_height"].GetUint();
		std::vector<std::string> subTexturesNames;
		for (const auto& sub_text_it : current_atlas["sub_textures"].GetArray())
		{
			subTexturesNames.emplace_back(sub_text_it.GetString());
		}
		RES.loadTextureAtlas(name, subTexturesNames, path, sub_texture_width, sub_texture_height);
	}
}

void ResourceManager::loadImage(const rapidjson::Document::MemberIterator& image_iter)
{
}

void ResourceManager::loadLevel(const rapidjson::Document::MemberIterator& levels_iterator)
{
	for (const auto& current_level : levels_iterator->value.GetArray())
	{
		const auto& description = current_level["description"].GetArray();
		std::vector<std::string> level;
		size_t max_length = 0;
		for (const auto& level_row : description)
		{
			level.emplace_back(level_row.GetString());
			if (max_length < level.back().length())
				max_length = level.back().length();
		}
		for (auto& raw : level)
		{
			while (raw.length() < max_length)
			{
				raw.append("D");
			}
		}
		m_levels.emplace_back(level);
	}
}

ResourceManager::ResourceManager(const std::string& execPath)
{
	size_t found = execPath.find_last_of("/\\");
	m_path = execPath.substr(0, found);
}

ResourceManager::~ResourceManager()
{
	unloadAllResources();
}

ResourceManager& ResourceManager::Instanse()
{
	static ResourceManager manager;
	return manager;
}

void ResourceManager::setExecutablePath(const std::string& execPath)
{
	size_t found = execPath.find_last_of("/\\");
	m_path = execPath.substr(0, found);
}

void ResourceManager::unloadAllResources()
{
	m_shaderPrograms.clear();
	m_textures.clear();
	m_levels.clear();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexShader = getFileString(vertexPath);
	if (vertexShader.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}
	std::string fragmentShader = getFileString(fragmentPath);
	if (fragmentShader.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexShader, fragmentShader)).first->second;
	if (newShader->isCompiled())
		return newShader;

	std::cerr << "Can't load shader program: \n"
		<< " Vertex: " << vertexPath << std::endl
		<< " Fragment: " << fragmentPath << std::endl;
	return nullptr;
	
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShader(const std::string& shaderName)
{
	shaderProgramMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the shader program: " << shaderName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	///Загрузка пикселей в массив байт из файла текстуры
	const auto& pixels = FILES.getPixelFile(texturePath);
	if (!pixels->isLoaded() && !pixels->getData())
	{
		std::cerr << "Can't load image: " << texturePath << std::endl;
		return nullptr;
	}
	std::shared_ptr<RenderEngine::Texture2D> newTexture = m_textures.emplace(
														textureName, std::make_shared<RenderEngine::Texture2D>
														(pixels->getWidth(), pixels->getHeight(), pixels->getData(), pixels->getChanels(), GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;
	return newTexture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	auto it = m_textures.find(textureName);
	if (it != m_textures.end())
		return it->second;
	std::cerr << "Can't find the texture: " << textureName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::TextureAtlas> ResourceManager::getAtlas(const std::string& textureName)
{
	auto it = m_atlases.find(textureName);
	if (it != m_atlases.end())
		return it->second;
	std::cerr << "Can't find the atlas: " << textureName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::TextureAtlas> ResourceManager::loadTextureAtlas(const std::string& textureName,
		std::vector<std::string> subTextures,
		const std::string& texturePath,
		const unsigned int subTextureWidth,
		const unsigned int subTextureHeight)
{
	///Загрузка пикселей в массив байт из файла текстуры
	const auto& pixels = FILES.getPixelFile(texturePath);
	if (!pixels->isLoaded() && !pixels->getData())
	{
		std::cerr << "Can't load image: " << texturePath << std::endl;
		return nullptr;
	}	///Загрузка пикселей в массив байт из файла текстуры

	std::shared_ptr<RenderEngine::TextureAtlas> newAtlas = m_atlases.emplace(
		textureName, std::make_shared<RenderEngine::TextureAtlas>
		(pixels->getWidth(), pixels->getHeight(), pixels->getData(), pixels->getChanels(), GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;
	
	if (newAtlas)
	{
		const unsigned int textureWidth = newAtlas->getWidth();
		const unsigned int textureHeight = newAtlas->getHeight();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;
		for (const auto& currentSubTextureName : subTextures)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight );
			newAtlas->addSubTexture(currentSubTextureName, leftBottomUV, rightTopUV);
			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}
	}
	return newAtlas;
}

bool ResourceManager::loadResJSON(const std::string& path)
{
	const auto& json_doc = getFileString(path);
	if (json_doc.empty())
	{
		std::cerr << "Failed to parse json: " << path << std::endl;
		return false;
	}
	rapidjson::Document document;
	rapidjson::ParseResult parse_result = document.Parse(json_doc.c_str());
	if (!parse_result)
	{
		std::cerr << "Json parse error: " << rapidjson::GetParseError_En(parse_result.Code()) << " ("
		<< parse_result .Offset()<< ") in file: "<< path << std::endl;
		return false;
	}

	const auto& shaders_iterator = document.FindMember("shaders");
	if (shaders_iterator != document.MemberEnd())	
		loadShader(shaders_iterator);

	const auto& textur_iterator = document.FindMember("textures");
	if (textur_iterator != document.MemberEnd())
		loadTexture2D(textur_iterator);

	const auto& atlas_iterator = document.FindMember("atlases");
	if (atlas_iterator != document.MemberEnd())
		loadAtlas(atlas_iterator);

	auto levelsIt = document.FindMember("levels");
	if (levelsIt != document.MemberEnd())
		loadLevel(levelsIt);

	FILES.unloadAllFiles();
	return true;
}

const std::vector<std::vector<std::string>>& ResourceManager::getLevels()
{
	return m_levels;
}

const std::vector<std::string>& ResourceManager::atLevelDiscription(const unsigned& index)
{
	return m_levels.at(index);
}

std::string ResourceManager::getFileString(const std::string& path)
{
	if(const auto& ptr = FILES.getFile(path); ptr)
		return ptr->getData();
	return "";
}
