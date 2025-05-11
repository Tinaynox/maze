using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec2S
    {
        public int X;
        public int Y;

        public static Vec2S Zero => new Vec2S(0, 0);
        public static Vec2S One => new Vec2S(1, 1);
        public static Vec2S UnitX => new Vec2S(1, 0);
        public static Vec2S UnitY => new Vec2S(0, 1);

        public Vec2S(
            int x = 0,
            int y = 0)
        {
            X = x;
            Y = y;
        }

        public static Vec2S operator +(Vec2S vec0, int value)
        {
            return new Vec2S(vec0.X + value, vec0.Y + value);
        }

        public static Vec2S operator +(Vec2S vec0, Vec2S vec1)
        {
            return new Vec2S(vec0.X + vec1.X, vec0.Y + vec1.Y);
        }

        public static Vec2S operator -(Vec2S vec0, int value)
        {
            return new Vec2S(vec0.X - value, vec0.Y - value);
        }

        public static Vec2S operator -(Vec2S vec0, Vec2S vec1)
        {
            return new Vec2S(vec0.X - vec1.X, vec0.Y - vec1.Y);
        }

        public static Vec2S operator*(Vec2S vec0, int value)
        {
            return new Vec2S(vec0.X * value, vec0.Y * value);
        }

        public static Vec2S operator *(Vec2S vec0, Vec2S vec1)
        {
            return new Vec2S(vec0.X * vec1.X, vec0.Y * vec1.Y);
        }

        public static Vec2S operator /(Vec2S vec0, int value)
        {
            return new Vec2S(vec0.X / value, vec0.Y / value);
        }

        public static Vec2S operator /(Vec2S vec0, Vec2S vec1)
        {
            return new Vec2S(vec0.X / vec1.X, vec0.Y / vec1.Y);
        }

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[8];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 4);
            return bytes;
        }

        public static Vec2S FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 8)
                throw new ArgumentException($"Byte array must contain at least 8 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Vec2S
            {
                X = BitConverter.ToInt32(bytes, startIndex + 0),
                Y = BitConverter.ToInt32(bytes, startIndex + 4)
            };
        }

        public static int ParseString(string str, int start, int end, out Vec2S outValue, char separator = ',')
        {
            start = StringHelper.ParseInteger(str, start, end, out int x); outValue.X = x;
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseInteger(str, start, end, out int y); outValue.Y = y;

            return start;
        }

        public static int ParseString(string str, out Vec2S outValue, char separator = ',')
        {
            return ParseString(str, 0, str.Length, out outValue, separator);
        }

        public static int ParseString(List<byte> str, int start, int end, out Vec2S outValue, byte separator = (byte)',')
        {
            start = StringHelper.ParseInteger(str, start, end, out int x); outValue.X = x;
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseInteger(str, start, end, out int y); outValue.Y = y;

            return start;
        }

        public static int ParseString(List<byte> str, out Vec2S outValue, byte separator = (byte)',')
        {
            return ParseString(str, 0, str.Count, out outValue, separator);
        }

        public override string ToString()
        {
            return $"[{X}, {Y}]";
        }
    }
}
