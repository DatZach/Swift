using System;
using System.IO;

namespace Swift.Assets
{
	internal class Configuration : Section
	{
		public override char[] Tag
		{
			get
			{
				return new[] { 'C', 'N', 'F', 'G' };
			}
		}

		private readonly Project.ConfigurationProperties configuration;

		public Configuration(Project.ConfigurationProperties configuration)
		{
			this.configuration = configuration;
		}

		public override void Write(BinaryWriter writer)
		{
			if (String.IsNullOrEmpty(configuration.InitialState))
				throw new AssetException("Initial state not set in Game Configuration.");

			writer.Write(configuration.Title);

			writer.Write((ushort)configuration.Size.X);
			writer.Write((ushort)configuration.Size.Y);
			writer.Write(configuration.Fullscreen);
			writer.Write(configuration.Vsync);
			writer.Write(configuration.Cursor);
			writer.Write(configuration.AntiAliasingLevel);
			writer.Write(configuration.TargetFramerate);
			writer.Write(configuration.InitialState);
		}
	}
}
