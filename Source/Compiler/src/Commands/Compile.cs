using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using Swift.Assets;
using Swift.Project;
using Swift.Source;

namespace Swift.Commands
{
	public class Compile : CommandLineMethod
	{
		public override string Command
		{
			get
			{
				return "compile";
			}
		}

		public override string DocString
		{
			get
			{
				return "Compile a project for a target platform.";
			}
		}

		public override Dictionary<string, string> ArgumentsDocString
		{
			get
			{
				return new Dictionary<string, string>
				{
					{ "<project-name>", "Name of project to compile." },
					{ "[platform]", "Name of platform to target (windows, macosx, linux, ios, android)." },
					{ "--export-sheets", "Export compiled texture sheets." }
				};
			}
		}

		public override void Execute(string[] args)
		{
			if (args.Length < 1)
			{
				Console.WriteLine("swift compile <project-name> [platform]");
				return;
			}

			if (!Swift.License.IsRegistered())
			{
				Console.WriteLine("This copy of Swift is unlicensed. Please contact Zach Reedy for your Alpha license key.");
				return;
			}

			string projectName = args[0];

			if (!Directory.Exists(projectName))
			{
				Console.WriteLine("Project directory does not exist.");
				return;
			}

			GameData.PlatformFlags platform;

			if (args.Length > 1 && !args[1].StartsWith("--"))
			{
				if (!Enum.TryParse(args[1], true, out platform))
				{
					Console.WriteLine("Warning: Unknown target platform \"{0}\", targeting default platform instead.", args[1]);
					platform = DetectPlatform();
				}
			}
			else
				platform = DetectPlatform();

			if (platform == GameData.PlatformFlags.Unknown)
				Console.WriteLine("Warning: Could not resolve current target platform, games may not compile or run.");

			Stopwatch compileTimer = new Stopwatch();
			compileTimer.Start();

#if !DEBUG
			try
			{
#endif
				GameProject gameProject = new GameProject();
				gameProject.Load(projectName);

				if (File.Exists(gameProject.TargetGameDataPath))
					File.Delete(gameProject.TargetGameDataPath);

				GameData gameData = new GameData(gameProject, platform)
				{
					ExportSheets = args.Any(arg => arg == "--export-sheets"),
					ShowDisassembly = args.Any(arg => arg == "--disassemble")
				};

				if (gameData.SerializeFromProject())
				{
					if (!gameData.Save(gameProject.TargetGameDataPath))
						throw new CompilerException("Cannot save game data.");

					compileTimer.Stop();
					Console.WriteLine("Compiled in {0}ms.", compileTimer.ElapsedMilliseconds);
				}
#if !DEBUG
			}
			catch (CompilerException e)
			{
				Console.WriteLine(e.Message);
			}
			catch (IOException e)
			{
				Console.WriteLine(e.Message);
			}
			catch (Exception e)
			{
				Console.WriteLine("UNHANDLED EXCEPTION: {0}", e.Message);
				Console.WriteLine("--- Stack Trace ---");
				Console.WriteLine(e.StackTrace);
			}
#endif
		}

		private static GameData.PlatformFlags DetectPlatform()
		{
			switch (Environment.OSVersion.Platform)
			{
				case PlatformID.Win32NT:
					return GameData.PlatformFlags.Windows;

				case PlatformID.MacOSX:
					return GameData.PlatformFlags.MacOsx;

				case PlatformID.Unix:
					return (Directory.Exists("/Applications") && Directory.Exists("/System") &&
							Directory.Exists("/Users") && Directory.Exists("/Volumes"))
								? GameData.PlatformFlags.MacOsx
								: GameData.PlatformFlags.Linux;

				default:
					return GameData.PlatformFlags.Unknown;
			}
		}
	}
}
