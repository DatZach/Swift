using System.IO;

namespace Swift
{
	internal static class Program
	{
		public static string EnginePath
		{
			get
			{
				if (File.Exists("SwiftEngine.exe"))
					return "SwiftEngine.exe";

				return File.Exists(@"X:\Swift\Source\Engine\bin\windows\Debug\SwiftEngine.exe")
							? @"X:\Swift\Source\Engine\bin\windows\Debug\SwiftEngine.exe"
							: "";
			}
		}

		public static void Main(string[] args)
		{
			// Come the fuck on git...
			CommandLine.Switch(args);
		}
	}
}
