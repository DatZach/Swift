/*
 *	shader.cpp
 *	Shader
 */

#include <sstream>
#include <string>
#include <Video/Shader.hpp>
#include <Video/Vertex.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Video
{
	Shader::Shader()
		: vertexShader(0),
		  fragmentShader(0),
		  program(0),
		  attribOffset(0)
	{
		program = glCreateProgram();

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	}

	Shader::~Shader()
	{
		glDeleteProgram(program);
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}

	std::string Shader::ReadShaderLog(GLuint shaderIndex) const
	{
		GLint length;
		glGetShaderiv(shaderIndex, GL_INFO_LOG_LENGTH, &length);

		if (length > 0)
		{
			std::string log(length, 0);
			glGetShaderInfoLog(shaderIndex, length, &length, &log[0]);

			return log;
		}

		return "";
	}

	void Shader::CompileString(const std::string& source, SourceType type)
	{
		// C++ is fucking stupid
		const char* shaderSource = source.c_str();

		// Get shader index
		GLuint shaderIndex = (type == StVertex) ? vertexShader : fragmentShader;
		GLint compileStatus;

		// Compile shader
		glShaderSource(shaderIndex, 1, &shaderSource, nullptr);
		glCompileShader(shaderIndex);

		// Make sure we compiled correctly
		glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus != GL_TRUE)
		{
			std::stringstream ss;
			ss << "Compile error (Type "
			   << type
			   << ", 0x" << std::hex << this
			   << ")\n"
			   << "\t\t" << ReadShaderLog(shaderIndex);

			throw ShaderException(ss.str());
		}

		// Attach the shader
		glAttachShader(program, shaderIndex);

		GlCheckError();
	}

	void Shader::Link()
	{
		glLinkProgram(program);
	}

	void Shader::Use()
	{
		glUseProgram(program);
	}

	void Shader::BindFragmentData(GLuint location, const std::string& name)
	{
		glBindFragDataLocation(program, location, name.c_str());

		GlCheckError();
	}

	void Shader::SetUniform(const std::string& name, int value)
	{
		GLuint location = glGetUniformLocation(program, name.c_str());
		glUniform1i(location, value);

		GlCheckError();
	}

	void Shader::SetUniform(const std::string& name, const glm::mat4x4& value)
	{
		GLuint location = glGetUniformLocation(program, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

		GlCheckError();
	}

	void Shader::DescribeVertexAttribute(const std::string& name, size_t size)
	{
		GLint attributeIndex = glGetAttribLocation(program, name.c_str());
		glEnableVertexAttribArray(attributeIndex);

		glVertexAttribPointer(attributeIndex, size, GL_FLOAT, GL_FALSE, Vertex::VertexSize,
							  reinterpret_cast<void*>(attribOffset * sizeof(float)));
		attribOffset += size;

		GlCheckError();
	}

	ShaderException::ShaderException(const std::string& _message)
		: message(_message)
	{
		GlCheckError();
	}

	const char* ShaderException::what() const throw()
	{
		return message.c_str();
	}
}
