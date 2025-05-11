using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec4S8
    {
        public sbyte X;
        public sbyte Y;
        public sbyte Z;
        public sbyte W;

        public static Vec4S8 Zero => new Vec4S8(0, 0, 0, 0);
        public static Vec4S8 One => new Vec4S8(1, 1, 1, 1);
        public static Vec4S8 UnitX => new Vec4S8(1, 0, 0, 0);
        public static Vec4S8 UnitY => new Vec4S8(0, 1, 0, 0);
        public static Vec4S8 UnitZ => new Vec4S8(0, 0, 1, 0);
        public static Vec4S8 UnitW => new Vec4S8(0, 0, 0, 1);

        public Vec4S8(
            sbyte x = 0,
            sbyte y = 0,
            sbyte z = 0,
            sbyte w = 0)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vec4S8(
            int x = 0,
            int y = 0,
            int z = 0,
            int w = 0)
        {
            X = (sbyte)x;
            Y = (sbyte)y;
            Z = (sbyte)z;
            W = (sbyte)w;
        }

        public static Vec4S8 operator +(Vec4S8 vec0, sbyte value)
        {
            return new Vec4S8(vec0.X + value, vec0.Y + value, vec0.Z + value, vec0.W + value);
        }

        public static Vec4S8 operator +(Vec4S8 vec0, Vec4S8 vec1)
        {
            return new Vec4S8(vec0.X + vec1.X, vec0.Y + vec1.Y, vec0.Z + vec1.Z, vec0.W + vec1.W);
        }

        public static Vec4S8 operator -(Vec4S8 vec0, sbyte value)
        {
            return new Vec4S8(vec0.X - value, vec0.Y - value, vec0.Z - value, vec0.W - value);
        }

        public static Vec4S8 operator -(Vec4S8 vec0, Vec4S8 vec1)
        {
            return new Vec4S8(vec0.X - vec1.X, vec0.Y - vec1.Y, vec0.Z - vec1.Z, vec0.W - vec1.W);
        }

        public static Vec4S8 operator *(Vec4S8 vec0, sbyte value)
        {
            return new Vec4S8(vec0.X * value, vec0.Y * value, vec0.Z * value, vec0.W * value);
        }

        public static Vec4S8 operator *(Vec4S8 vec0, Vec4S8 vec1)
        {
            return new Vec4S8(vec0.X * vec1.X, vec0.Y * vec1.Y, vec0.Z * vec1.Z, vec0.W * vec1.W);
        }

        public static Vec4S8 operator /(Vec4S8 vec0, sbyte value)
        {
            return new Vec4S8(vec0.X / value, vec0.Y / value, vec0.Z / value, vec0.W / value);
        }

        public static Vec4S8 operator /(Vec4S8 vec0, Vec4S8 vec1)
        {
            return new Vec4S8(vec0.X / vec1.X, vec0.Y / vec1.Y, vec0.Z / vec1.Z, vec0.W / vec1.W);
        }

        public byte[] GetBytes()
        {
            return new byte[] { (byte)X, (byte)Y, (byte)Z, (byte)W };
        }

        public static Vec4S8 FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 4)
                throw new ArgumentException($"Byte array must contain at least 4 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Vec4S8
            {
                X = (sbyte)bytes[startIndex + 0],
                Y = (sbyte)bytes[startIndex + 1],
                Z = (sbyte)bytes[startIndex + 2],
                W = (sbyte)bytes[startIndex + 3]
            };
        }

        public static int ParseString(string str, int start, int end, out Vec4S8 outValue, char separator = ',')
        {
            start = StringHelper.ParseInteger(str, start, end, out sbyte x); outValue.X = x;
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseInteger(str, start, end, out sbyte y); outValue.Y = y;
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseInteger(str, start, end, out sbyte z); outValue.Z = z;
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseInteger(str, start, end, out sbyte w); outValue.W = w;

            return start;
        }

        public static int ParseString(string str, out Vec4S8 outValue, char separator = ',')
        {
            return ParseString(str, 0, str.Length, out outValue, separator);
        }

        public static int ParseString(List<byte> str, int start, int end, out Vec4S8 outValue, byte separator = (byte)',')
        {
            start = StringHelper.ParseInteger(str, start, end, out sbyte x); outValue.X = x;
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseInteger(str, start, end, out sbyte y); outValue.Y = y;
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseInteger(str, start, end, out sbyte z); outValue.Z = z;
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseInteger(str, start, end, out sbyte w); outValue.W = w;

            return start;
        }

        public static int ParseString(List<byte> str, out Vec4S8 outValue, byte separator = (byte)',')
        {
            return ParseString(str, 0, str.Count, out outValue, separator);
        }

        public override string ToString()
        {
            return $"[{X}, {Y}, {Z}, {W}]";
        }
    }
}
