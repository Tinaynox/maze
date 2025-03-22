using System;
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
                throw new ArgumentException("Byte array must contain at least 12 bytes");

            return new Vec3U
            {
                X = BitConverter.ToUInt32(bytes, startIndex + 0),
                Y = BitConverter.ToUInt32(bytes, startIndex + 4),
                Z = BitConverter.ToUInt32(bytes, startIndex + 8)
            };
        }

        public override string ToString()
        {
            return $"[{X}, {Y}, {Z}]";
        }
    }
}
