using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace Swift.Commands
{
	public class Update : CommandLineMethod
	{
		public override string Command
		{
			get
			{
				return "update";
			}
		}

		public override string DocString
		{
			get
			{
				return "Update Swift if any updates are found.";
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
			if (!Swift.License.IsRegistered())
			{
				Console.WriteLine("This copy of Swift is unlicensed. Please contact Zach Reedy for your Alpha license key.");
				return;
			}

			Process.Start("Updater.exe");
		}
	}
}
