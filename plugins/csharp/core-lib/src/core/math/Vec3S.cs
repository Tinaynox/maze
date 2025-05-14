using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec3S
    {
        public int X;
        public int Y;
        public int Z;

        public static Vec3S Zero => new Vec3S(0, 0, 0);
        public static Vec3S One => new Vec3S(1, 1, 1);
        public static Vec3S UnitX => new Vec3S(1, 0, 0);
        public static Vec3S UnitY => new Vec3S(0, 1, 0);
        public static Vec3S UnitZ => new Vec3S(0, 0, 1);

        public Vec3S(
            int x = 0,
            int y = 0,
            int z = 0)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public int this[int index]
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

        public static Vec3S operator +(Vec3S vec0, int value)
        {
            return new Vec3S(vec0.X + value, vec0.Y + value, vec0.Z + value);
        }

        public static Vec3S operator +(Vec3S vec0, Vec3S vec1)
        {
            return new Vec3S(vec0.X + vec1.X, vec0.Y + vec1.Y, vec0.Z + vec1.Z);
        }

        public static Vec3S operator -(Vec3S vec0, int value)
        {
            return new Vec3S(vec0.X - value, vec0.Y - value, vec0.Z - value);
        }

        public static Vec3S operator -(Vec3S vec0, Vec3S vec1)
        {
            return new Vec3S(vec0.X - vec1.X, vec0.Y - vec1.Y, vec0.Z - vec1.Z);
        }

        public static Vec3S operator *(Vec3S vec0, int value)
        {
            return new Vec3S(vec0.X * value, vec0.Y * value, vec0.Z * value);
        }

        public static Vec3S operator *(Vec3S vec0, Vec3S vec1)
        {
            return new Vec3S(vec0.X * vec1.X, vec0.Y * vec1.Y, vec0.Z * vec1.Z);
        }

        public static Vec3S operator /(Vec3S vec0, int value)
        {
            return new Vec3S(vec0.X / value, vec0.Y / value, vec0.Z / value);
        }

        public static Vec3S operator /(Vec3S vec0, Vec3S vec1)
        {
            return new Vec3S(vec0.X / vec1.X, vec0.Y / vec1.Y, vec0.Z / vec1.Z);
        }

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[12];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 4);
            BitConverter.GetBytes(Z).CopyTo(bytes, 8);
            return bytes;
        }

        public static Vec3S FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 12)
                throw new ArgumentException($"Byte array must contain at least 12 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Vec3S
            {
                X = BitConverter.ToInt32(bytes, startIndex + 0),
                Y = BitConverter.ToInt32(bytes, startIndex + 4),
                Z = BitConverter.ToInt32(bytes, startIndex + 8)
            };
        }

        public static int ParseString(string str, int start, int end, out Vec3S outValue, char separator = ',')
        {
            start = StringHelper.ParseInteger(str, start, end, out int x); outValue.X = x;
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseInteger(str, start, end, out int y); outValue.Y = y;
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseInteger(str, start, end, out int z); outValue.Z = z;

            return start;
        }

        public static int ParseString(string str, out Vec3S outValue, char separator = ',')
        {
            return ParseString(str, 0, str.Length, out outValue, separator);
        }

        public static int ParseString(List<byte> str, int start, int end, out Vec3S outValue, byte separator = (byte)',')
        {
            start = StringHelper.ParseInteger(str, start, end, out int x); outValue.X = x;
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseInteger(str, start, end, out int y); outValue.Y = y;
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseInteger(str, start, end, out int z); outValue.Z = z;

            return start;
        }

        public static int ParseString(List<byte> str, out Vec3S outValue, byte separator = (byte)',')
        {
            return ParseString(str, 0, str.Count, out outValue, separator);
        }

        public override string ToString()
        {
            return $"{X}, {Y}, {Z}";
        }
    }
}
