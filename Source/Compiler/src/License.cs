using System;
using System.IO;
using System.Linq;

namespace Swift
{
	internal static class License
	{
		private const string LicenseFilename = "licensekey";

		private static string LicenseFullPath
		{
			get
			{
				return Path.Combine(AppDomain.CurrentDomain.BaseDirectory, LicenseFilename);
			}
		}

		public static LicenseType Type
		{
			get
			{
				return LicenseType.Alpha;
			}
		}

		public static bool IsRegistered()
		{
#if !DEBUG
			string key = GetLicenseKey();
			if (String.IsNullOrEmpty(key))
				return false;

			return CheckKey(key);
#else
			return true;
#endif
		}

		public static string GetLicenseKey()
		{
			if (!File.Exists(LicenseFullPath))
				return "";

			return File.ReadAllText(LicenseFullPath);
		}

		public static bool Register(string key)
		{
			if (!CheckKey(key))
				return false;

			try
			{
				using (TextWriter writer = File.CreateText(LicenseFullPath))
					writer.Write(key);
			}
			catch (Exception)
			{
				return false;
			}

			return true;
		}

		private static bool CheckKey(string key)
		{
			// Check if we have 3 sextets
			if (key.Length < 20 || key[6] != '-' || key[13] != '-')
				return false;

			string[] sextets = key.Split(new[] { '-' });

			// Rot13 the first sextet and check it against the second
			if (Rot13.Transform(sextets[0]) != sextets[1])
				return false;

			string checksumString = sextets[0] + sextets[1];
			int checksum = checksumString.Aggregate(0, (current, ch) => current + (ch ^ 0x2D));
			checksum <<= 2;
			checksum ^= 0x1D234;
			checksum %= 999999;

			return sextets[2] == checksum.ToString("G");
		}
	}

	internal enum LicenseType
	{
		None,
		Alpha,
		Free,
		Registered
	}
}
