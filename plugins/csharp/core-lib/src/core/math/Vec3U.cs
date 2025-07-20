using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec3U
    {
        public uint X;
        public uint Y;
        public uint Z;

        public static Vec3U Zero => new Vec3U(0, 0, 0);
        public static Vec3U One => new Vec3U(1, 1, 1);
        public static Vec3U UnitX => new Vec3U(1, 0, 0);
        public static Vec3U UnitY => new Vec3U(0, 1, 0);
        public static Vec3U UnitZ => new Vec3U(0, 0, 1);

        public Vec3U(
            uint x = 0,
            uint y = 0,
            uint z = 0)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public uint this[int index]
        {
            get
            {
                if (index == 0)
                    return X;
                else if (index == 1)
                    return Y;
                else if (index == 2)
                    return Z;
                else
                    throw new IndexOutOfRangeException("Index must be between 0 and 2.");
            }
        }

        public static Vec3U operator +(Vec3U vec0, uint value)
        {
            return new Vec3U(vec0.X + value, vec0.Y + value, vec0.Z + value);
        }

        public static Vec3U operator +(Vec3U vec0, Vec3U vec1)
        {
            return new Vec3U(vec0.X + vec1.X, vec0.Y + vec1.Y, vec0.Z + vec1.Z);
        }

        public static Vec3U operator -(Vec3U vec0, uint value)
        {
            return new Vec3U(vec0.X - value, vec0.Y - value, vec0.Z - value);
        }

        public static Vec3U operator -(Vec3U vec0, Vec3U vec1)
        {
            return new Vec3U(vec0.X - vec1.X, vec0.Y - vec1.Y, vec0.Z - vec1.Z);
        }

        public static Vec3U operator*(Vec3U vec0, uint value)
        {
            return new Vec3U(vec0.X * value, vec0.Y * value, vec0.Z * value);
        }

        public static Vec3U operator *(Vec3U vec0, Vec3U vec1)
        {
            return new Vec3U(vec0.X * vec1.X, vec0.Y * vec1.Y, vec0.Z * vec1.Z);
        }

        public static Vec3U operator /(Vec3U vec0, uint value)
        {
            return new Vec3U(vec0.X / value, vec0.Y / value, vec0.Z / value);
        }

        public static Vec3U operator /(Vec3U vec0, Vec3U vec1)
        {
            return new Vec3U(vec0.X / vec1.X, vec0.Y / vec1.Y, vec0.Z / vec1.Z);
        }

        public static bool operator ==(Vec3U value0, Vec3U value1)
        {
            return value0.X == value1.X && value0.Y == value1.Y && value0.Z == value1.Z;
        }

        public static bool operator !=(Vec3U value0, Vec3U value1)
        {
            return value0.X != value1.X || value0.Y != value1.Y || value0.Z != value1.Z;
        }

        public override bool Equals(object obj)
        {
            if (obj is Vec3U other)
                return this == other;
            return false;
        }

        public override int GetHashCode()
        {
            unchecked // Handle arithmetic overflow
            {
                int hash = 17;
                hash = hash * 23 + X.GetHashCode();
                hash = hash * 23 + Y.GetHashCode();
                hash = hash * 23 + Z.GetHashCode();
                return hash;
            }
        }

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[12];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 4);
            BitConverter.GetBytes(Z).CopyTo(bytes, 8);
            return bytes;
        }

        public static Vec3U FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 12)
                throw new ArgumentException($"Byte array must contain at least 12 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Vec3U
            {
                X = BitConverter.ToUInt32(bytes, startIndex + 0),
                Y = BitConverter.ToUInt32(bytes, startIndex + 4),
                Z = BitConverter.ToUInt32(bytes, startIndex + 8)
            };
        }

        public static int ParseString(string str, int start, int end, out Vec3U outValue, char separator = ',')
        {
            start = StringHelper.ParseInteger(str, start, end, out uint x); outValue.X = x;
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseInteger(str, start, end, out uint y); outValue.Y = y;
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseInteger(str, start, end, out uint z); outValue.Z = z;

            return start;
        }

        public static int ParseString(string str, out Vec3U outValue, char separator = ',')
        {
            return ParseString(str, 0, str.Length, out outValue, separator);
        }

        public static int ParseString(List<byte> str, int start, int end, out Vec3U outValue, byte separator = (byte)',')
        {
            start = StringHelper.ParseInteger(str, start, end, out uint x); outValue.X = x;
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseInteger(str, start, end, out uint y); outValue.Y = y;
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseInteger(str, start, end, out uint z); outValue.Z = z;

            return start;
        }

        public static int ParseString(List<byte> str, out Vec3U outValue, byte separator = (byte)',')
        {
            return ParseString(str, 0, str.Count, out outValue, separator);
        }

        public override string ToString()
        {
            return $"{X}, {Y}, {Z}";
        }
    }
}
