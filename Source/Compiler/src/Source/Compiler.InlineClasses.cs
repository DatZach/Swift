using System.Collections.Generic;
using System.Linq;
using Swift.Source.Bytecode;

namespace Swift.Source
{
	internal partial class Compiler
	{
		private void GenerateAssetClasses()
		{
			// Textures class
			List<Field> texturesConstants = project.Textures.Select(texture => new Field
			{
				Name = texture.Key,
				Value = new Variant(texture.Key),
				Visibility = AccessVisibility.Public
			}).ToList();

			Classes.Add(new Class
			{
				Name = "Textures",
				Type = ClassType.Class,
				Constants = texturesConstants
			});

			// Sounds class
			List<Field> soundsConstants = project.Sounds.Select(sound => new Field
			{
				Name = sound.Key,
				Value = new Variant(sound.Key),
				Visibility = AccessVisibility.Public
			}).ToList();

			Classes.Add(new Class
			{
				Name = "Sounds",
				Type = ClassType.Class,
				Constants = soundsConstants
			});

			// Fonts class
			List<Field> fontsConstants = project.Fonts.Select(font => new Field
			{
				Name = font.Key,
				Value = new Variant(font.Key),
				Visibility = AccessVisibility.Public
			}).ToList();

			Classes.Add(new Class
			{
				Name = "Fonts",
				Type = ClassType.Class,
				Constants = fontsConstants
			});

			// Scenes class
			List<Field> scenesConstants = project.Scenes.Select(scene => new Field
			{
				Name = scene.Key,
				Value = new Variant(scene.Key),
				Visibility = AccessVisibility.Public
			}).ToList();

			Classes.Add(new Class
			{
				Name = "Scenes",
				Type = ClassType.Class,
				Constants = scenesConstants
			});
		}
	}
}
