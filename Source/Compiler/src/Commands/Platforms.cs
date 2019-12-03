using System;
using System.Collections.Generic;

namespace Swift.Commands
{
	public class Platforms : CommandLineMethod
	{
		public override string Command
		{
			get
			{
				return "platforms";
			}
		}

		public override string DocString
		{
			get
			{
				return "Display a list of registered target platforms.";
			}
		}

		public override Dictionary<string, string> ArgumentsDocString
		{
			get
			{
				return new Dictionary<string, string>();
			}
		}

		public override void Execute(string[] args)
		{
			Console.WriteLine("Not implemented.");
		}
	}
}
