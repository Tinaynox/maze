using System;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec4U8
    {
        public byte X;
        public byte Y;
        public byte Z;
        public byte W;

        public static Vec4U8 Zero => new Vec4U8(0, 0, 0, 0);
        public static Vec4U8 One => new Vec4U8(1, 1, 1, 1);
        public static Vec4U8 UnitX => new Vec4U8(1, 0, 0, 0);
        public static Vec4U8 UnitY => new Vec4U8(0, 1, 0, 0);
        public static Vec4U8 UnitZ => new Vec4U8(0, 0, 1, 0);
        public static Vec4U8 UnitW => new Vec4U8(0, 0, 0, 1);

        public Vec4U8(
            byte x = 0,
            byte y = 0,
            byte z = 0,
            byte w = 0)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vec4U8(
            int x = 0,
            int y = 0,
            int z = 0,
            int w = 0)
        {
            X = (byte)x;
            Y = (byte)y;
            Z = (byte)z;
            W = (byte)w;
        }

        public static Vec4U8 operator +(Vec4U8 vec0, byte value)
        {
            return new Vec4U8(vec0.X + value, vec0.Y + value, vec0.Z + value, vec0.W + value);
        }

        public static Vec4U8 operator +(Vec4U8 vec0, Vec4U8 vec1)
        {
            return new Vec4U8(vec0.X + vec1.X, vec0.Y + vec1.Y, vec0.Z + vec1.Z, vec0.W + vec1.W);
        }

        public static Vec4U8 operator -(Vec4U8 vec0, byte value)
        {
            return new Vec4U8(vec0.X - value, vec0.Y - value, vec0.Z - value, vec0.W - value);
        }

        public static Vec4U8 operator -(Vec4U8 vec0, Vec4U8 vec1)
        {
            return new Vec4U8(vec0.X - vec1.X, vec0.Y - vec1.Y, vec0.Z - vec1.Z, vec0.W - vec1.W);
        }

        public static Vec4U8 operator *(Vec4U8 vec0, byte value)
        {
            return new Vec4U8(vec0.X * value, vec0.Y * value, vec0.Z * value, vec0.W * value);
        }

        public static Vec4U8 operator *(Vec4U8 vec0, Vec4U8 vec1)
        {
            return new Vec4U8(vec0.X * vec1.X, vec0.Y * vec1.Y, vec0.Z * vec1.Z, vec0.W * vec1.W);
        }

        public static Vec4U8 operator /(Vec4U8 vec0, byte value)
        {
            return new Vec4U8(vec0.X / value, vec0.Y / value, vec0.Z / value, vec0.W / value);
        }

        public static Vec4U8 operator /(Vec4U8 vec0, Vec4U8 vec1)
        {
            return new Vec4U8(vec0.X / vec1.X, vec0.Y / vec1.Y, vec0.Z / vec1.Z, vec0.W / vec1.W);
        }

        public byte[] GetBytes()
        {
            return new byte[] { X, Y, Z, W };
        }

        public static Vec4U8 FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 4)
                throw new ArgumentException("Byte array must contain at least 4 bytes");

            return new Vec4U8
            {
                X = bytes[startIndex + 0],
                Y = bytes[startIndex + 1],
                Z = bytes[startIndex + 2],
                W = bytes[startIndex + 3]
            };
        }

        public override string ToString()
        {
            return $"[{X}, {Y}, {Z}, {W}]";
        }
    }
}
