namespace Swift.Project
{
	internal class Sound
	{
		public string Filename = "";
		public string Mode = SoundStreamMode.Memory;

		public static class SoundStreamMode
		{
			public const string File = "file";
			public const string Memory = "memory";
		}
	}
}
