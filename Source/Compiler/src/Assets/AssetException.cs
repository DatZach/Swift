using System;

namespace Swift.Assets
{
	[Serializable]
	internal class AssetException : Exception
	{
		public AssetException(string message, params object[] args)
			: base(String.Format(message, args))
		{

		}

		public AssetException(string message, Exception innerException, params object[] args)
			: base(String.Format(message, args), innerException)
		{

		}
	}
}
