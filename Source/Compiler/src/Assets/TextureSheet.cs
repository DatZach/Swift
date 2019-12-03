using System.Collections.Generic;
using System.Drawing;
using System.Linq;

namespace Swift.Assets
{
	internal class TextureSheet
	{
		public Bitmap Sheet { get; private set; }

		private readonly List<SheetTexture> packedTextures;
		private readonly List<Rectangle> partitionTable;

		public TextureSheet(int size)
		{
			Sheet = new Bitmap(size, size);
			packedTextures = new List<SheetTexture>();
			partitionTable = new List<Rectangle> { new Rectangle(0, 0, size, size) };
		}

		public bool PackTexture(Bitmap texture, out Vector2i position)
		{
			// Find a partition that honors our size, if none can be found this sheet is out of space
			Rectangle space = partitionTable.FirstOrDefault(r => r.Width >= texture.Width && r.Height >= texture.Height);
			if (space.Width == 0 || space.Height == 0)
			{
				position = new Vector2i();
				return false;
			}

			// Create new partitions
			Rectangle rightPartition = new Rectangle(space.X + texture.Width, space.Y, space.Width - texture.Width, texture.Height);
			if (rightPartition.Width != 0 && rightPartition.Height != 0)
				partitionTable.Add(rightPartition);

			Rectangle bottomPartition = new Rectangle(space.X, space.Y + texture.Height, space.Width, space.Height - texture.Height);
			if (bottomPartition.Width != 0 && bottomPartition.Height != 0)
				partitionTable.Add(bottomPartition);

			// Remove old one since we've split it
			partitionTable.Remove(space);

			/*
			 *	We sort the partitions from biggest to smallest horizontally.
			 *	This both speeds up packing and mamkes packing tighter because
			 *	we split partitions slim across and large down, by forcing larger
			 *	images to take the horizontal path first we prevent our largest
			 *	partition from being split unnecessarily.
			 */
			partitionTable.Sort((a, b) =>
			{
				int aScore = a.Width * a.Height;
				int bScore = b.Width * b.Height;

				if (aScore == bScore)
				{
					if (a.Width == b.Width)
						return 0;

					return a.Height < b.Height ? -1 : 1;
				}

				return aScore < bScore ? -1 : 1;
			});

			// Keep track of what we pack
			position = new Vector2i(space.X, space.Y);
			packedTextures.Add(new SheetTexture(texture, position));

			return true;
		}

		public void Compile()
		{
			using (Graphics surface = Graphics.FromImage(Sheet))
			{
				foreach (SheetTexture texture in packedTextures)
					surface.DrawImage(texture.Image, texture.Position.X, texture.Position.Y, texture.Image.Width, texture.Image.Height);
			}
		}

		private class SheetTexture
		{
			public Bitmap Image { get; private set; }
			public Vector2i Position { get; private set; }

			public SheetTexture(Bitmap image, Vector2i position)
			{
				Image = image;
				Position = position;
			}
		}
	}
}
