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

#include "LogSystem.h"
#include "stb_image.h"

ResourceManager::shaderProgramMap ResourceManager::m_shaderPrograms;
ResourceManager::texturesMap ResourceManager::m_textures;
ResourceManager::levels ResourceManager::m_levels;
ResourceManager::spriteAtlasMap ResourceManager::m_SpriteAtlases;
ResourceManager::frameAtlasMap ResourceManager::m_FrameAtlases;
ResourceManager::imageMap ResourceManager::m_images;
ResourceManager::spriteMap ResourceManager::m_sprites;
std::string ResourceManager::m_path;

std::shared_ptr<RenderEngine::SpriteAtlas> ResourceManager::loadSpriteAtlas(std::shared_ptr<RenderEngine::SpriteAtlas> newAtlas, const rapidjson::Document& document)
{
	unsigned atlas_width = newAtlas->getWidth(), atlas_height = newAtlas->getHeight(), default_frame_width = 0, default_frame_height = 0, default_duration = 1000;
	const auto& size_iterator = document.FindMember("info");
	if (size_iterator != document.MemberEnd() && document["info"].IsObject())
	{
		if (document["info"].HasMember("width") && !document["info"]["width"].IsNull())
			atlas_width = document["info"]["width"].GetUint();
		if (document["info"].HasMember("height") && !document["info"]["height"].IsNull())
			atlas_height = document["info"]["height"].GetUint();
		if (document["info"].HasMember("default_frame_width") && !document["info"]["default_frame_width"].IsNull())
			default_frame_width = document["info"]["default_frame_width"].GetUint();
		if (document["info"].HasMember("default_frame_height") && !document["info"]["default_frame_height"].IsNull())
			default_frame_height = document["info"]["default_frame_height"].GetUint();
		if (document["info"].HasMember("default_duration") && !document["info"]["default_duration"].IsNull())
			default_duration = document["info"]["default_duration"].GetUint();
	}
	//! TODO parse animations to anim list
	unsigned x_offset = 0, y_offset = atlas_height - default_frame_height;
	const auto& animations = document.FindMember("animations");
	if (animations != document.MemberEnd() && animations->value.IsArray())
	{
		for (const auto& anim_iter : animations->value.GetArray())
		{
			std::string name = std::to_string(x_offset) + "x" + std::to_string(y_offset);
			bool isLoop = false;
			if (anim_iter.HasMember("name") && !anim_iter["name"].IsNull())
				name = anim_iter["name"].GetString();
			if (anim_iter.HasMember("loop") && !anim_iter["loop"].IsNull())
				isLoop = anim_iter["loop"].GetBool();

			const auto& frames_iterator = anim_iter.FindMember("frames");
			if (RenderEngine::SpriteAtlas::animations_frame animations_frame;
				frames_iterator != document.MemberEnd())
			{
				for (const auto& current_frame : frames_iterator->value.GetArray())
				{
					if (current_frame.HasMember("offset_x") && !current_frame["offset_x"].IsNull())
						x_offset = current_frame["offset_x"].GetUint();
					if (current_frame.HasMember("offset_y") && !current_frame["offset_y"].IsNull())
						y_offset = current_frame["offset_y"].GetUint();

					unsigned width = 0, height = 0, duration = 0;
					if (current_frame.HasMember("width") && !current_frame["width"].IsNull())
						width = current_frame["width"].GetUint();
					else if (default_frame_width > 0)
						width = default_frame_width;
					else
					{
						LOG("Default_frame_width is null and width is null!", LOG_TYPE::WAR);
						return nullptr;
					}
					if (current_frame.HasMember("height") && !current_frame["height"].IsNull())
						height = current_frame["height"].GetUint();
					else if (default_frame_height > 0)
						height = default_frame_height;
					else
					{
						LOG("Default_frame_height is null and height is null!", LOG_TYPE::WAR);
						return nullptr;
					}
					if (current_frame.HasMember("duration") && !current_frame["duration"].IsNull())
						duration = current_frame["duration"].GetUint();
					else if (default_duration > 0)
						duration = default_duration;
					else
					{
						LOG("Default_frame_width is null and width is null!", LOG_TYPE::WAR);
						return nullptr;
					}

					RenderEngine::SubTexture2D sub_texture(
						{ static_cast<float>(x_offset) / static_cast<float>(atlas_width),
							static_cast<float>(y_offset) / static_cast<float>(atlas_height) },
						{ static_cast<float>((x_offset + width)) / static_cast<float>(atlas_width),
							static_cast<float>((y_offset + height)) / static_cast<float>(atlas_height) },
						duration);
					animations_frame.second.push_back(sub_texture);
					//...

					x_offset += width;
					if (x_offset == atlas_width)
					{
						x_offset = 0;
						y_offset -= height;
					}
				}
				animations_frame.first = isLoop;
				newAtlas->addAnimation(name, animations_frame);
			}
			else
				continue;
		}
	}
	return newAtlas;
}

