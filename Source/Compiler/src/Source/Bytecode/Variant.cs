using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace Swift.Source.Bytecode
{
	internal class Variant
	{
		private const double Epsilon = Double.Epsilon;

		private readonly long timestamp;

		[JsonProperty(ItemConverterType = typeof(StringEnumConverter))]
		public VariantType Type { get; private set; }
		public Int64 IntValue;
		public double DoubleValue;
		public string StringValue;
		public object ObjectValue;
		public List<Variant> ArrayValue;

		// This is honestly absolutely terrible, but what else can I do?
		public string ObjectName;

		public int Length
		{
			get
			{
				switch (Type)
				{
					case VariantType.String:
						return StringValue.Length;

					case VariantType.Double:
						return 8;

					case VariantType.Int64:
						return 8;

					case VariantType.Nil:
						return 0;

					case VariantType.List:
						return ArrayValue.Count;

					case VariantType.Object:
						return 8;
				}

				return 0;
			}
		}

		public Variant this[int index]
		{
			get
			{
				if (Type != VariantType.List)
					return new Variant();

				return ArrayValue[index];
			}

			set
			{
				if (Type != VariantType.List)
					return;

				ArrayValue[index] = value;
			}
		}

		public Variant()
		{
			timestamp = DateTime.Now.Ticks;
			Type = VariantType.Nil;
		}

		public Variant(Int64 value)
		{
			timestamp = DateTime.Now.Ticks;
			Type = VariantType.Int64;
			IntValue = value;
		}

		public Variant(double value)
		{
			timestamp = DateTime.Now.Ticks;
			Type = VariantType.Double;
			DoubleValue = value;
		}

		public Variant(string value)
		{
			timestamp = DateTime.Now.Ticks;
			Type = VariantType.String;
			StringValue = value;
		}

		public Variant(object value)
		{
			timestamp = DateTime.Now.Ticks;
			Type = VariantType.Object;
			ObjectValue = value;
		}

		public Variant(List<Variant> value)
		{
			timestamp = DateTime.Now.Ticks;
			Type = VariantType.List;
			ArrayValue = value;
		}

		public override string ToString()
		{
			switch (Type)
			{
				case VariantType.Int64:
					return IntValue.ToString("G");

				case VariantType.Double:
					return DoubleValue.ToString("F");

				case VariantType.String:
					return '"' + StringValue + '"';

				case VariantType.Object:
					return "object";

				case VariantType.List:
				{
					StringBuilder builder = new StringBuilder();
					builder.Append("[ ");

					for (int i = 0; i < ArrayValue.Count; ++i)
					{
						builder.Append(ArrayValue[i]);

						if (i + 1 != ArrayValue.Count)
							builder.Append(", ");
					}

					builder.Append(" ]");

					return builder.ToString();
				}
			}

			return "nil";
		}

		public override bool Equals(object obj)
		{
			return obj != null && obj.Equals(this);
		}

		public override int GetHashCode()
		{
			int hashCode = 0x7FFFFFFF ^ (int)timestamp;

			return hashCode ^ ((int)Type << 8) ^ ((int)Type << 6) ^ ((int)Type << 4) ^ ((int)Type << 2);
		}

		public Variant Cast(VariantType type)
		{
			switch (Type)
			{
				case VariantType.Int64:
					if (type == VariantType.String)
						return new Variant(ToString());

					if (type == VariantType.Double)
						return new Variant((double)IntValue);

					break;

				case VariantType.Double:
					if (type == VariantType.String)
						return new Variant(ToString());

					if (type == VariantType.Int64)
						return new Variant((Int64)DoubleValue);

					break;

				case VariantType.String:
					if (type == VariantType.Int64)
						return new Variant(Int64.Parse(StringValue, NumberStyles.Number, CultureInfo.InvariantCulture));

					if (type == VariantType.Double)
						return new Variant(Double.Parse(StringValue, NumberStyles.Number, CultureInfo.InvariantCulture));
					break;

				case VariantType.List:
					if (type == VariantType.String)
						return new Variant(ToString());
					break;
			}

			throw new VariantException("Cannot cast type \"{0}\" to type \"{1}\"!", Type, type);
		}

		public void Write(BinaryWriter writer)
		{
			writer.Write((byte)Type);
			if (Type == VariantType.Int64)
				writer.Write(IntValue);
			else if (Type == VariantType.Double)
				writer.Write(DoubleValue);
			else if (Type == VariantType.String)
				writer.Write(StringValue);
			else if (Type == VariantType.List)
			{
				writer.Write(ArrayValue.Count);
				foreach (Variant value in ArrayValue)
					value.Write(writer);
			}
		}

		public static Variant operator +(Variant a, Variant b)
		{
			if (a.Type != b.Type)
				throw new VariantException("Variable types do not match ({0} != {1})", a.Type, b.Type);

			switch (a.Type)
			{
				case VariantType.Int64:
					return new Variant(a.IntValue + b.IntValue);

				case VariantType.Double:
					return new Variant(a.DoubleValue + b.DoubleValue);

				case VariantType.String:
					return new Variant(a.StringValue + b.StringValue);

				case VariantType.List:
				{
					var c = new List<Variant>();
					c.AddRange(a.ArrayValue);
					c.AddRange(b.ArrayValue);

					return new Variant(c);
				}
			}

			throw new VariantException("Cannot + variant type \"{0}\"", a.Type);
		}

		public static Variant operator -(Variant a, Variant b)
		{
			if (a.Type != b.Type)
				throw new VariantException("Variable types do not match ({0} != {1})", a.Type, b.Type);

			if (a.Type == VariantType.Int64)
				return new Variant(a.IntValue - b.IntValue);

			if (a.Type == VariantType.Double)
				return new Variant(a.DoubleValue - b.DoubleValue);

			throw new VariantException("Cannot - variant type \"{0}\"", a.Type);
		}

		public static Variant operator *(Variant a, Variant b)
		{
			if (a.Type == VariantType.String && b.Type == VariantType.Int64)
			{
				StringBuilder builder = new StringBuilder();
				for (Int64 i = 0; i < b.IntValue; ++i)
					builder.Append(a.StringValue);

				return new Variant(builder.ToString());
			}

			if (a.Type != b.Type)
				throw new VariantException("Variable types do not match ({0} != {1})", a.Type, b.Type);

			if (a.Type == VariantType.Int64)
				return new Variant(a.IntValue * b.IntValue);

			if (a.Type == VariantType.Double)
				return new Variant(a.DoubleValue * b.DoubleValue);

			throw new VariantException("Cannot * variant type \"{0}\"", a.Type);
		}

		public static Variant operator /(Variant a, Variant b)
		{
			if (a.Type != b.Type)
				throw new VariantException("Variable types do not match ({0} != {1})", a.Type, b.Type);

			if (a.Type == VariantType.Int64)
				return new Variant(a.IntValue / b.IntValue);

			if (a.Type == VariantType.Double)
				return new Variant(a.DoubleValue / b.DoubleValue);

			throw new VariantException("Cannot / variant type \"{0}\"", a.Type);
		}

		public static Variant operator %(Variant a, Variant b)
		{
			if (a.Type != b.Type)
				throw new VariantException("Variable types do not match ({0} != {1})", a.Type, b.Type);

			if (a.Type == VariantType.Int64)
				return new Variant(a.IntValue % b.IntValue);

			if (a.Type == VariantType.Double)
				return new Variant(a.DoubleValue % b.DoubleValue);

			throw new VariantException("Cannot % variant type \"{0}\"", a.Type);
		}

		public static Variant operator -(Variant a)
		{
			if (a.Type == VariantType.Int64)
				return new Variant(-a.IntValue);

			if (a.Type == VariantType.Double)
				return new Variant(-a.DoubleValue);

			throw new VariantException("Cannot unary - variant type \"{0}\"", a.Type);
		}

		public static Variant operator +(Variant a)
		{
			if (a.Type == VariantType.Int64)
				return new Variant(Math.Abs(a.IntValue));

			if (a.Type == VariantType.Double)
				return new Variant(Math.Abs(a.DoubleValue));

			throw new VariantException("Cannot unary + variant type \"{0}\"", a.Type);
		}

		public static Variant operator ~(Variant a)
		{
			if (a.Type == VariantType.Int64)
				return new Variant(~a.IntValue);

			throw new VariantException("Cannot ~ variant type \"{0}\"", a.Type);
		}

		public static Variant operator !(Variant a)
		{
			if (a.Type == VariantType.Int64)
				return new Variant(a.IntValue == 0 ? 1 : 0);

			throw new VariantException("Cannot ! variant type \"{0}\"", a.Type);
		}

		public static Variant operator &(Variant a, Variant b)
		{
			if (a.Type != b.Type)
				throw new VariantException("Variable types do not match ({0} != {1})", a.Type, b.Type);

			if (a.Type == VariantType.Int64)
				return new Variant(a.IntValue & b.IntValue);

			throw new VariantException("Cannot & variant type \"{0}\"", a.Type);
		}

		public static Variant operator |(Variant a, Variant b)
		{
			if (a.Type != b.Type)
				throw new VariantException("Variable types do not match ({0} != {1})", a.Type, b.Type);

			if (a.Type == VariantType.Int64)
				return new Variant(a.IntValue | b.IntValue);

			throw new VariantException("Cannot | variant type \"{0}\"", a.Type);
		}

		public static Variant operator ^(Variant a, Variant b)
		{
			if (a.Type != b.Type)
				throw new VariantException("Variable types do not match ({0} != {1})", a.Type, b.Type);

			if (a.Type == VariantType.Int64)
				return new Variant(a.IntValue ^ b.IntValue);

			throw new VariantException("Cannot ^ variant type \"{0}\"", a.Type);
		}

		public static Variant operator >>(Variant a, int b)
		{
			if (a.Type == VariantType.Int64)
				return new Variant(a.IntValue >> b);

			throw new VariantException("Cannot >> variant type \"{0}\"", a.Type);
		}

		public static Variant operator <<(Variant a, int b)
		{
			if (a.Type == VariantType.Int64)
				return new Variant(a.IntValue << b);

			throw new VariantException("Cannot << variant type \"{0}\"", a.Type);
		}

		public static bool operator <(Variant a, Variant b)
		{
			if (a.Type == VariantType.Int64)
				return a.IntValue < b.IntValue;

			if (a.Type == VariantType.Double)
				return a.DoubleValue < b.DoubleValue;

			throw new VariantException("Cannot < variant type \"{0}\"", a.Type);
		}

		public static bool operator >(Variant a, Variant b)
		{
			if (a.Type == VariantType.Int64)
				return a.IntValue > b.IntValue;

			if (a.Type == VariantType.Double)
				return a.DoubleValue > b.DoubleValue;

			throw new VariantException("Cannot > variant type \"{0}\"", a.Type);
		}

		public static bool operator <=(Variant a, Variant b)
		{
			if (a.Type == VariantType.Int64)
				return a.IntValue <= b.IntValue;

			if (a.Type == VariantType.Double)
				return a.DoubleValue <= b.DoubleValue;

			throw new VariantException("Cannot <= variant type \"{0}\"", a.Type);
		}

		public static bool operator >=(Variant a, Variant b)
		{
			if (a.Type == VariantType.Int64)
				return a.IntValue >= b.IntValue;

			if (a.Type == VariantType.Double)
				return a.DoubleValue >= b.DoubleValue;

			throw new VariantException("Cannot >= variant type \"{0}\"", a.Type);
		}

		public static bool operator ==(Variant a, Variant b)
		{
			if (((object)a) == null || ((object)b) == null)
				return false;

			if (a.Type != b.Type)
				return false;

			switch (a.Type)
			{
				case VariantType.Int64:
					return a.IntValue == b.IntValue;

				case VariantType.Double:
					return Math.Abs(a.DoubleValue - b.DoubleValue) < Epsilon;

				case VariantType.String:
					return a.StringValue == b.StringValue;

				case VariantType.List:
				case VariantType.Object:
					return a.Equals(b.Type == VariantType.Nil ? null : b);

				case VariantType.Nil:
					return true;
			}

			return false;
		}

		public static bool operator !=(Variant a, Variant b)
		{
			if (((object)a) == null || ((object)b) == null)
				return false;

			if (a.Type != b.Type)
				return false;

			switch (a.Type)
			{
				case VariantType.Int64:
					return a.IntValue != b.IntValue;

				case VariantType.Double:
					return !(Math.Abs(a.DoubleValue - b.DoubleValue) < Epsilon);

				case VariantType.String:
					return a.StringValue != b.StringValue;

				case VariantType.List:
				case VariantType.Object:
					return !a.Equals(b);
			}

			return false;
		}
	}

	internal enum VariantType : byte
	{
		Nil,
		Int64,
		Double,
		String,
		List,
		Object
	}

	[Serializable]
	internal class VariantException : Exception
	{
		public VariantException(string message, params object[] args)
			: base(String.Format(message, args))
		{

		}

		public VariantException(string message, Exception innerException, params object[] args)
			: base(String.Format(message, args), innerException)
		{

		}
	}
}
