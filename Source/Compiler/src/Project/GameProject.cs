using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json.Linq;

namespace Swift.Project
{
	internal class GameProject
	{
		private const string SceneDataExtension = "*.sd";

		public string Name { get; private set; }

		public string RootPath { get; private set; }
		public string TargetGameDataPath
		{
			get
			{
				return Path.Combine(RootPath, "bin", Project.Target + Assets.GameData.GameDataExtension);
			}
		}

		public string TargetPath
		{
			get
			{
				return Path.GetDirectoryName(TargetGameDataPath);
			}
		}

		public string TexturesPath { get; private set; }
		public string SoundsPath { get; private set; }
		public string FontsPath { get; private set; }
		public string ScenesPath { get; private set; }
		public string ScriptsPath { get; private set; }

		public ProjectProperties Project { get; private set; }
		public ConfigurationProperties Config { get; private set; }
		public Dictionary<string, Texture> Textures { get; private set; }
		public Dictionary<string, Sound> Sounds { get; private set; }
		public Dictionary<string, Font> Fonts { get; private set; }
		public Dictionary<string, byte[]> Scenes { get; private set; }
		public List<string> Scripts { get; private set; }

		public GameProject()
		{
			Name = "";

			Project = new ProjectProperties();
			Config = new ConfigurationProperties();
			Textures = new Dictionary<string, Texture>();
			Sounds = new Dictionary<string, Sound>();
			Fonts = new Dictionary<string, Font>();
			Scenes = new Dictionary<string, byte[]>();
			Scripts = new List<string>();
		}

		public void Load(string name)
		{
			Name = name;

			RootPath = Name;

			Console.Write("Reading project... ");

			Project = Json.Load<ProjectProperties>(Path.Combine(Name, "project.sproj"));

			foreach (string item in Project.Assets)
			{
				if (ReadAssetConfiguration(item))
					continue;

				// This is really ugly...
				string path = Path.Combine(Name, item);
				DirectoryInfo directoryInfo = new DirectoryInfo(path);
				FileInfo[] files = directoryInfo.GetFiles(SceneDataExtension, SearchOption.AllDirectories);

				foreach (FileInfo file in files)
					Scenes.Add(Path.GetFileNameWithoutExtension(file.Name), File.ReadAllBytes(file.FullName));

				ScenesPath = Path.GetDirectoryName(path);
			}

			foreach (string item in Project.Sources)
			{
				string path = Path.Combine(Name, item);
				DirectoryInfo directoryInfo = new DirectoryInfo(path);
				foreach (FileInfo file in directoryInfo.GetFiles("*.ss", SearchOption.AllDirectories))
					Scripts.Add(Path.Combine(Name, "scripts", file.Name));

				ScriptsPath = Path.GetDirectoryName(path);
			}
		}

		private bool ReadAssetConfiguration(string item)
		{
			string path = Path.Combine(Name, item);
			if (!Path.HasExtension(path))
				path = Path.Combine(path, item + ".json");

			if (!File.Exists(path))
				return false;

			JObject jObject = JObject.Parse(File.ReadAllText(path));
			foreach (KeyValuePair<string, JToken> prop in jObject)
			{
				switch (prop.Key.ToLower())
				{
					case "configuration":
						Config = ReadConfiguration<ConfigurationProperties>(path);
						return true;

					case "textures":
						Textures = ReadConfiguration<Dictionary<string, Texture>>(path);
						TexturesPath = Path.GetDirectoryName(path);
						return true;

					case "sounds":
						Sounds = ReadConfiguration<Dictionary<string, Sound>>(path);
						SoundsPath = Path.GetDirectoryName(path);
						return true;

					case "fonts":
						Fonts = ReadConfiguration<Dictionary<string, Font>>(path);
						FontsPath = Path.GetDirectoryName(path);
						return true;

					default:
						return false;
				}
			}

			return false;
		}

		private static T ReadConfiguration<T>(string path)
		{
			// This is here to clean up the code a little
			// Because of how our JSON files are formatted we need to read the tag
			// and them immediately discard it. This does that for us.
			return Json.Load<Dictionary<string, T>>(path).Values.First();
		}

		public static void Create(string name)
		{
			if (Path.HasExtension(name))
			{
				Console.WriteLine("Projects cannot have extensions.");
				return;
			}

			// Create directory structure, if it doesn't exist
			if (Directory.Exists(name))
			{
				Console.WriteLine("Project with that name already exists.");
				return;
			}

			name = Path.GetFileNameWithoutExtension(name);
			if (string.IsNullOrEmpty(name))
			{
				Console.WriteLine("Cannot resolve project path.");
				return;
			}

			// Create directory structure
			List<string> directoryTree = new List<string>
			{
				"Textures",
				"Sounds",
				"Fonts",
				"Scenes",
				"Scripts"
			};

			Directory.CreateDirectory(name);
			foreach (string sub in directoryTree)
			{
				// Create subdirectory
				Directory.CreateDirectory(Path.Combine(name, sub));

				if (sub == "Scenes" || sub == "Scripts")
					continue;

				// TODO Really? This is terrible.
				// Create project file
				File.WriteAllText(Path.Combine(name, sub, sub + ".json"),
					"{" + Environment.NewLine +
					"\t\"" + sub + "\" :" + Environment.NewLine +
					"\t{" + Environment.NewLine +
					Environment.NewLine +
					"\t}" + Environment.NewLine +
					"}" + Environment.NewLine
				);
			}

			// Create top level project files
			ProjectProperties properties = new ProjectProperties
			{
				Target = name,
				Assets = new List<string>
				{
					"config.json",
					"Textures",
					"Sounds",
					"Fonts",
					"Scenes"
				},
				Sources = new List<string>
				{
					"Scripts"
				}
			};

			try
			{
				Json.Save(Path.Combine(name, "project.sproj"), properties);
				Json.Save(Path.Combine(name, "config.json"), new Dictionary<string, ConfigurationProperties>
				{
					{
						"Configuration",
						new ConfigurationProperties
						{
							Title = name
						}
					}
				});
			}
			catch (JsonException e)
			{
				Console.WriteLine("Cannot create project \"{0}\"", name);
				Console.WriteLine(e.Message);
			}
		}
	}
}
