using System;
using System.Collections.Generic;

namespace Swift.Commands
{
	public class License : CommandLineMethod
	{
		public override string Command
		{
			get
			{
				return "license";
			}
		}

		public override string DocString
		{
			get
			{
				return "License this copy of Swift.";
			}
		}

		public override Dictionary<string, string> ArgumentsDocString
		{
			get
			{
				return new Dictionary<string, string>
				{
					{ "add <key>", "Registers key with software." },
					{ "show", "Displays your current license key." }
				};
			}
		}

		public override void Execute(string[] args)
		{
			if (args.Length < 1)
			{
				Console.WriteLine("swift license [command]");
				return;
			}

			switch(args[0])
			{
				case "add":
				{
					if (args.Length < 2)
						return;

					bool result = Swift.License.Register(args[1]);
					Console.WriteLine(result ? "Success!" : "Invalid key.");
					break;
				}

				case "show":
				{
					string key = Swift.License.GetLicenseKey();
					Console.WriteLine(String.IsNullOrEmpty(key) ? "Not registered!" : key);
					break;
				}
			}
		}
	}
}
