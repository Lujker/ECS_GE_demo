#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <map>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

/// <summary>
/// Нужно сделать его синглтоном
/// </summary>
/// 
namespace RenderEngine
{
	enum class eAtlasType;
	class FrameAtlas;
	class ShaderProgram;
	class Texture2D;
	class Image2D;
	class Sprite;
	class SpriteAtlas;
}


#define RES ResourceManager::Instanse()
class ResourceManager 
{
public:
	static ResourceManager& Instanse();
	static void setExecutablePath(const std::string& execPath);
	static void unloadAllResources();
	static std::shared_ptr<RenderEngine::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<RenderEngine::ShaderProgram> getShader(const std::string& shaderName);
	static std::shared_ptr<RenderEngine::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	static std::shared_ptr<RenderEngine::Texture2D> getTexture(const std::string& textureName);
	static std::shared_ptr<RenderEngine::Image2D> loadImage(const std::string& imageName, const std::string& textureName, const std::string& texturePath);
	static std::shared_ptr<RenderEngine::Image2D> getImage(const std::string& imageName);
	static std::shared_ptr<RenderEngine::Image2D> getSharedImage(const std::string& path);
	static bool removeSharedImage(const std::string& path);

	static std::pair<RenderEngine::eAtlasType, std::shared_ptr<RenderEngine::Texture2D>> loadAtlas(const std::string& name, const std::string& path);
	static std::pair<RenderEngine::eAtlasType, std::shared_ptr<RenderEngine::Texture2D>> getAtlas(const std::string& path);
	static std::shared_ptr<RenderEngine::FrameAtlas> getFrameAtlas(const std::string& path);
	static std::shared_ptr<RenderEngine::SpriteAtlas> getSpriteAtlas(const std::string& path);
	static std::shared_ptr<RenderEngine::Sprite> getSprite(const std::string& path);
	static std::shared_ptr<RenderEngine::Sprite> loadSprite(const std::string& name, const std::string& path);

	static bool loadResJSON(const std::string& path);
	static const std::vector<std::vector<std::string>>& getLevels();
	static const std::vector<std::string>& atLevelDiscription(const unsigned& index);

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::ShaderProgram>> shaderProgramMap;
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Texture2D>> texturesMap;
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::FrameAtlas>> frameAtlasMap;
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::SpriteAtlas>> spriteAtlasMap;
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Image2D>> imageMap;
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Sprite>> spriteMap;
	typedef std::vector<std::vector<std::string>> levels;

private:
	static std::shared_ptr<RenderEngine::SpriteAtlas> loadSpriteAtlas(std::shared_ptr<RenderEngine::SpriteAtlas> atlas, const rapidjson::Document& document);
	static std::shared_ptr<RenderEngine::FrameAtlas> loadFrameAtlas(std::shared_ptr<RenderEngine::FrameAtlas> atlas, const rapidjson::Document& document);
	static void loadShader(const rapidjson::Document::MemberIterator&);
	static void loadTexture2D(const rapidjson::Document::MemberIterator&);
	static void loadAtlas(const rapidjson::Document::MemberIterator&);
	static void loadImage(const rapidjson::Document::MemberIterator&);
	static void loadSprite(const rapidjson::Document::MemberIterator&);
	static void loadLevel(const rapidjson::Document::MemberIterator&);
	static void parseAtalsJSON(const std::string& path);

	ResourceManager() = default;
	ResourceManager(const std::string& execPath);
	~ResourceManager();
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	static std::string getFileString(const std::string& path);

	static shaderProgramMap m_shaderPrograms;
	static texturesMap m_textures;
	static frameAtlasMap m_FrameAtlases;
	static spriteAtlasMap m_SpriteAtlases;
	static levels m_levels;
	static imageMap m_images;
	static spriteMap m_sprites;
	static std::string m_path;
};