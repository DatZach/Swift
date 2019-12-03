using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Swift.Commands;

namespace Swift
{
	internal static class CommandLine
	{
		private static readonly List<CommandLineMethod> Methods;

		static CommandLine()
		{
			Assembly assembly = Assembly.GetCallingAssembly();
			IEnumerable<Type> types = assembly.GetExportedTypes().Where(t => t.IsSubclassOf(typeof(CommandLineMethod)));

			Methods = types.Select(t => (CommandLineMethod)Activator.CreateInstance(t)).ToList();
		}

		public static void Switch(string[] args)
		{
			if (args.Length == 0)
			{
				ShowHelp();
				return;
			}

			CommandLineMethod method = Methods.FirstOrDefault(m => m.Command == args[0].ToLower());
			if (method == null)
			{
				ShowHelp();
				return;
			}

			string[] commandArguments = new string[args.Length - 1];
			Array.Copy(args, 1, commandArguments, 0, commandArguments.Length);

			method.Execute(commandArguments);
		}

		public static void ShowHelp()
		{
			Console.WriteLine("swift <command> [args]");
			Console.WriteLine();
			Console.WriteLine("Commands:");
			
			foreach(CommandLineMethod method in Methods.OrderBy(m => m.Command))
				Console.WriteLine(" {0,15}\t{1}", method.Command, method.DocString);

			Console.WriteLine();
			Console.WriteLine("(c) 2013 x2048");
		}

		public static void ShowHelpCommand(string command)
		{
			CommandLineMethod method = Methods.FirstOrDefault(m => m.Command == command.ToLower());
			if (method == null)
			{
				ShowHelp();
				return;
			}

			Console.WriteLine("swift {0} {1}", command, method.ArgumentsDocString.Count == 0 ? "" : "[args]");
			Console.WriteLine(method.DocString);

			if (method.ArgumentsDocString.Count > 0)
			{
				Console.WriteLine();
				Console.WriteLine("Arguments:");
				foreach(var arg in method.ArgumentsDocString)
					Console.WriteLine(" {0,15}\t{1}", arg.Key, arg.Value);
			}
		}
	}
}
