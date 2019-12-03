using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using Swift.Project;

namespace Swift.Assets
{
	internal class Textures : Section
	{
		public override char[] Tag
		{
			get
			{
				return new [] { 'T', 'X', 'T', 'R' };
			}
		}

		public List<TextureSheet> TextureSheets { get; private set; }
		private readonly GameProject project;
		private readonly List<Texture> textures; 

		public Textures(GameProject project)
		{
			this.project = project;
			TextureSheets = new List<TextureSheet>();
			textures = new List<Texture>();

			CompileTextureSheets(project.Config.SheetSize);
		}

		public override void Write(BinaryWriter writer)
		{
			writer.Write(textures.Count);
			foreach(var texture in textures)
			{
				writer.Write(texture.Name);

				writer.Write((ushort)texture.Size.X);
				writer.Write((ushort)texture.Size.Y);

				writer.Write(texture.Frames.Count);
				foreach(Texture.Frame frame in texture.Frames)
				{
					writer.Write((ushort)frame.SheetIndex);
					writer.Write((ushort)frame.Position.X);
					writer.Write((ushort)frame.Position.Y);
				}

				writer.Write(texture.Origin.X);
				writer.Write(texture.Origin.Y);
				writer.Write(texture.Smooth);
				writer.Write((float)texture.BoundingBox.Left);
				writer.Write((float)texture.BoundingBox.Top);
				writer.Write((float)texture.BoundingBox.Right);
				writer.Write((float)texture.BoundingBox.Bottom);
				writer.Write(texture.CollisionMode);
			}
		}

		private void CompileTextureSheets(int size)
		{
			Stopwatch timer = new Stopwatch();
			timer.Start();

			// TODO Put this somewhere else
			foreach (var texture in project.Textures)
			{
				Texture t = new Texture
				{
					Name = texture.Key,
					Origin = texture.Value.Origin,
					Smooth = texture.Value.Smooth,
					BoundingBox = texture.Value.BBox,
					CollisionMode = Texture.TranslateCMode(texture.Value.CMode)
				};

				// TODO Support Texture.0000.ext format
				if (texture.Value.Animated)
				{
					int i = 0;

					while(true)
					{
						StringBuilder builder = new StringBuilder();
						builder.Append(Path.GetFileNameWithoutExtension(texture.Value.Filename));
						builder.Append(".");
						builder.Append(i.ToString("G"));
						builder.Append(Path.GetExtension(texture.Value.Filename));

						string path = Path.Combine(project.TexturesPath, builder.ToString());
						if (!File.Exists(path))
							break;

						t.Frames.Add(new Texture.Frame
						{
							Image = new Bitmap(path)
						});

						++i;
					}
				}
				else
				{
					string path = Path.Combine(project.TexturesPath, texture.Value.Filename);
					if (!File.Exists(path))
						break;

					t.Frames.Add(new Texture.Frame
					{
						Image = new Bitmap(path)
					});
				}

				if (t.Frames.Count == 0)
					throw new AssetException("No frames loaded for texture \"{0}\".", t.Name);

				textures.Add(t);
			}

			textures.Sort((a, b) =>
			{
				int aScore = a.Size.X * a.Size.Y;
				int bScore = b.Size.X * b.Size.Y;

				if (aScore == bScore)
				{
					if (a.Size.Y == b.Size.Y)
						return 0;

					return a.Size.X < b.Size.X ? 1 : -1;
				}

				return aScore < bScore ? 1 : -1;
			});

			foreach (Texture.Frame frame in textures.SelectMany(texture => texture.Frames))
			{
				Vector2i position;
				bool packed = false;

				foreach (TextureSheet sheet in TextureSheets)
				{
					if (!sheet.PackTexture(frame.Image, out position))
						continue;

					frame.SheetIndex = TextureSheets.IndexOf(sheet);
					frame.Position = position;
					packed = true;
					break;
				}

				if (packed)
					continue;

				TextureSheet newSheet = new TextureSheet(size);
				if (!newSheet.PackTexture(frame.Image, out position))
					throw new AssetException("Texture dimensions ({0}x{1}) exceed sheet's ({2}x{2}).",
											 frame.Image.Size.Width, frame.Image.Size.Height, size);

				TextureSheets.Add(newSheet);

				frame.SheetIndex = TextureSheets.IndexOf(newSheet);
				frame.Position = position;
			}

			timer.Stop();

			Console.WriteLine("Compiled {0} textures into {1} sheets ({3}x{3}) in {2}ms.",
							  textures.Count,
							  TextureSheets.Count,
							  timer.ElapsedMilliseconds,
							  size);
		}

		private class Texture
		{
			public string Name;
			public readonly List<Frame> Frames;
			public Vector2i Size
			{
				get
				{
					Frame frame = Frames.FirstOrDefault();
					return frame == null
							? new Vector2i()
							: new Vector2i(frame.Image.Size.Width, frame.Image.Size.Height);
				}
			}

			public Vector2f Origin;
			public bool Smooth;
			public Rectangle BoundingBox;
			public byte CollisionMode;

			public Texture()
			{
				Name = "";
				Frames = new List<Frame>();
				Origin = new Vector2f();
				Smooth = false;
				BoundingBox = new Rectangle();
				CollisionMode = 0;
			}

			public static byte TranslateCMode(string value)
			{
				return (byte)new List<string> { "none", "precise", "bbox" }.IndexOf(value);
			}

			public class Frame
			{
				public int SheetIndex;
				public Bitmap Image;
				public Vector2i Position;

				public Frame()
				{
					SheetIndex = -1;
					Image = null;
					Position = new Vector2i();
				}
			}
		}
	}
}
