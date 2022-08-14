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



class ResourceManager 
{
public:
	friend class ServiceLocator;
	void setExecutablePath(const std::string& execPath);
	void unloadAllResources();
	std::shared_ptr<RenderEngine::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<RenderEngine::ShaderProgram> getShader(const std::string& shaderName);
	std::shared_ptr<RenderEngine::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	std::shared_ptr<RenderEngine::Texture2D> getTexture(const std::string& textureName);
	std::shared_ptr<RenderEngine::Image2D> loadImage(const std::string& imageName, const std::string& textureName, const std::string& texturePath);
	std::shared_ptr<RenderEngine::Image2D> getImage(const std::string& imageName);
	std::shared_ptr<RenderEngine::Image2D> getSharedImage(const std::string& path);
	bool removeSharedImage(const std::string& path);

	std::pair<RenderEngine::eAtlasType, std::shared_ptr<RenderEngine::Texture2D>> loadAtlas(const std::string& name, const std::string& path);
	std::pair<RenderEngine::eAtlasType, std::shared_ptr<RenderEngine::Texture2D>> getAtlas(const std::string& path);
	std::shared_ptr<RenderEngine::FrameAtlas> getFrameAtlas(const std::string& path);
	std::shared_ptr<RenderEngine::SpriteAtlas> getSpriteAtlas(const std::string& path);
	std::shared_ptr<RenderEngine::Sprite> getSprite(const std::string& path);
	std::shared_ptr<RenderEngine::Sprite> loadSprite(const std::string& name, const std::string& path);

	bool loadResJSON(const std::string& path);
	const std::vector<std::vector<std::string>>& getLevels();
	const std::vector<std::string>& atLevelDiscription(const unsigned& index);

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::ShaderProgram>> shaderProgramMap;
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Texture2D>> texturesMap;
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::FrameAtlas>> frameAtlasMap;
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::SpriteAtlas>> spriteAtlasMap;
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Image2D>> imageMap;
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Sprite>> spriteMap;
	typedef std::vector<std::vector<std::string>> levels;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
private:
	std::shared_ptr<RenderEngine::SpriteAtlas> loadSpriteAtlas(std::shared_ptr<RenderEngine::SpriteAtlas> atlas, const rapidjson::Document& document);
	std::shared_ptr<RenderEngine::FrameAtlas> loadFrameAtlas(std::shared_ptr<RenderEngine::FrameAtlas> atlas, const rapidjson::Document& document);
	void loadShader(const rapidjson::Document::MemberIterator&);
	void loadTexture2D(const rapidjson::Document::MemberIterator&);
	void loadAtlas(const rapidjson::Document::MemberIterator&);
	void loadImage(const rapidjson::Document::MemberIterator&);
	void loadSprite(const rapidjson::Document::MemberIterator&);
	void loadLevel(const rapidjson::Document::MemberIterator&);
	void parseAtalsJSON(const std::string& path);

	ResourceManager() = default;
	ResourceManager(const std::string& execPath);
	~ResourceManager();
	std::string getFileString(const std::string& path);

	shaderProgramMap m_shaderPrograms;
	texturesMap m_textures;
	frameAtlasMap m_FrameAtlases;
	spriteAtlasMap m_SpriteAtlases;
	levels m_levels;
	imageMap m_images;
	spriteMap m_sprites;
	std::string m_path;
};