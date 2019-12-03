using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;

namespace Swift.Assets
{
	internal class TextureSheets : Section
	{
		public override char[] Tag
		{
			get
			{
				return new[] { 'T', 'X', 'S', 'T' };
			}
		}

		private readonly List<TextureSheet> textureSheets;
		private readonly Project.GameProject project;
		private readonly bool exportSheets;

		public TextureSheets(List<TextureSheet> textureSheets, Project.GameProject project, bool exportSheets)
		{
			this.textureSheets = textureSheets;
			this.project = project;
			this.exportSheets = exportSheets;
		}

		public override void Write(BinaryWriter writer)
		{
			int i = 0;

			writer.Write(textureSheets.Count);
			foreach(TextureSheet textureSheet in textureSheets)
			{
				textureSheet.Compile();

				MemoryStream stream = new MemoryStream();
				textureSheet.Sheet.Save(stream, ImageFormat.Png);

				if (exportSheets)
				{
					string filename = "sheet." + (i++).ToString("G") + ".png";
					string path = Path.Combine(project.RootPath, filename);

					FileStream fileStream = new FileStream(path, FileMode.Create);
					textureSheet.Sheet.Save(fileStream, ImageFormat.Png);
				}

				writer.Write((uint)stream.Length);
				writer.Write(stream.ToArray());
			}
		}
	}
}