std::shared_ptr<RenderEngine::FrameAtlas>  ResourceManager::loadFrameAtlas(std::shared_ptr<RenderEngine::FrameAtlas> newAtlas, const rapidjson::Document& document)
{
	unsigned atlas_width = newAtlas->getWidth(), atlas_height = newAtlas->getHeight(), default_frame_width = 0, default_frame_height = 0;
	const auto& size_iterator = document.FindMember("info");
	if (size_iterator != document.MemberEnd() && document["info"].IsObject())
	{
		if (document["info"].HasMember("width") && !document["info"]["width"].IsNull())
			atlas_width = document["info"]["width"].GetUint();
		if (document["info"].HasMember("height") && !document["info"]["height"].IsNull())
			atlas_height = document["info"]["height"].GetUint();
		if (document["info"].HasMember("default_frame_width") && !document["info"]["default_frame_width"].IsNull())
			default_frame_width = document["info"]["default_frame_width"].GetUint();
		if (document["info"].HasMember("default_frame_height") && !document["info"]["default_frame_height"].IsNull())
			default_frame_height = document["info"]["default_frame_height"].GetUint();
	}

	unsigned x_offset = 0, y_offset = atlas_height - default_frame_height;
	const auto& frames_iterator = document.FindMember("frames");
	if (frames_iterator != document.MemberEnd() && frames_iterator->value.IsArray())
	{
		for (const auto& current_frame : frames_iterator->value.GetArray())
		{
			std::string name = std::to_string(x_offset) + "x" + std::to_string(y_offset);
			if (current_frame.HasMember("name") && !current_frame["name"].IsNull())
				name = current_frame["name"].GetString();

			if (current_frame.HasMember("offset_x") && !current_frame["offset_x"].IsNull())
				x_offset = current_frame["offset_x"].GetUint();
			if (current_frame.HasMember("offset_x") && !current_frame["offset_x"].IsNull())
				x_offset = current_frame["offset_x"].GetUint();
			if (current_frame.HasMember("offset_y") && !current_frame["offset_y"].IsNull())
				y_offset = current_frame["offset_y"].GetUint();

			unsigned width = 0, height = 0, duration = 0;
			if (current_frame.HasMember("width") && !current_frame["width"].IsNull())
				width = current_frame["width"].GetUint();
			else if (default_frame_width > 0)
				width = default_frame_width;
			else
			{
				LOG("Default_frame_width is null and width is null!", LOG_TYPE::WAR);
				return nullptr;
			}
			if (current_frame.HasMember("height") && !current_frame["height"].IsNull())
				height = current_frame["height"].GetUint();
			else if (default_frame_height > 0)
				height = default_frame_height;
			else
			{
				LOG("Default_frame_height is null and height is null!", LOG_TYPE::WAR);
				return nullptr;
			}

			RenderEngine::SubTexture2D sub_texture(
				{ static_cast<float>(x_offset) / static_cast<float>(atlas_width),
					static_cast<float>(y_offset) / static_cast<float>(atlas_height) },
				{ static_cast<float>((x_offset + width)) / static_cast<float>(atlas_width),
					static_cast<float>((y_offset + height)) / static_cast<float>(atlas_height) },
				duration);
			newAtlas->addFrame(name, sub_texture);

			x_offset += width;
			if (x_offset == atlas_width)
			{
				x_offset = 0;
				y_offset -= height;
			}
		}
	}
	else
		return nullptr;

	return newAtlas;
}

