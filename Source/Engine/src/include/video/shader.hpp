/*
 *	shader.hpp
 *	Shader
 */

#ifndef __VIDEO_SHADER_HPP
#define __VIDEO_SHADER_HPP

#include <exception>
#include <string>
#include <Video/Video.hpp>
#include <glm/glm.hpp>

#define GLSL_VEC2_SIZE			2
#define GLSL_VEC4_SIZE			4

namespace Video
{
	class Shader
	{
	public:
		// TODO enum class
		typedef enum _SourceType
		{
			StVertex,
			StFragment,
			StGeometry
		} SourceType;

	private:
		GLuint vertexShader, fragmentShader;
		GLuint program;
		size_t attribOffset;

		std::string ReadShaderLog(GLuint shaderIndex) const;

	public:
		Shader();
		~Shader();

		void CompileString(const std::string& source, SourceType type);
		void Link();
		void Use();

		void BindFragmentData(GLuint location, const std::string& name);
		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, const glm::mat4x4& value);

		void DescribeVertexAttribute(const std::string& name, size_t size);
	};

	class ShaderException : public std::exception
	{
	private:
		std::string message;

	public:
		ShaderException(const std::string& _message);

		virtual const char* what() const throw();
	};
}

#endif
