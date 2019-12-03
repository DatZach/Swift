using System.Collections.Generic;

namespace Swift.Commands
{
	public class Help : CommandLineMethod
	{
		public override string Command
		{
			get
			{
				return "help";
			}
		}

		public override string DocString
		{
			get
			{
				return "Display this information.";
			}
		}

		public override Dictionary<string, string> ArgumentsDocString
		{
			get
			{
				return new Dictionary<string, string>
				{
					{ "[command]", "Print documentation specific to this command." }
				};
			}
		}

		public override void Execute(string[] args)
		{
			if (args.Length == 0)
				CommandLine.ShowHelp();
			else
				CommandLine.ShowHelpCommand(args[0]);
		}
	}
}
