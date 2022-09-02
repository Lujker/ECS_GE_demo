#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <map>

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

	friend class ServiceLocator;
	void setExecutablePath(const std::string& execPath);
	void unloadAllFiles();
	std::string getFormatedPath(const std::string& path);
	std::shared_ptr<File> loadFile(std::string path = "");
	std::shared_ptr<File> getFile(std::string path = "");
	typedef std::map<const std::string, std::shared_ptr<File>> filesMap;
	FileSystem(const FileSystem&) = delete;
	FileSystem(FileSystem&&) = delete;
	FileSystem& operator=(const FileSystem&) = delete;
	FileSystem& operator=(FileSystem&&) = delete;
private:
	FileSystem() = default;
	~FileSystem() = default;
	filesMap m_files;
	std::string m_path;
};