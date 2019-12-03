using System.Collections.Generic;
using System.IO;
using Swift.Source.Bytecode;

namespace Swift.Assets
{
	internal class Bytecode : Section
	{
		public override char[] Tag
		{
			get
			{
				return new[] { 'C', 'O', 'D', 'E' };
			}
		}

		private readonly List<Class> classes; 

		public Bytecode(List<Class> classes)
		{
			this.classes = classes;
		}

		public override void Write(BinaryWriter writer)
		{
			writer.Write(classes.Count);
			foreach(Class obj in classes)
			{
				writer.Write((byte)obj.Type);
				writer.Write(obj.Name);

				// Fields
				writer.Write((ushort)obj.Fields.Count);
				foreach(Field field in obj.Fields)
				{
					writer.Write(field.Name);
					field.Value.Write(writer);
				}

				// Methods
				writer.Write((ushort)obj.Methods.Count);
				foreach(Method method in obj.Methods)
				{
					writer.Write(method.Name);
					writer.Write((byte)method.Arguments.Count);
					writer.Write(method.Instructions.Count);
					foreach(var instruction in method.Instructions)
						instruction.Write(writer);
				}
			}
		}
	}
}
