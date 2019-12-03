using System;
using System.Collections.Generic;
using System.IO;
using Swift.Project;

namespace Swift.Assets
{
	internal class Sounds : Section
	{
		public override char[] Tag
		{
			get
			{
				return new[] { 'S', 'O', 'N', 'D' };
			}
		}

		private readonly GameProject project;

		public Sounds(GameProject project)
		{
			this.project = project;
		}

		public override void Write(BinaryWriter writer)
		{
			int audioIndex = 0;

			writer.Write(project.Sounds.Count);
			foreach (var sound in project.Sounds)
			{
				byte mode = (byte)new List<string>
				{
					Sound.SoundStreamMode.File,
					Sound.SoundStreamMode.Memory
				}.IndexOf(sound.Value.Mode);

				writer.Write(sound.Key);
				writer.Write(mode);

				switch (sound.Value.Mode)
				{
					case Sound.SoundStreamMode.File:
						writer.Write(sound.Value.Filename);
						break;

					case Sound.SoundStreamMode.Memory:
						writer.Write(audioIndex++);
						break;

					default:
						Console.WriteLine("Warning: \"{0}\" is an unsupported value for sound.{1}.mode.", sound.Value.Mode, sound.Key);
						break;
				}
			}
		}
	}
}
