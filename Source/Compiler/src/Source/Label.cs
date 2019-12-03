using System.Collections.Generic;
using System.Linq;

namespace Swift.Source
{
	internal class Label
	{
		private readonly Compiler compiler;
		private readonly List<int> patches;
		private int labelOffset;

		public Label(Compiler compiler)
		{
			this.compiler = compiler;
			patches = new List<int>();
			labelOffset = 0;
		}

		public void Mark()
		{
			labelOffset = compiler.Instructions.Count;
		}

		public void PatchHere()
		{
			compiler.Instructions.Last().Operand.IntValue = 0;
			patches.Add(compiler.Instructions.Count - 1);
		}

		public void Fix()
		{
			foreach (int offset in patches)
				compiler.Instructions[offset].Operand.IntValue = labelOffset;
		}

		public void ClearPatches()
		{
			patches.Clear();
		}
	}
}
