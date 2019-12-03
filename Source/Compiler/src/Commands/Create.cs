using System;
using System.Collections.Generic;
using Swift.Project;

namespace Swift.Commands
{
	public class Create : CommandLineMethod
	{
		public override string Command
		{
			get
			{
				return "create";
			}
		}

		public override string DocString
		{
			get
			{
				return "Create a new Swift project.";
			}
		}

		public override Dictionary<string, string> ArgumentsDocString
		{
			get
			{
				return new Dictionary<string, string>
				{
					{ "<project-name>", "Name of project to create." }
				};
			}
		}

		public override void Execute(string[] args)
		{
			if (args.Length < 1)
			{
				Console.WriteLine("swift compile <project-name>");
				return;
			}

			try
			{
				GameProject.Create(args[0]);
			}
			catch (Exception e)
			{
				Console.WriteLine(e.Message);
			}
		}
	}
}