void ResourceManager::loadShader(const rapidjson::Document::MemberIterator& shaders_iterator)
{
	for (const auto& current_shader : shaders_iterator->value.GetArray())
	{
		std::string name;
		std::string file_path_vertex;
		std::string file_path_fragment;
		if (current_shader.HasMember("name") && !current_shader["name"].IsNull() && current_shader["name"].IsString())
			name = current_shader["name"].GetString();
		if (current_shader.HasMember("vertex_path") && !current_shader["vertex_path"].IsNull() && current_shader["vertex_path"].IsString())
			file_path_vertex = current_shader["vertex_path"].GetString();
		if (current_shader.HasMember("fragment_path") && !current_shader["fragment_path"].IsNull() && current_shader["fragment_path"].IsString())
			file_path_fragment = current_shader["fragment_path"].GetString();
		if(name.empty() || file_path_fragment.empty() || file_path_vertex.empty())
		{
			LOG("Incorrect shader obj in parsed json", LOG_TYPE::WAR);
			continue;
		}
		else
			RES.loadShaders(name, file_path_vertex, file_path_fragment);
	}
}

void ResourceManager::loadTexture2D(const rapidjson::Document::MemberIterator& textur_iterator)
{
	for (const auto& current_texture : textur_iterator->value.GetArray())
	{
		std::string name;
		std::string path;
		if (current_texture.HasMember("name") && !current_texture["name"].IsNull() && current_texture["name"].IsString())
			name = current_texture["name"].GetString();
		if (current_texture.HasMember("path") && !current_texture["path"].IsNull() && current_texture["path"].IsString())
			path = current_texture["path"].GetString();
		else continue;

		if (!name.empty())
			RES.loadTexture(name, path);
		else if (!path.empty())
			RES.loadTexture(path, path);
		else
			LOG("Incorrect texture2D obj in parsed json", LOG_TYPE::WAR);
	}
}
//! need rewrite
void ResourceManager::loadAtlas(const rapidjson::Document::MemberIterator& atlas_iterator)
{
	for (const auto& current_atlas : atlas_iterator->value.GetArray())
	{
		std::string name;
		std::string path;
		if (current_atlas.HasMember("name") && !current_atlas["name"].IsNull())
			name = current_atlas["name"].GetString();
		if (current_atlas.HasMember("path") && !current_atlas["path"].IsNull())
			path = current_atlas["path"].GetString();
		else 
			continue;

		if (!name.empty())
			RES.loadAtlas(name, path);
		else if (!path.empty())
			RES.loadAtlas(path, path);
		else
			LOG( "Incorrect atlas obj in parsed json", LOG_TYPE::WAR);
	}
}

void ResourceManager::loadImage(const rapidjson::Document::MemberIterator& image_iter)
{
	for (const auto& current_image : image_iter->value.GetArray())
	{
		std::string name;
		std::string path;
		if(current_image.HasMember("name") && !current_image["name"].IsNull())
			name = current_image["name"].GetString();
		if (current_image.HasMember("path") && !current_image["path"].IsNull())
			path = current_image["path"].GetString();
		if (!name.empty() && !path.empty())
			RES.loadImage(name, path, path);
		else if (!path.empty())
			RES.getSharedImage(path);
		else
			LOG("Incorrect image obj in parsed json", LOG_TYPE::WAR);
	}
}

