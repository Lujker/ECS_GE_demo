#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <map>


#define FILES FileSystem::Instanse()
class FileSystem
{
public:

	struct File
	{
		File(const std::string& path = "");
		~File();
		const std::string& getData();
		bool isLoaded();
		void load(const std::string& path = "");
		void unload();
	private:
		std::string m_buffer;
		std::string path;
		bool isLoad;
	};

	struct Pixels
	{
		Pixels(const std::string& path = "");
		~Pixels();
		const unsigned char* load(const std::string& path = "");
		void unload();

		bool isLoaded() { return isLoad; }
		const unsigned char* getData() const { return pixels; }
		const int getWidth() const { return width; }
		const int getHeight() const { return height; }
		const int getChanels() const { return chanel; }
	private:
		unsigned char* pixels = nullptr;
		int chanel = 0, width = 0, height = 0;
		std::string path;
		bool isLoad = false;
	};

	static FileSystem& Instanse();
	void setExecutablePath(const std::string& execPath);
	void unloadAllFiles();
	std::string getFormatedPath(const std::string& path);
	std::shared_ptr<File> loadFile(std::string path = "");
	std::shared_ptr<File> getFile(std::string path = "");
	std::shared_ptr<Pixels> loadPixelFile(std::string path = "");
	std::shared_ptr<Pixels> getPixelFile(std::string path = "");
	typedef std::map<const std::string, std::shared_ptr<File>> filesMap;
	typedef std::map<const std::string, std::shared_ptr<Pixels>> pixelsFileMap;
private:
	filesMap m_files;
	pixelsFileMap m_pixels;
	std::string m_path;
};