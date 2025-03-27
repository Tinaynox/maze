using System;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec2U
    {
        public uint X;
        public uint Y;

        public static Vec2U Zero => new Vec2U(0, 0);
        public static Vec2U One => new Vec2U(1, 1);
        public static Vec2U UnitX => new Vec2U(1, 0);
        public static Vec2U UnitY => new Vec2U(0, 1);

        public Vec2U(
            uint x = 0,
            uint y = 0)
        {
            X = x;
            Y = y;
        }

        public static Vec2U operator +(Vec2U vec0, uint value)
        {
            return new Vec2U(vec0.X + value, vec0.Y + value);
        }

        public static Vec2U operator +(Vec2U vec0, Vec2U vec1)
        {
            return new Vec2U(vec0.X + vec1.X, vec0.Y + vec1.Y);
        }

        public static Vec2U operator -(Vec2U vec0, uint value)
        {
            return new Vec2U(vec0.X - value, vec0.Y - value);
        }

        public static Vec2U operator -(Vec2U vec0, Vec2U vec1)
        {
            return new Vec2U(vec0.X - vec1.X, vec0.Y - vec1.Y);
        }

        public static Vec2U operator*(Vec2U vec0, uint value)
        {
            return new Vec2U(vec0.X * value, vec0.Y * value);
        }

        public static Vec2U operator *(Vec2U vec0, Vec2U vec1)
        {
            return new Vec2U(vec0.X * vec1.X, vec0.Y * vec1.Y);
        }

        public static Vec2U operator /(Vec2U vec0, uint value)
        {
            return new Vec2U(vec0.X / value, vec0.Y / value);
        }

        public static Vec2U operator /(Vec2U vec0, Vec2U vec1)
        {
            return new Vec2U(vec0.X / vec1.X, vec0.Y / vec1.Y);
        }

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[8];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 4);
            return bytes;
        }

        public static Vec2U FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 8)
                throw new ArgumentException($"Byte array must contain at least 8 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Vec2U
            {
                X = BitConverter.ToUInt32(bytes, startIndex + 0),
                Y = BitConverter.ToUInt32(bytes, startIndex + 4)
            };
        }

        public override string ToString()
        {
            return $"[{X}, {Y}]";
        }
    }
}
