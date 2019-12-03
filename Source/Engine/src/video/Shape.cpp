/*
 *	Shape.cpp
 *	Primitive Shapes
 */

#include <Video/Shape.hpp>
#include <Video/Video.hpp>

namespace Video
{
	// ==== Shape ====

	Shape::Shape()
		: vertexBuffer(),
		  dirty(false),
		  position(),
		  color(Color::White)
	{
		
	}

	Shape::~Shape()
	{

	}

	void Shape::Update()
	{
		size_t count = GetPointCount();

		for(size_t i = 0; i < count; ++i)
			vertexBuffer.Add(Vertex(position + GetPoint(i), color));

		dirty = false;
	}

	void Shape::SetPosition(const Vector2f& value)
	{
		position = value;
		dirty = true;
	}

	void Shape::SetColor(const Color& value)
	{
		color = value;
		dirty = true;
	}

	const Vector2f& Shape::GetPosition() const
	{
		return position;
	}

	const Color& Shape::GetColor() const
	{
		return color;
	}

	void Shape::Draw()
	{
		if (dirty)
			Update();

		glBufferData(GL_ARRAY_BUFFER, vertexBuffer.GetSize(), vertexBuffer.GetBuffer(), GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertexBuffer.Count());
	}

	// ==== Shape Rectangle ===

	ShapeRectangle::ShapeRectangle()
		: Shape(),
		  size()
	{
		
	}

	Vector2f ShapeRectangle::GetPoint(size_t i)
	{
		switch(i)
		{
			case 0:
				return Vector2f(0.0f, 0.0f);

			case 1:
				return Vector2f(size.x, 0.0f);

			case 2:
				return size;

			case 3:
				return Vector2f(0.0f, size.y);
		}

		return Vector2f();
	}

	size_t ShapeRectangle::GetPointCount() const
	{
		return 4;
	}

	void ShapeRectangle::SetSize(const Vector2f& value)
	{
		size = value;
		dirty = true;
	}

	const Vector2f& ShapeRectangle::GetSize() const
	{
		return size;
	}

	// ==== Shape Circle ====

	ShapeCircle::ShapeCircle()
		: Shape(),
		  radius(0.0f),
		  pointCount(POINT_COUNT)
	{
		
	}

	Vector2f ShapeCircle::GetPoint(size_t i)
	{
		static const float PI = 3.141592654f;

		float angle = i * 2 * PI / pointCount - PI / 2;
		float x = std::cos(angle) * radius;
		float y = std::sin(angle) * radius;

		return Vector2f(std::cos(angle) * radius,
						std::sin(angle) * radius);
	}

	size_t ShapeCircle::GetPointCount() const
	{
		return pointCount;
	}

	void ShapeCircle::SetPointCount(size_t value)
	{
		pointCount = value >= 3 ? value : 3;
		dirty = true;
	}

	void ShapeCircle::SetRadius(float value)
	{
		radius = value;
		dirty = true;
	}

	float ShapeCircle::GetRadius() const
	{
		return radius;
	}

	// ==== Shape Convex ====
	ShapeConvex::ShapeConvex()
		: Shape(),
		  points()
	{

	}

	Vector2f ShapeConvex::GetPoint(size_t i)
	{
		return points[i];
	}

	size_t ShapeConvex::GetPointCount() const
	{
		return points.size();
	}

	void ShapeConvex::SetPoint(size_t i, const Vector2f& value)
	{
		if (i >= points.size())
			points.resize(i + 1);

		points[i] = value;
	}
}
