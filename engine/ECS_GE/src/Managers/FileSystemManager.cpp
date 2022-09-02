#include "FileSystemManager.h"
#include "Engine.h"
#include "LogSystem.h"
#include "stb_image.h"
#include <memory>
#include "Magick++.h"

void FileSystem::setExecutablePath(const std::string& execPath)
{
	size_t found = execPath.find_last_of("/\\");
	m_path = execPath.substr(0, found);
}

void FileSystem::unloadAllFiles()
{
	for (auto &it : m_files)
	{
		it.second->unload();
	}
	m_files.clear();
}

std::string FileSystem::getFormatedPath(const std::string& path)
{
	return m_path + "\\" + path;
}

std::shared_ptr<FileSystem::File> FileSystem::loadFile(std::string path)
{
	if (!path.empty())
	{
		const auto& f = m_files.emplace(getFormatedPath(path), std::make_shared<File>(getFormatedPath(path)));
		if (f.second)
			return f.first->second;
	}
	return nullptr;
}

std::shared_ptr<FileSystem::File> FileSystem::getFile(std::string path)
{
	if (path.empty())
		return nullptr;
	const auto& f = m_files.find(getFormatedPath(path));
	if (f == m_files.end())
		return loadFile(path);
	return f->second;
}

FileSystem::File::File(const std::string& path) :
	path(path), isLoad(false)
{
	if (!path.empty())
		load(path);
}

FileSystem::File::~File()
{
	unload();
}

const std::string& FileSystem::File::getData()
{
	return m_buffer;
}

bool FileSystem::File::isLoaded()
{
	return isLoad;
}

void FileSystem::File::load(const std::string& path)
{
	try
	{
		this->path = path;
		unload();
		std::fstream f;
		f.open(this->path.c_str(), std::ios::in | std::ios::binary);
		if (!f.is_open())
		{
			LOG("Failed to open file: " + this->path, LOG_TYPE::WAR);
			return;
		}

		std::stringstream buffer;
		buffer << f.rdbuf();
		f.close();
		m_buffer = buffer.str();
		if (!m_buffer.empty())
			isLoad = true;
	}
	catch (const std::bad_alloc& exep)
	{
		LOG(std::string("Failed catch bad_alloc: ") + exep.what(), LOG_TYPE::WAR);
	}
	catch (const std::exception& exep)
	{
		LOG(std::string("Failed catch exeption: ") + exep.what(), LOG_TYPE::WAR);
	}
}

void FileSystem::File::unload()
{
	m_buffer.clear();
	isLoad = false;
}
