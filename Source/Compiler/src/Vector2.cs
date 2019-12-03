namespace Swift
{
	internal class Vector2<T>
	{
		// NOTE THESE FIELDS CANNOT BE MADE READONLY, RESHARPER DOESN'T KNOW SHIT
		public T X;
		public T Y;

		protected Vector2(T x, T y)
		{
			X = x;
			Y = y;
		}
	}

	internal class Vector2s : Vector2<short>
	{
		public Vector2s()
			: base(0, 0)
		{
			
		}

		public Vector2s(short x, short y)
			: base(x, y)
		{
			
		}
	}

	internal class Vector2i : Vector2<int>
	{
		public Vector2i()
			: base(0, 0)
		{
			
		}

		public Vector2i(int x, int y)
			: base(x, y)
		{
			
		}
	}

	internal class Vector2f : Vector2<float>
	{
		public Vector2f()
			: base(0, 0)
		{
			
		}

		public Vector2f(float x, float y)
			: base(x, y)
		{
			
		}
	}
}
