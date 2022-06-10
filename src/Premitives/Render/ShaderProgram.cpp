#include "ShaderProgram.h"

RenderEngine::ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint vertexShaderID;
	if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
	{
		std::cerr<<"VERTEX SHADER compile-time error"<<std::endl;
		return;
	}

	GLuint fragmentShaderID;
	if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
	{
		std::cerr << "FRAGMENT SHADER compile-time error" << std::endl;
		glDeleteShader(fragmentShaderID);
		return;
	}

	m_ID = glCreateProgram();

	glAttachShader(m_ID, vertexShaderID);
	glAttachShader(m_ID, fragmentShaderID);
	glLinkProgram(m_ID);

	GLint success;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(m_ID, 1024, nullptr, infoLog);
		std::cerr << "ERROR::SHADER: Link-time error:\n" << infoLog << std::endl;
	}
	else
	{
		m_isCompiled = true;
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

RenderEngine::ShaderProgram& RenderEngine::ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
{
	glDeleteProgram(m_ID);
	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;

	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;
	return *this;
}

RenderEngine::ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept :
	m_isCompiled(shaderProgram.m_isCompiled), m_ID(shaderProgram.m_ID)
{
	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;
}

RenderEngine::ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ID);
}

bool RenderEngine::ShaderProgram::isCompiled() const
{
	return m_isCompiled;
}

void RenderEngine::ShaderProgram::use() const
{
	glUseProgram(m_ID);
}

//! указывает для uniform фрагментного шейдера под именем (name) значение id слота в котором загруженна текстура в движке(value) (см. конструктор texture2D) 
void RenderEngine::ShaderProgram::setInt(const std::string& name, const GLint value)
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void RenderEngine::ShaderProgram::setFloat(const std::string& name, const GLfloat value)
{
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void RenderEngine::ShaderProgram::setMatrix4(const std::string& name, const glm::mat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

bool RenderEngine::ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
{
	shaderID = glCreateShader(shaderType);
	const char* code = source.c_str();
	glShaderSource(shaderID, 1, &code, nullptr);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
		std::cerr << "ERROR::SHADER: Compile-time error:\n" << infoLog << std::endl;
		return false;
	}
	return true;
}
