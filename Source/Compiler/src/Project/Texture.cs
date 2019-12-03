using System.Drawing;

namespace Swift.Project
{
	internal class Texture
	{
		public string Filename = "";
		public bool Animated = false;
		public Vector2f Origin = new Vector2f();
		public bool Smooth = false;
		public Rectangle BBox = new Rectangle(0, 0, 1, 1);
		public string CMode = CollisionMode.None;
		public bool Lazy = false;
		
		public static class CollisionMode
		{
			public const string None = "none";
			public const string Precise = "precise";
			public const string BoundingBox = "bbox";
		}
	}
}
