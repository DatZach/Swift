using System;

namespace hwid
{
	public class Program
	{
		public static void Main(string[] args)
		{
			Console.WriteLine("HWID\t: {0}", HardwareId.Hash);
			Console.ReadKey();
		}
	}
}
