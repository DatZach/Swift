/*
 *	rendertarget.cpp
 *	Render Target
 */

#include <Video/RenderTarget.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Util/Logger.hpp>

#define GLSL(src)			"#version 150 core\n" #src

namespace Video
{
	RenderTarget::RenderTarget()
		: vao(0),
		  vbo(0),
		  projectionMatrix(),
		  shader(nullptr)
	{

	}

	RenderTarget::~RenderTarget()
	{
		delete shader;

		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	void RenderTarget::InitializeTarget()
	{
		// Create the VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Create the VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Prepare default shader
		shader = new Shader();

		try
		{
			// Default Vertex Shader
			shader->CompileString(
				GLSL(
					in vec2 position;
					in lowp vec4 color;
					in vec2 texCoord;

					out lowp vec4 Color;
					out vec2 TexCoord;

					uniform mat4 projectionMatrix;

					void main()
					{
						Color = color;
						TexCoord = texCoord;
						gl_Position = projectionMatrix * vec4(position, 0.0, 1.0);
					}
				),
				Shader::StVertex);

			// Default Fragment Shader
			shader->CompileString(
				GLSL(
					in lowp vec4 Color;
					in vec2 TexCoord;

					out lowp vec4 outColor;

					uniform sampler2D tex;

					void main()
					{
						outColor = TexCoord.x < 0 ? Color : texture(tex, TexCoord);
					}
				),
				Shader::StFragment
			);
		}
		catch(ShaderException& e)
		{
			Error << "RenderTarget::Initialize() Shader Exception:" << lendl
				  << ltab << e.what() << lendl;
		}

		shader->BindFragmentData(0, "outColor");

		// Link & use shader
		shader->Link();
		shader->Use();

		// Describe the vertex shader format
		shader->DescribeVertexAttribute("position", GLSL_VEC2_SIZE);
		shader->DescribeVertexAttribute("color", GLSL_VEC4_SIZE);
		shader->DescribeVertexAttribute("texCoord", GLSL_VEC2_SIZE);

		shader->SetUniform("tex", 0);

		// Enable alpha channel
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void RenderTarget::Clear(const Color& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderTarget::SetView(const Vector2f& size)
	{
		projectionMatrix = glm::ortho(0.0f, size.x, size.y, 0.0f);
		glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
	}

	void RenderTarget::UpdateView()
	{
		shader->SetUniform("projectionMatrix", projectionMatrix);
	}
}
