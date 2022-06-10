#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RenderEngine
{
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgram& operator=(ShaderProgram&&) noexcept;
		ShaderProgram(ShaderProgram&&) noexcept;
		~ShaderProgram();
		bool isCompiled() const;
		void use() const;
		void setInt(const std::string& name, const GLint value);
		void setFloat(const std::string& name, const GLfloat value);
		void setMatrix4(const std::string& name, const glm::mat4);

	private:
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		

		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}