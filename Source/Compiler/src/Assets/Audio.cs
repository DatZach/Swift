using System;
using System.IO;
using System.Linq;
using Swift.Project;

namespace Swift.Assets
{
	internal class Audio : Section
	{
		public override char[] Tag
		{
			get
			{
				return new[] { 'A', 'U', 'D', 'O' };
			}
		}

		private readonly GameProject project;

		public Audio(GameProject project)
		{
			this.project = project;
		}

		public override void Write(BinaryWriter writer)
		{
			writer.Write(project.Sounds.Count(s => s.Value.Mode == Sound.SoundStreamMode.Memory));
			foreach (var kv in project.Sounds)
			{
				Sound sound = kv.Value;
				string path = Path.Combine(project.SoundsPath, sound.Filename);
				
				switch(sound.Mode)
				{
					case Sound.SoundStreamMode.File:
					{
						string targetFilename = Path.GetFileName(path);
						if (String.IsNullOrEmpty(targetFilename))
						{
							Console.WriteLine("Cannot resolve sound \"{0}\"'s target path.", kv.Key);
							break;
						}

						string newPath = Path.Combine(project.TargetPath, targetFilename);
						File.Delete(newPath);
						File.Copy(path, newPath);
						break;
					}

					case Sound.SoundStreamMode.Memory:
					{
						byte[] fileData = null;

						try
						{
							fileData = File.ReadAllBytes(path);
						}
						catch(Exception)
						{
							Console.WriteLine("Cannot open \"{0}\" for serialization.", path);
						}

						if (fileData == null)
							throw new AssetException("No bytes read for audio file. Aborting.");

						writer.Write(fileData.Length);
						writer.Write(fileData);
						break;
					}
				}
			}
		}
	}
}
