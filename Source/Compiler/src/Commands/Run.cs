using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using Swift.Project;

namespace Swift.Commands
{
	public class Run : CommandLineMethod
	{
		public override string Command
		{
			get
			{
				return "run";
			}
		}

		public override string DocString
		{
			get
			{
				return "Run a compiled game.";
			}
		}

		public override Dictionary<string, string> ArgumentsDocString
		{
			get
			{
				return new Dictionary<string, string>
				{
					{ "<project-name>", "Name of project to run." },
					{ "[args]", "Command line arguments to pass to Engine." }
				};
			}
		}

		public override void Execute(string[] args)
		{
			if (args.Length < 1)
			{
				Console.WriteLine("swift run <project-name> [args]");
				return;
			}

			if (!Swift.License.IsRegistered())
			{
				Console.WriteLine("This copy of Swift is unlicensed. Please contact Zach Reedy for your Alpha license key.");
				return;
			}

			if (string.IsNullOrEmpty(Program.EnginePath))
			{
				Console.WriteLine("Error: Cannot locate suitable engine to launch with.");
				return;
			}

			string target = args.First();
			if (!Directory.Exists(target))
			{
				Console.WriteLine("Project directory does not exist.");
				return;
			}

			GameProject project = new GameProject();
			project.Load(target);

			string arguments = "--game \"" + project.TargetGameDataPath + "\" --debug";
			for (int i = 1; i < args.Length; ++i)
				arguments += " " + args[i];

			try
			{
				Console.WriteLine("Running game from \"{0}\"", project.TargetPath);
				Console.WriteLine("{0} {1}", Program.EnginePath, arguments);

				ProcessStartInfo startInfo = new ProcessStartInfo(Program.EnginePath, arguments)
				{
					WorkingDirectory = project.TargetPath,
					RedirectStandardOutput = false,
					RedirectStandardError = false,
					UseShellExecute = false,
					CreateNoWindow = false
				};

				Process engine = Process.Start(startInfo);
				if (engine != null)
				{
					engine.WaitForExit();

					Console.WriteLine();

					if (engine.ExitCode != 0)
						Console.WriteLine("Engine returned with exit code {0}", engine.ExitCode);
				}
				else
					Console.WriteLine("Cannot start engine, but it was found.");
			}
			catch (Exception)
			{
				Console.WriteLine("Cannot start engine because it wasn't found.");
				Console.WriteLine("Tried to start it from \"{0}\".", Program.EnginePath);
			}
		}
	}
}
