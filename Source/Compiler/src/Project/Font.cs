namespace Swift.Project
{
	internal class Font
	{
		public string FontFace = "@Arial";
		public bool Aa = true;
		public int Size = 10;
		public bool Bold = false;
		public bool Italic = false;
		public bool Underline = false;
		public bool Overline = false;
		public bool StrikeThrough = false;
		public OutlineProperties Outline = new OutlineProperties();
		public DropShadowProperties DropShadow = new DropShadowProperties();

		public class OutlineProperties
		{
			public uint Color = 0x000000FF;
			public int Thickness = 1;
		}

		public class DropShadowProperties
		{
			public Vector2i Offset = new Vector2i(1, 1);
			public int Spread = 3;
			public uint Color = 0x000000FF;
		}
	}
}
