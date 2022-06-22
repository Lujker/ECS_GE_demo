#include "FileSystemManager.h"

#include "stb_image.h"

FileSystem::filesMap FileSystem::m_files;
FileSystem::pixelsFileMap FileSystem::m_pixels;
std::string FileSystem::m_path;

FileSystem& FileSystem::Instanse()
{
	static FileSystem fileSystem;
	return fileSystem;
}

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
	for (auto &it : m_pixels)
	{
		it.second->unload();
	}
	m_pixels.clear();
}

std::string FileSystem::getFormatedPath(const std::string& path)
{
	return m_path + "/" + path;
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

std::shared_ptr<FileSystem::Pixels> FileSystem::loadPixelFile(std::string path)
{
	if (!path.empty())
	{
		const auto& f = m_pixels.emplace(getFormatedPath(path), std::make_shared<Pixels>(getFormatedPath(path)));
		if (f.second)
			return f.first->second;
	}
	return nullptr;
}

std::shared_ptr<FileSystem::Pixels> FileSystem::getPixelFile(std::string path)
{
	if (path.empty())
		return nullptr;
	const auto& f = m_pixels.find(getFormatedPath(path));
	if (f == m_pixels.end())
		return loadPixelFile(path);
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
			std::cerr << "Failed to open file: " << this->path << std::endl;
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
		std::cerr << "Failed catch bad_alloc: " << exep.what() << std::endl;
	}
	catch (const std::exception& exep)
	{
		std::cerr << "Failed catch exeption: " << exep.what() << std::endl;
	}
}

void FileSystem::File::unload()
{
	m_buffer.clear();
	isLoad = false;
}

FileSystem::Pixels::Pixels(const std::string& path)
{
	load(path);
}

FileSystem::Pixels::~Pixels()
{
	unload();
}

const unsigned char* FileSystem::Pixels::load(const std::string& path)
{
	try
	{
		this->path = path;
		unload();
		stbi_set_flip_vertically_on_load(true); ///Загружать текстуры в порядке OpenGL
		auto pix = stbi_load(this->path.c_str(), &width, &height, &chanel, 0);
		if (!pix)
		{
			std::cerr << "Can't load image: " << this->path << std::endl;
			return nullptr;
		}
		isLoad = true;
		pixels = pix;
		return pixels;
	}
	catch (const std::bad_alloc& exep)
	{
		std::cerr << "Failed catch bad_alloc: " << exep.what() << std::endl;
	}
	catch (const std::exception& exep)
	{
		std::cerr << "Failed catch exeption: " << exep.what() << std::endl;
	}
	return nullptr;
}

void FileSystem::Pixels::unload()
{
	if (pixels && isLoad)
		stbi_image_free(pixels);
	isLoad = false;
}
