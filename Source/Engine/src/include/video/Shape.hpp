/*
 *	Shape.hpp
 *	Primitive Shapes
 */

#ifndef __VIDEO_PRIMITIVE_SHAPE_HPP
#define __VIDEO_PRIMITIVE_SHAPE_HPP

#include <Video/VertexBuffer.hpp>

namespace Video
{
	class Shape
	{
	private:
		VertexBuffer vertexBuffer;
		void Update();

	protected:
		bool dirty;

		Vector2f position;
		Color color;

		virtual Vector2f GetPoint(size_t i) = 0;
		virtual size_t GetPointCount() const = 0;

	public:
		Shape();
		virtual ~Shape();

		void SetPosition(const Vector2f& value);
		void SetColor(const Color& value);

		const Color& GetColor() const;
		const Vector2f& GetPosition() const;

		void Draw();
	};

	class ShapeRectangle : public Shape
	{
	private:
		Vector2f size;

	protected:
		Vector2f GetPoint(size_t i);
		size_t GetPointCount() const;

	public:
		ShapeRectangle();

		void SetSize(const Vector2f& value);
		const Vector2f& GetSize() const;
	};

	class ShapeCircle : public Shape
	{
	private:
		static const size_t POINT_COUNT = 24;

	private:
		float radius;
		size_t pointCount;

	protected:
		Vector2f GetPoint(size_t i);
		size_t GetPointCount() const;

	public:
		ShapeCircle();

		void SetPointCount(size_t value);
		void SetRadius(float value);

		float GetRadius() const;
	};

	class ShapeConvex : public Shape
	{
	private:
		std::vector<Vector2f> points;

	protected:
		Vector2f GetPoint(size_t i);
		size_t GetPointCount() const;

	public:
		ShapeConvex();

		void SetPoint(size_t i, const Vector2f& value);
	};
}

#endif
