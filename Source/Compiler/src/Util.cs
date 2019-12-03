using System.Collections.Generic;

namespace Swift
{
	internal static class Util
	{
		public static IEnumerable<T> FastReverse<T>(this IList<T> items)
		{
			for (int i = items.Count - 1; i >= 0; --i)
				yield return items[i];
		}
	}
}
