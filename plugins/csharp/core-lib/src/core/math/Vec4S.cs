using System;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec4S
    {
        public int X;
        public int Y;
        public int Z;
        public int W;

        public static Vec4S Zero => new Vec4S(0, 0, 0, 0);
        public static Vec4S One => new Vec4S(1, 1, 1, 1);
        public static Vec4S UnitX => new Vec4S(1, 0, 0, 0);
        public static Vec4S UnitY => new Vec4S(0, 1, 0, 0);
        public static Vec4S UnitZ => new Vec4S(0, 0, 1, 0);
        public static Vec4S UnitW => new Vec4S(0, 0, 0, 1);

        public Vec4S(
            int x = 0,
            int y = 0,
            int z = 0,
            int w = 0)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
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
                else if (index == 3)
                    return W;
                else
                    throw new IndexOutOfRangeException("Index must be between 0 and 3.");
            }
        }

        public static Vec4S operator +(Vec4S vec0, int value)
        {
            return new Vec4S(vec0.X + value, vec0.Y + value, vec0.Z + value, vec0.W + value);
        }

        public static Vec4S operator +(Vec4S vec0, Vec4S vec1)
        {
            return new Vec4S(vec0.X + vec1.X, vec0.Y + vec1.Y, vec0.Z + vec1.Z, vec0.W + vec1.W);
        }

        public static Vec4S operator -(Vec4S vec0, int value)
        {
            return new Vec4S(vec0.X - value, vec0.Y - value, vec0.Z - value, vec0.W - value);
        }

        public static Vec4S operator -(Vec4S vec0, Vec4S vec1)
        {
            return new Vec4S(vec0.X - vec1.X, vec0.Y - vec1.Y, vec0.Z - vec1.Z, vec0.W - vec1.W);
        }

        public static Vec4S operator *(Vec4S vec0, int value)
        {
            return new Vec4S(vec0.X * value, vec0.Y * value, vec0.Z * value, vec0.W * value);
        }

        public static Vec4S operator *(Vec4S vec0, Vec4S vec1)
        {
            return new Vec4S(vec0.X * vec1.X, vec0.Y * vec1.Y, vec0.Z * vec1.Z, vec0.W * vec1.W);
        }

        public static Vec4S operator /(Vec4S vec0, int value)
        {
            return new Vec4S(vec0.X / value, vec0.Y / value, vec0.Z / value, vec0.W / value);
        }

        public static Vec4S operator /(Vec4S vec0, Vec4S vec1)
        {
            return new Vec4S(vec0.X / vec1.X, vec0.Y / vec1.Y, vec0.Z / vec1.Z, vec0.W / vec1.W);
        }

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[16];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 4);
            BitConverter.GetBytes(Z).CopyTo(bytes, 8);
            BitConverter.GetBytes(W).CopyTo(bytes, 12);
            return bytes;
        }

        public static Vec4S FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 16)
                throw new ArgumentException($"Byte array must contain at least 16 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Vec4S
            {
                X = BitConverter.ToInt32(bytes, startIndex + 0),
                Y = BitConverter.ToInt32(bytes, startIndex + 4),
                Z = BitConverter.ToInt32(bytes, startIndex + 8),
                W = BitConverter.ToInt32(bytes, startIndex + 12)
            };
        }

        public override string ToString()
        {
            return $"[{X}, {Y}, {Z}, {W}]";
        }
    }
}
