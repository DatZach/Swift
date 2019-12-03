using System.Collections.Generic;

namespace Swift.Source.Bytecode
{
	internal class Enumeration
	{
		public string Name = "";
		public AccessVisibility Visibility = AccessVisibility.Private;
		public Dictionary<string, long> Values = new Dictionary<string, long>();
	}
}
