using System.Collections.Generic;
using Swift.Source.Lexer;

namespace Swift.Source.Bytecode
{
	internal class Class
	{
		public string Name;
		public ClassType Type;
		public List<Method> Methods { get; private set; }
		public List<Field> Fields { get; private set; }
		public List<Field> Constants; // { get; private set; }
		public List<Enumeration> Enums { get; private set; }
		public TokenStream Stream;
  
		public Class()
		{
			Name = "";
			Type = ClassType.Undefined;
			Methods = new List<Method>();
			Fields = new List<Field>();
			Constants = new List<Field>();
			Enums = new List<Enumeration>();
			Stream = null;
		}
	}

	internal enum ClassType
	{
		Undefined,
		Class,
		Global,
		Entity,
		State
	}
}
