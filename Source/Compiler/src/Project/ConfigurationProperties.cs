namespace Swift.Project
{
	internal class ConfigurationProperties
	{
		public string Title = "Untitled Game";
		public string Description = "";
		public string Company = "";
		public string Product = "";
		public string Copyright = "";
		public string Trademark = "";
		public string Version = "1.0.0";
		public string InitialState = "";
		public bool IncBuild = true;
		public int SheetSize = 2048;
		public byte TargetFramerate = 60;
		public Vector2s Size = new Vector2s(800, 600);
		public bool Fullscreen = false;
		public bool Vsync = true;
		public bool Cursor = true;
		public byte AntiAliasingLevel = 0;
		public string Icon = "";
	}
}
