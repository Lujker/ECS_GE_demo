#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "texture2D.h"
#include "Atlas2D.h"
#include "Image.h"
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
ResourceManager::imageMap ResourceManager::m_images;
ResourceManager::spriteMap ResourceManager::m_sprites;
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
		std::string name;
		std::string path;
		if (!current_atlas["name"].IsNull())
			name = current_atlas["name"].GetString();
		if (!current_atlas["path"].IsNull())
			path = current_atlas["path"].GetString();
		else continue;

		if (!name.empty())
			RES.loatAtlas(name, path);
		else
			RES.loatAtlas(path, path);
	}
}

void ResourceManager::loadImage(const rapidjson::Document::MemberIterator& image_iter)
{
	for (const auto& current_image : image_iter->value.GetArray())
	{
		std::string name;
		std::string path;
		if(!current_image["name"].IsNull())
			name = current_image["name"].GetString();
		if (!current_image["path"].IsNull())
			path = current_image["path"].GetString();
		if (!name.empty() && !path.empty())
			RES.loadImage(name, path, path);
		else if (!path.empty())
			RES.getSharedImage(path);
		else
			std::cerr << "Empty image obj in parsed json" << std::endl;
		
	}
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
	m_images.clear();
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
	const auto newTexture = m_textures.emplace(textureName, std::make_shared<RenderEngine::Texture2D>
														(pixels->getWidth(), pixels->getHeight(), pixels->getData(), pixels->getChanels(), GL_NEAREST, GL_CLAMP_TO_EDGE));
	if (newTexture.second)
		return newTexture.first->second;
	return getTexture(textureName);
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	auto it = m_textures.find(textureName);
	if (it != m_textures.end())
		return it->second;
	std::cerr << "Can't find the texture: " << textureName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::Image2D> ResourceManager::loadImage(const std::string& imageName, const std::string& textureName, const std::string& texturePath)
{
	auto texture = loadTexture(textureName, texturePath);
	const auto image = m_images.emplace(imageName, std::make_shared<RenderEngine::Image2D>(texture));
	if(image.second)
		return image.first->second;
	std::cerr << "Can't load the image: " << imageName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::Image2D> ResourceManager::getImage(const std::string& imageName)
{
	auto it = m_images.find(imageName);
	if (it != m_images.end())
		return it->second;
	std::cerr << "Can't find the image: " << imageName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::Image2D> ResourceManager::getSharedImage(const std::string& path)
{
	auto image = getImage(path);
	if (!image)
		image = loadImage(path, path, path);
	return image;
}

bool ResourceManager::removeSharedImage(const std::string& path)
{
	auto it = m_images.find(path);
	if (it != m_images.end())
	{
		m_images.erase(it);
		return true;
	}
	std::cerr << "Can't find the image: " << path << std::endl;
	return false;
}

std::shared_ptr<RenderEngine::TextureAtlas> ResourceManager::getAtlas(const std::string& textureName)
{
	auto it = m_atlases.find(textureName);
	if (it != m_atlases.end())
		return it->second;
	std::cerr << "Can't find the atlas: " << textureName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const std::string& path)
{
	return std::shared_ptr<RenderEngine::Sprite>();
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::loatSprite(const std::string& path)
{
	return std::shared_ptr<RenderEngine::Sprite>();
}

std::shared_ptr<RenderEngine::TextureAtlas> ResourceManager::loatAtlas(const std::string& name, const std::string& path)
{
	///Загрузка пикселей в массив байт из файла текстуры
	const auto& pixels = FILES.getPixelFile(path + ".png");
	if (!pixels->isLoaded() && !pixels->getData())
	{
		std::cerr << "Can't load atlas: " << path << std::endl;
		return nullptr;
	}	///Загрузка пикселей в массив байт из файла текстуры

	const auto& json_doc = getFileString(path + ".json");
	if (json_doc.empty())
	{
		std::cerr << "Failed to parse json: " << path << std::endl;
		return nullptr;
	}
	rapidjson::Document document;
	rapidjson::ParseResult parse_result = document.Parse(json_doc.c_str());
	if (!parse_result)
	{
		std::cerr << "Json parse error: " << rapidjson::GetParseError_En(parse_result.Code()) << " ("
			<< parse_result.Offset() << ") in file: " << path << std::endl;
		return nullptr;
	}
	std::shared_ptr<RenderEngine::TextureAtlas> newAtlas = std::make_shared<RenderEngine::TextureAtlas>
		(pixels->getWidth(), pixels->getHeight(), pixels->getData(), pixels->getChanels(), GL_NEAREST, GL_CLAMP_TO_EDGE);

	unsigned atlas_width = newAtlas->getWidth(), atlas_height = newAtlas->getHeight(), default_frame_width = 0, default_frame_height = 0, default_duration = 1;
	bool isLoop = false;
	const auto& size_iterator = document.FindMember("info");
	if (size_iterator != document.MemberEnd() && document["info"].IsObject())
	{
		if (!document["info"]["width"].IsNull())
			atlas_width = document["info"]["width"].GetUint();
		if (!document["info"]["height"].IsNull())
			atlas_height = document["info"]["height"].GetUint();
		if (!document["info"]["default_frame_width"].IsNull())
			default_frame_width = document["info"]["default_frame_width"].GetUint();
		if (!document["info"]["default_frame_height"].IsNull())
			default_frame_height = document["info"]["default_frame_height"].GetUint();
		if (!document["info"]["loop"].IsNull())
			isLoop = document["info"]["loop"].GetBool();
		if (!document["info"]["default_duration"].IsNull())
			default_duration = document["info"]["default_duration"].GetUint();
	}
	unsigned x_offset = 0, y_offset = 0;
	const auto& frames_iterator = document.FindMember("frames");
	if (frames_iterator != document.MemberEnd())
		for (const auto& current_frame : frames_iterator->value.GetArray())
		{
			std::string name = std::to_string(x_offset) + "x" + std::to_string(y_offset);
			if (!current_frame["name"].IsNull())
				name = current_frame["name"].GetString();
			if (!current_frame["offset_x"].IsNull())
				x_offset = current_frame["offset_x"].GetUint();
			if (!current_frame["offset_y"].IsNull())
				y_offset = current_frame["offset_y"].GetUint();

			unsigned width = 0, height = 0, duration = 0;
			if (!current_frame["width"].IsNull())
				width = current_frame["width"].GetUint();
			else if (default_frame_width > 0)
				width = default_frame_width;
			else
			{
				std::cerr << "Default_frame_width is null and width is null!" << std::endl;
				return nullptr;
			}
			if (!current_frame["height"].IsNull())
				height = current_frame["height"].GetUint();
			else if (default_frame_height > 0)
				height = default_frame_height;
			else
			{
				std::cerr << "Default_frame_height is null and height is null!" << std::endl;
				return nullptr;
			}
			if (!current_frame["duration"].IsNull())
				duration = current_frame["duration"].GetUint();
			else if (default_duration > 0)
				duration = default_duration;
			else
			{
				std::cerr << "Default_frame_width is null and width is null!" << std::endl;
				return nullptr;
			}

			RenderEngine::SubTexture2D sub_texture({ x_offset / atlas_width, y_offset / atlas_height }, { x_offset + width / atlas_width, y_offset + height / atlas_height }, duration);
			newAtlas->addSubTexture(name, sub_texture);
			//...

			x_offset += width;
			y_offset += height;
		}
	auto emp = m_atlases.emplace(
		name, newAtlas);
	if (emp.second)
		return newAtlas;
	return nullptr;
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

	const auto& images_iterator = document.FindMember("images");
	if (images_iterator != document.MemberEnd())
		loadImage(images_iterator);

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
