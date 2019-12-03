using System.Collections.Generic;

namespace Swift.Commands
{
	public abstract class CommandLineMethod
	{
		public abstract string Command { get; }
		public abstract string DocString { get; }
		public abstract Dictionary<string, string> ArgumentsDocString { get; }

		public abstract void Execute(string[] args);
	}
}
