using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;

namespace Swift.Commands
{
	public class Version : CommandLineMethod
	{
		public override string Command
		{
			get
			{
				return "version";
			}
		}

		public override string DocString
		{
			get
			{
				return "Display current compiler version.";
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
			Console.WriteLine("Swift Compiler v{0}", GetInformationalVersion());
		}

		private static string GetInformationalVersion()
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			return FileVersionInfo.GetVersionInfo(assembly.Location).ProductVersion;
		}
	}
}
