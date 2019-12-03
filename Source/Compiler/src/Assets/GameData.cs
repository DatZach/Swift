using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using Swift.Source;
using Swift.Source.Bytecode;
using Swift.Source.Lexer;

namespace Swift.Assets
{
	internal class GameData
	{
		public const string GameDataExtension = ".sgd";
		private static readonly char[] Magic = { 'S', 'W', 'A', 'G' };
		private const ushort Version = 11;

		public bool ExportSheets;
		public bool ShowDisassembly;

		private readonly List<Section> sections;
		private readonly Project.GameProject project;
		private readonly PlatformFlags platform;

		public GameData(Project.GameProject project, PlatformFlags platform)
		{
			sections = new List<Section>();
			this.project = project;
			this.platform = platform;
		}

		public bool Save(string filename)
		{
			string path = Path.GetDirectoryName(filename) ?? "";

			if (sections.Count == 0)
				return false;

			if (!Directory.Exists(path))
				Directory.CreateDirectory(path);

			try
			{
				Console.WriteLine("Saving game data '{0}'...", Path.GetFileName(project.TargetGameDataPath));

				using (BinaryWriter writer = new BinaryWriter(new FileStream(project.TargetGameDataPath, FileMode.Create)))
				{
					writer.Write(Magic);
					writer.Write(Version);
					writer.Write((byte)platform);

					writer.Write((byte)sections.Count);
					foreach (Section section in sections)
					{
						Console.WriteLine("Writing section '{0}'...", new String(section.Tag));
						writer.Write(section.Tag);
						section.Write(writer);
					}
				}
			}
			catch (AssetException e)
			{
				if (File.Exists(filename))
					File.Delete(filename);

				Console.WriteLine("Asset Error: {0}", e.Message);

				return false;
			}

			return true;
		}

		public bool SerializeFromProject()
		{
			Compiler compiler = null;

			Console.WriteLine("Serializing project...");

			Stopwatch sourceCompilerTimer = new Stopwatch();
			sourceCompilerTimer.Start();
			
#if !DEBUG
			try
			{
#endif
				compiler = new Compiler(project);
				compiler.Compile(project.Scripts.Select(source => new TokenStream(source)).ToList());
#if !DEBUG
			}
			catch (LexerException e)
			{
				Console.WriteLine("Error in file \"{0}\" line {1}:", Path.GetFileName(Tokenizer.Filename), Tokenizer.Line);
				Console.WriteLine("\t{0}", e.Message);
				return false;
			}
			catch (CompilerException e)
			{
				if (compiler == null)
				{
					// NOTE Apparently, this CAN happen.
					Console.WriteLine("An error occurred while initializing the compiler:");
					Console.WriteLine("\t{0}", e.Message);
				}
				else
				{
					string filename = Path.GetFileName(compiler.TokenStream.Filename);
					uint currentLine = compiler.TokenStream.CurrentLine;
					Console.WriteLine("Error in file '{0}' line {1}:", filename, currentLine);
					Console.WriteLine("\t{0}", e.Message);
				}

				return false;
			}
#endif

			sourceCompilerTimer.Stop();

			if (ShowDisassembly)
				DisassembleClasses(compiler.Classes);

			Console.WriteLine("Compiled {0} classes in {1}ms.", compiler.Classes.Count, sourceCompilerTimer.ElapsedMilliseconds);

			try
			{
				Textures textures = new Textures(project);
				sections.Add(new Configuration(project.Config));
				sections.Add(new Drm());
				sections.Add(textures);
				sections.Add(new Sounds(project));
				sections.Add(new Bytecode(compiler.Classes));
				sections.Add(new Scenes(project.Scenes));
				sections.Add(new TextureSheets(textures.TextureSheets, project, ExportSheets));
				sections.Add(new Audio(project));
			}
			catch(Exception e)
			{
				Console.WriteLine("File {0}: {1}", e.Source, e.Message);
				return false;
			}

			return true;
		}

		private static void DisassembleClasses(IEnumerable<Class> classes)
		{
			foreach (Class c in classes)
			{
				Console.WriteLine("=== Class {0} ===", c.Name);
				foreach (Method m in c.Methods)
				{
					Console.Write("--- Method {0} (", m.Name);
					foreach (string arg in m.Arguments)
						Console.Write(arg + (arg == m.Arguments.Last() ? "" : ", "));

					Console.WriteLine(") ---");

					int p = 0;
					foreach (Instruction i in m.Instructions)
						Console.WriteLine("{1}\t{0}", i, p++);
				}
			}
		}

		[Flags]
		public enum PlatformFlags : byte
		{
			Windows = 0x01,
			MacOsx = 0x02,
			Linux = 0x04,
			Android = 0x08,
			Ios = 0x10,
			Html5 = 0x20,
			Undefined = 0x40,
			Unknown = 0x80
		}
	}
}
