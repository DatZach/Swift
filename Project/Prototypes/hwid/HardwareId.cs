using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Security.Cryptography;
using System.Text;

namespace hwid
{
	internal static class HardwareId
	{
		public static string Hash { get; private set; }

		static HardwareId()
		{
			Hash = Sha1Hash(GetMacAddress());
		}

		private static string GetMacAddress()
		{
			foreach (NetworkInterface nic in NetworkInterface.GetAllNetworkInterfaces())
			{
				if (nic.OperationalStatus != OperationalStatus.Up)
					continue;
				
				return nic.GetPhysicalAddress().ToString();
			}

			return String.Empty;
		}

        private static string Sha1Hash(string s)
        {
            byte[] bytes = Encoding.ASCII.GetBytes(s);
 
            SHA1 sha1 = SHA1.Create();
            byte[] hashBytes = sha1.ComputeHash(bytes);
 
            return HexStringFromBytes(hashBytes);
        }

		private static string HexStringFromBytes(IEnumerable<byte> bytes)
        {
            var sb = new StringBuilder();
            foreach(var hex in bytes.Select(b => b.ToString("x2")))
	            sb.Append(hex);

            return sb.ToString();
        }
	}
}
