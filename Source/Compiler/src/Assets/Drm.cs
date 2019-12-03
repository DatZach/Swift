using System;
using System.IO;
using System.Security.Cryptography;

namespace Swift.Assets
{
	internal class Drm : Section
	{
		public override char[] Tag
		{
			get
			{
				return new[] { 'S', 'D', 'R', 'M' };
			}
		}

		public override void Write(BinaryWriter writer)
		{
			writer.Write((byte)License.Type);

			switch(License.Type)
			{
				case LicenseType.Alpha:
					writer.Write(Sha1String(License.GetLicenseKey()));
					break;

				case LicenseType.Registered:
					writer.Write((byte)GameData.PlatformFlags.Windows);
					writer.Write(Sha1String(License.GetLicenseKey()));
					break;
			}
		}

		private string Sha1String(string value)
		{
			byte[] bytes = new byte[value.Length * sizeof(char)];
			Buffer.BlockCopy(value.ToCharArray(), 0, bytes, 0, bytes.Length);

			SHA1CryptoServiceProvider sha1 = new SHA1CryptoServiceProvider();
			byte[] hash = sha1.ComputeHash(bytes);

			return BitConverter.ToString(hash).Replace("-", null);
		}
	}
}
