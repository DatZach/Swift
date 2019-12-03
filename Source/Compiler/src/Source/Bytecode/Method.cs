using System.Collections.Generic;

namespace Swift.Source.Bytecode
{
	internal class Method
	{
		public string Name;
		public AccessVisibility Visibility;
		public List<string> Arguments; 
		public bool Static;
		public Dictionary<string, Variant> Variables;
		public List<Instruction> Instructions { get; private set; }
		public int Position;
		public string Filename;
		public uint Line;

		public Method()
		{
			Name = "";
			Visibility = AccessVisibility.Private;
			Arguments = new List<string>();
			Static = false;
			Variables = new Dictionary<string, Variant>();
			Instructions = new List<Instruction>();
			Position = -1;
			Filename = "";
			Line = 0;
		}
	}
}