void ResourceManager::loadSprite(const rapidjson::Document::MemberIterator& sprite_iter)
{
	for (const auto& current_sprite : sprite_iter->value.GetArray())
	{
		std::string name;
		std::string path;
		if (current_sprite.HasMember("name") && !current_sprite["name"].IsNull())
			name = current_sprite["name"].GetString();
		if (current_sprite.HasMember("path") && !current_sprite["path"].IsNull())
			path = current_sprite["path"].GetString();
		if (!name.empty() && !path.empty())
			RES.loadSprite(name, path);
		else if (!path.empty())
			RES.loadSprite(path, path);
		else
			LOG( "Incorrect sprite obj in parsed json", LOG_TYPE::WAR);
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

void ResourceManager::parseAtalsJSON(const std::string& path)
{
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
		LOG("No vertex shader!", LOG_TYPE::WAR);
		return nullptr;
	}
	std::string fragmentShader = getFileString(fragmentPath);
	if (fragmentShader.empty())
	{
		LOG("No vertex shader!", LOG_TYPE::WAR);
		return nullptr;
	}

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexShader, fragmentShader)).first->second;
	if (newShader->isCompiled())
		return newShader;

	LOG(std::string("Can't load shader program: ") +
		 " Vertex: " + vertexPath +
		 " Fragment: " + fragmentPath , LOG_TYPE::WAR);
	return nullptr;
	
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShader(const std::string& shaderName)
{
	shaderProgramMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}
	LOG("Can't find the shader program: " + shaderName, LOG_TYPE::WAR);
	return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	///Загрузка пикселей в массив байт из файла текстуры
	const auto& pixels = FILES.getPixelFile(texturePath);
	if (!pixels->isLoaded() && !pixels->getData())
	{
		LOG("Can't load image: " + texturePath, LOG_TYPE::WAR);
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
	LOG("Can't find the texture: " + textureName, LOG_TYPE::WAR);
	return nullptr;
}

std::shared_ptr<RenderEngine::Image2D> ResourceManager::loadImage(const std::string& imageName, const std::string& textureName, const std::string& texturePath)
{
	auto texture = loadTexture(textureName, texturePath);
	if(texture)
	{
		const auto image = m_images.emplace(imageName, std::make_shared<RenderEngine::Image2D>(texture));
		if(image.second)
			return image.first->second;
	}
	LOG("Can't load the image: " + imageName, LOG_TYPE::WAR);
	return nullptr;
}

std::shared_ptr<RenderEngine::Image2D> ResourceManager::getImage(const std::string& imageName)
{
	auto it = m_images.find(imageName);
	if (it != m_images.end())
		return it->second;
	LOG("Can't find the image: " + imageName, LOG_TYPE::WAR);
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
	LOG("Can't find the image: " + path, LOG_TYPE::WAR);
	return false;
}

std::pair<RenderEngine::eAtlasType, std::shared_ptr<RenderEngine::Texture2D>> ResourceManager::getAtlas(const std::string& textureName)
{
	const auto sprite_it = m_SpriteAtlases.find(textureName);
	if (sprite_it != m_SpriteAtlases.end())
		return { RenderEngine::eAtlasType::eSprite, sprite_it->second };
	const auto frame_it = m_FrameAtlases.find(textureName);
	if (frame_it != m_FrameAtlases.end())
		return { RenderEngine::eAtlasType::eFrames, frame_it->second };
	LOG("Can't find the atlas: " + textureName, LOG_TYPE::WAR);
	return { RenderEngine::eAtlasType::eNone, nullptr };
}

std::shared_ptr<RenderEngine::FrameAtlas> ResourceManager::getFrameAtlas(const std::string& path)
{
	auto frame_it = m_FrameAtlases.find(path);
	if (frame_it != m_FrameAtlases.end())
		return frame_it->second ;
	else
	{
		const auto result = loadAtlas(path, path);
		if (result.first == RenderEngine::eAtlasType::eFrames)
		{
			frame_it = m_FrameAtlases.find(path);
			if (frame_it != m_FrameAtlases.end())
				return frame_it->second;
		}
	}
	LOG("Can't find the atlas: " + path, LOG_TYPE::WAR);
	return nullptr;
}

