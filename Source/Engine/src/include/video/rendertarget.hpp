/*
 *	rendertarget.hpp
 *	Render Target
 */

#ifndef __VIDEO_RENDER_TARGET_HPP
#define __VIDEO_RENDER_TARGET_HPP

#include <Video/Video.hpp>
#include <Video/Shader.hpp>
#include <Video/Color.hpp>
#include <Util/Vector2.hpp>

namespace Video
{
	class RenderTarget
	{
	private:
		GLuint vao, vbo;
		glm::mat4x4 projectionMatrix;
		Shader* shader;

	protected:
		void InitializeTarget();

	public:
		RenderTarget();
		virtual ~RenderTarget();

		void Clear(const Color& color);

		void SetView(const Vector2f& size);
		void UpdateView();
	};
}

#endif
