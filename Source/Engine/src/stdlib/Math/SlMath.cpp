/*
 *	slmath.cpp
 *	Standard Library Math
 */

#include <iostream>
#include <cmath>
#include <StdLib/SlApi.hpp>
#include <Util/Logger.hpp>

namespace StdLib
{
	namespace Math
	{
		/*
			static abs(value);
			static acos(value);
			static asin(value);
			static atan(value);
			static atan2(value);
			static ceil(value);
			static cos(value);
			static cosh(value);
			static exp(value);
			static floor(value);
			static log(value, exp);
			static log10(value);
			static max(x, y);
			static min(x, y);
			static pow(n, exp);
			static round(value);
			static sign(value);
			static sin(value);
			static sinh(value);
			static sqrt(value)
			static tan(value);
			static tanh(value);
		 */
		
		void MathAbs(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			if (StackIsType(0, Vm::Variant::Type::Integer))
			{
				long long value = *stack.Pop();
				StackPush(value < 0 ? -value : value);
				return;
			}
			else if (StackIsType(0, Vm::Variant::Type::Double))
			{
				double value = *stack.Pop();
				StackPush(value < 0.0 ? -value : value);
				return;
			}
			
			Error << "abs(): Expected an integer or double." << lendl;
			StackPush(Vm::Variant());
		}
		
		void MathAcos(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::acos(value));
		}
		
		void MathAsin(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::asin(value));
		}
		
		void MathAtan(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::atan(value));
		}
		
		void MathAtan2(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);
			double x = stack.Pop()->Number();
			double y = stack.Pop()->Number();

			StackPush(std::atan2(x, y));
		}
		
		void MathCeil(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::ceil(value));
		}
		
		void MathCos(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::cos(value));
		}
		
		void MathCosh(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::cosh(value));
		}
		
		void MathExp(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::exp(value));
		}

		void MathFloor(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::floor(value));
		}

		void MathLog(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::log(value));
		}

		void MathLog10(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::log10(value));
		}
		
		void MathMax(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);

			Vm::Variant* x = stack.Pop();
			Vm::Variant* y = stack.Pop();
			stack.Push(*x > *y ? x : y);
		}

		void MathMin(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);

			Vm::Variant* x = stack.Pop();
			Vm::Variant* y = stack.Pop();
			stack.Push(*x < *y ? x : y);
		}

		void MathPow(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);

			if (StackIsType(0, Vm::Variant::Type::Integer))
			{
				long long n = *stack.Pop();
				long long exp = *stack.Pop();

				StackPush(static_cast<long long>(std::pow(n, exp)));
			}
			else if (StackIsType(0, Vm::Variant::Type::Double))
			{
				double n = *stack.Pop();
				double exp = *stack.Pop();

				StackPush(std::pow(n, exp));
			}

			StackPush(0);
		}
		
		void MathRound(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(static_cast<double>(static_cast<long long>(value + 0.5)));
		}

		void MathSign(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);

			if (StackIsType(0, Vm::Variant::Type::Integer))
			{
				long long value = *stack.Pop();
				StackPush(static_cast<long long>(value < 0 ? -1 : 1));
			}
			else if (StackIsType(0, Vm::Variant::Type::Double))
			{
				double value = *stack.Pop();
				StackPush(value < 0.0 ? -1.0 : 1.0);
			}

			StackPush(0);
		}

		void MathSin(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::sin(value));
		}

		void MathSinh(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::sinh(value));
		}

		void MathSqrt(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::sqrt(value));
		}

		void MathTan(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::tan(value));
		}

		void MathTanh(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			double value = stack.Pop()->Number();
			StackPush(std::tanh(value));
		}

		void MathClamp(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);
			ExpectTypeNumber(2);

			double x = stack.Pop()->Number();
			double min = stack.Pop()->Number();
			double max = stack.Pop()->Number();

			StackPush(std::min(std::max(x, min), max));
		}

		void MathLerp(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);
			ExpectTypeNumber(2);

			double a = stack.Pop()->Number();
			double b = stack.Pop()->Number();
			double w = stack.Pop()->Number();

			StackPush(a + w * (b - a));
		}

		void MathRLerp(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);
			ExpectTypeNumber(2);

			double a = stack.Pop()->Number();
			double b = stack.Pop()->Number();
			double c = stack.Pop()->Number();

			StackPush((c - a) / (b - a));
		}

		// Math.PointInRect(px, py, rx ry, width, height);
		void MathPointInRect(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);
			ExpectTypeNumber(2);
			ExpectTypeNumber(3);
			ExpectTypeNumber(4);
			ExpectTypeNumber(5);

			double px = stack.Pop()->Number();
			double py = stack.Pop()->Number();
			double rx = stack.Pop()->Number();
			double ry = stack.Pop()->Number();
			double width = stack.Pop()->Number();
			double height = stack.Pop()->Number();

			StackPush(px >= rx		   && py >= ry		 &&
					  px <= rx + width && py <= ry + height);
		}

		/*!
			class Math

			methods
			Abs				1 static
			Acos			1 static
			Asin			1 static
			Atan			1 static
			Atan2			1 static
			Ceil			1 static
			Cos				1 static
			Cosh			1 static
			Exp				1 static
			Floor			1 static
			Log				1 static
			Log10			1 static
			Max				2 static
			Min				2 static
			Pow				2 static
			Round			1 static
			Sign			1 static
			Sin				1 static
			Sinh			1 static
			Sqrt			1 static
			Tan				1 static
			Tanh			1 static
			Clamp			3 static
			Lerp			3 static
			RLerp			3 static
			PointInRect		6 static

			constants
			Pi				3.1415926535897931
			E				2.7182818284590451
		 */

		void Install()
		{
			Vm::Class classMath;
			
			// Methods
			classMath.AddMethod("Abs", MathAbs, 1);
			classMath.AddMethod("Acos", MathAcos, 1);
			classMath.AddMethod("Asin", MathAsin, 1);
			classMath.AddMethod("Atan", MathAtan, 1);
			classMath.AddMethod("Atan2", MathAtan2, 2);
			classMath.AddMethod("Ceil", MathCeil, 1);
			classMath.AddMethod("Cos", MathCos, 1);
			classMath.AddMethod("Cosh", MathCosh, 1);
			classMath.AddMethod("Exp", MathExp, 1);
			classMath.AddMethod("Floor", MathFloor, 1);
			classMath.AddMethod("Log", MathLog, 1);
			classMath.AddMethod("Log10", MathLog10, 1);
			classMath.AddMethod("Max", MathMax, 2);
			classMath.AddMethod("Min", MathMin, 2);
			classMath.AddMethod("Pow", MathPow, 2);
			classMath.AddMethod("Round", MathRound, 1);
			classMath.AddMethod("Sign", MathSign, 1);
			classMath.AddMethod("Sin", MathSin, 1);
			classMath.AddMethod("Sinh", MathSinh, 1);
			classMath.AddMethod("Sqrt", MathSqrt, 1);
			classMath.AddMethod("Tan", MathTan, 1);
			classMath.AddMethod("Tanh", MathTanh, 1);
			classMath.AddMethod("Clamp", MathClamp, 3);
			classMath.AddMethod("Lerp", MathLerp, 3);
			classMath.AddMethod("RLerp", MathRLerp, 3);

			classMath.AddMethod("PointInRect", MathPointInRect, 6);

			/*
			 *	TODO PointInRect, RectInRect, CircleInRect, TriangleInRect,
			 *		 CircleInCircle, CircleInTriangle, TriangleInTriangle
			 */
			
			VirtualMachine.AddClass("Math", classMath);
		}
	}
}
