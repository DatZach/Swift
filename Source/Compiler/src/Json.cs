using System;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

namespace Swift
{
	internal static class Json
	{
		public static T Load<T>(string filename)
		{
			JsonSerializerSettings s = new JsonSerializerSettings
			{
				Error = (sender, arg) =>
				{
					throw new JsonException(arg.ErrorContext.Error.Message, arg.ErrorContext.Error);
				}
			};

			T obj = JsonConvert.DeserializeObject<T>(File.ReadAllText(filename), s);

			if (obj.Equals(null))
				throw new JsonException("Empty JSON file: {0}", filename);

			return obj;
		}

		public static T LoadStream<T>(string stream)
		{
			JsonSerializerSettings s = new JsonSerializerSettings
			{
				Error = (sender, arg) =>
				{
					throw new JsonException(arg.ErrorContext.Error.Message, arg.ErrorContext.Error);
				}
			};

			T obj = JsonConvert.DeserializeObject<T>(stream, s);

			if (obj.Equals(null))
				throw new JsonException("Empty JSON stream");

			return obj;
		}

		public static void Save<T>(string filename, T obj)
		{
			JsonSerializerSettings settings = new JsonSerializerSettings
			{
				ContractResolver = new CamelCasePropertyNamesContractResolver()
			};

			File.WriteAllText(filename, JsonConvert.SerializeObject(obj, Formatting.Indented, settings));
		}
	}

	[Serializable]
	internal class JsonException : Exception
	{
		public JsonException(string message, params object[] args)
			: base(String.Format(message, args))
		{

		}

		public JsonException(string message, Exception innerExcetion, params object[] args)
			: base(String.Format(message, args), innerExcetion)
		{

		}
	}
}
