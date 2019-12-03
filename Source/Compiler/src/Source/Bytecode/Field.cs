using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace Swift.Source.Bytecode
{
	internal class Field
	{
		public string Name = "";
		[JsonProperty(ItemConverterType = typeof(StringEnumConverter))]
		public AccessVisibility Visibility = AccessVisibility.Private;
		public Variant Value = new Variant();
		public bool Static = false;
	}

	internal enum AccessVisibility
	{
		Public,
		Private,
		Protected
	}
}
