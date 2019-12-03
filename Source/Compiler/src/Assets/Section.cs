using System.IO;

namespace Swift.Assets
{
	internal abstract class Section
	{
		public abstract char[] Tag { get; }
		public abstract void Write(BinaryWriter writer);
	}
}
