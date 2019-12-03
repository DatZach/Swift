using System.Collections.Generic;
using System.IO;

namespace Swift.Assets
{
	internal class Scenes : Section
	{
		public override char[] Tag
		{
			get
			{
				return new[] { 'S', 'C', 'N', 'E' };
			}
		}

		private readonly Dictionary<string, byte[]> scenes; 

		public Scenes(Dictionary<string, byte[]> scenes)
		{
			this.scenes = scenes;
		}

		public override void Write(BinaryWriter writer)
		{
			writer.Write(scenes.Count);

			foreach(var item in scenes)
			{
				writer.Write(item.Key);
				writer.Write(item.Value.Length);
				writer.Write(item.Value);
			}
		}
	}
}
