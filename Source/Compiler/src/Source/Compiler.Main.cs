using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using Swift.Project;
using Swift.Source.Bytecode;
using Swift.Source.Lexer;

namespace Swift.Source
{
	internal partial class Compiler
	{
		public List<Class> Classes { get; private set; }
		public TokenStream TokenStream { get; private set; }

		private List<Class> engineClasses;
		private readonly GameProject project;

		public Compiler(GameProject project)
		{
			this.project = project;
			Classes = new List<Class>();
			scopeStack = new Stack<List<string>>();
			typeStack = new Stack<string>();
			breakStack = new Stack<Label>();
			continueStack = new Stack<Label>();
			random = new Random();

			LoadEngineLibrary();
		}
		
		public void Compile(IEnumerable<TokenStream> tokenStreams)
		{
			// Generate compile time classes
			GenerateAssetClasses();

			// Pass 0 -- Declare all symbols
			foreach(TokenStream stream in tokenStreams)
			{
				TokenStream = stream;
				Pass0();
			}

			// Pass 1 -- Compile everything
			foreach(Class c in Classes.Where(c => c.Stream != null))
			{
				currentClass = c;
				TokenStream = c.Stream;
				Pass1();
			}

			// Remove classes defined in Engine
			foreach (Class lib in engineClasses)
				Classes.Remove(lib);

			// Remove "namespaces"
			Classes.Remove(Classes.FirstOrDefault(c => c.Name == "Textures"));
			Classes.Remove(Classes.FirstOrDefault(c => c.Name == "Sounds"));
			Classes.Remove(Classes.FirstOrDefault(c => c.Name == "Fonts"));
			Classes.Remove(Classes.FirstOrDefault(c => c.Name == "Scenes"));
		}

		private void LoadEngineLibrary()
		{
			Assembly assembly = Assembly.GetExecutingAssembly();

			// Load engine classes (state, entity, etc)
			Stream engineClassesStream = assembly.GetManifestResourceStream("Swift.Source.Json.EngineClasses.json");
			if (engineClassesStream == null)
				throw new CompilerException("Cannot load engine classes.");

			using (TextReader classesReader = new StreamReader(engineClassesStream))
			{
				engineClasses = Json.LoadStream<List<Class>>(classesReader.ReadToEnd());
				Classes.AddRange(engineClasses);
			}

			// Load engine library (Sprite, Sound, Math, Debug, Game, etc)
			Stream engineLibraryStream = assembly.GetManifestResourceStream("Swift.Source.Json.EngineBindings.json");
			if (engineLibraryStream == null)
				throw new CompilerException("Cannot load engine library.");

			using (TextReader libraryReader = new StreamReader(engineLibraryStream))
			{
				List<Class> libraryClasses = Json.LoadStream<List<Class>>(libraryReader.ReadToEnd());
				Classes.AddRange(libraryClasses);
				engineClasses.AddRange(libraryClasses);
			}
		}
	}
}