std::shared_ptr<RenderEngine::SpriteAtlas> ResourceManager::getSpriteAtlas(const std::string& path)
{
	auto sprite_it = m_SpriteAtlases.find(path);
	if (sprite_it != m_SpriteAtlases.end())
		return sprite_it->second;
	else
	{
		const auto result = loadAtlas(path, path);
		if (result.first == RenderEngine::eAtlasType::eSprite)
		{
			sprite_it = m_SpriteAtlases.find(path);
			if (sprite_it != m_SpriteAtlases.end())
				return sprite_it->second;
		}
	}
	LOG("Can't find the atlas: " + path, LOG_TYPE::WAR);
	return nullptr;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const std::string& path)
{
	const auto& it = m_sprites.find(path);
	if (it == m_sprites.end())
	{
		return loadSprite(path, path);
	}
	return it->second;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::loadSprite(const std::string& name, const std::string& path)
{
	const auto& json_doc = getFileString(path + ".sprite");
	if (json_doc.empty())
	{
		LOG("Failed to parse json: " + path, LOG_TYPE::WAR);
		return nullptr;
	}
	rapidjson::Document document;
	rapidjson::ParseResult parse_result = document.Parse(json_doc.c_str());
	if (!parse_result)
	{
		LOG(std::string("Json parse error: ") + std::string(rapidjson::GetParseError_En(parse_result.Code())) + std::string(" (")
			+ std::string(parse_result.Offset() + ") in file: ") + path, LOG_TYPE::WAR);
		return nullptr;
	}
	const auto& atlas_iterator = document.FindMember("atlases");
	if (atlas_iterator != document.MemberEnd() && document["atlases"].IsArray())
	{
		std::list<RenderEngine::SpriteAnimation> animList;
		for (const auto& current_atlas : atlas_iterator->value.GetArray())
		{
			std::string atlas_path;
			if (current_atlas.HasMember("path") && !current_atlas["path"].IsNull() && current_atlas["path"].IsString())
				atlas_path = current_atlas["path"].GetString();
			auto atlas = getSpriteAtlas(atlas_path);
			if (!atlas) 
			{
				loadAtlas(atlas_path, atlas_path);
				atlas = getSpriteAtlas(atlas_path);
			}
			if (atlas)
				animList.emplace_back(RenderEngine::SpriteAnimation{ atlas });
		}
		if(!animList.empty())
		{
			auto elem = m_sprites.emplace(name, std::make_shared<RenderEngine::Sprite>( animList ));
			if (elem.second)
				return elem.first->second;
			else return nullptr;
		}
	}
	return nullptr;
}

std::pair<RenderEngine::eAtlasType, std::shared_ptr<RenderEngine::Texture2D>> ResourceManager::loadAtlas(const std::string& name, const std::string& path)
{
	///Загрузка пикселей в массив байт из файла текстуры
	const auto& pixels = FILES.getPixelFile(path + ".png");
	if (!pixels->isLoaded() && !pixels->getData())
	{
		LOG("Can't load atlas: " + path, LOG_TYPE::WAR);
		return{ RenderEngine::eAtlasType::eNone, nullptr };
	}	///Загрузка пикселей в массив байт из файла текстуры

	const auto& json_doc = getFileString(path + ".json");
	if (json_doc.empty())
	{
		LOG("Failed to parse json: " + path, LOG_TYPE::WAR);
		return { RenderEngine::eAtlasType::eNone, nullptr };
	}
	rapidjson::Document document;
	rapidjson::ParseResult parse_result = document.Parse(json_doc.c_str());
	if (!parse_result)
	{
		LOG("Json parse error: " + std::string(rapidjson::GetParseError_En(parse_result.Code())) + " ("
			+ std::string(parse_result.Offset() + ") in file: ") + path, LOG_TYPE::WAR);
		return { RenderEngine::eAtlasType::eNone, nullptr };
	}
	
	int type = -1;
	const auto& size_iterator = document.FindMember("info");
	if (size_iterator != document.MemberEnd() && document["info"].IsObject())
	{
			if (document["info"].HasMember("type") && !document["info"]["type"].IsNull())
				type = document["info"]["type"].GetUint();
			else 
			{
				LOG("Json parse error: " + std::string(rapidjson::GetParseError_En(parse_result.Code())) + " ("
					+ std::string(parse_result.Offset() + ") in file: ") + path + ": Failed to parse {info{type...}}", LOG_TYPE::WAR);
				return { RenderEngine::eAtlasType::eNone, nullptr };
			}
	}
	else
	{
		LOG("Json parse error: " + std::string(rapidjson::GetParseError_En(parse_result.Code())) + " ("
			+ std::string(parse_result.Offset() + ") in file: ") + path + ": Failed to parse member {info}, member is empty", LOG_TYPE::WAR);
		return { RenderEngine::eAtlasType::eNone, nullptr };
	}
	//! Parse Atlases by type
	if (static_cast<RenderEngine::eAtlasType>(type) == RenderEngine::eAtlasType::eSprite)
	{
		std::shared_ptr<RenderEngine::SpriteAtlas> newAtlas = std::make_shared<RenderEngine::SpriteAtlas>
			(pixels->getWidth(), pixels->getHeight(), pixels->getData(), pixels->getChanels(), GL_NEAREST, GL_CLAMP_TO_EDGE);
		newAtlas = std::move(loadSpriteAtlas(std::move(newAtlas), document));
		if (newAtlas) {
			auto emp = m_SpriteAtlases.emplace(
				name, newAtlas);
			if (emp.second)
				return { RenderEngine::eAtlasType::eSprite, newAtlas };
		}
		return { RenderEngine::eAtlasType::eNone, nullptr };
	}
	if(static_cast<RenderEngine::eAtlasType>(type) == RenderEngine::eAtlasType::eFrames)
	{
		std::shared_ptr<RenderEngine::FrameAtlas> newAtlas = std::make_shared<RenderEngine::FrameAtlas>
			(pixels->getWidth(), pixels->getHeight(), pixels->getData(), pixels->getChanels(), GL_NEAREST, GL_CLAMP_TO_EDGE);
		newAtlas = loadFrameAtlas(newAtlas, document);
		if (newAtlas) {
			auto emp = m_FrameAtlases.emplace(
				name, newAtlas);
			if (emp.second)
				return { RenderEngine::eAtlasType::eFrames, newAtlas };
		}
		return { RenderEngine::eAtlasType::eNone, nullptr };
	}
	LOG( "Json parse error: " + std::string(rapidjson::GetParseError_En(parse_result.Code())) + " ("
		+ std::string(parse_result.Offset() + ") in file: ") + path + ": Failed to parse {info{type...}}", LOG_TYPE::WAR);
	return { RenderEngine::eAtlasType::eNone, nullptr };
}

bool ResourceManager::loadResJSON(const std::string& path)
{
	const auto& json_doc = getFileString(path);
	if (json_doc.empty())
	{
		LOG( "Failed to parse json: " + path, LOG_TYPE::WAR);
		return false;
	}
	rapidjson::Document document;
	rapidjson::ParseResult parse_result = document.Parse(json_doc.c_str());
	if (!parse_result)
	{
		LOG("Json parse error: " + std::string(rapidjson::GetParseError_En(parse_result.Code())) + " ("
		+ std::string(parse_result .Offset() + ") in file: ") + path, LOG_TYPE::WAR);
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

	const auto& sprites_iterator = document.FindMember("sprites");
	if (sprites_iterator != document.MemberEnd())
		loadSprite(sprites_iterator);

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
