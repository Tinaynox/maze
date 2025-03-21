using System;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec2F
    {
        public float X;
        public float Y;

        public static Vec2F Zero => new Vec2F(0.0f, 0.0f);
        public static Vec2F One => new Vec2F(1.0f, 1.0f);
        public static Vec2F NegativeOne => new Vec2F(-1.0f, -1.0f);
        public static Vec2F UnitX => new Vec2F(1.0f, 0.0f);
        public static Vec2F UnitY => new Vec2F(0.0f, 1.0f);
        public static Vec2F NegativeUnitX => new Vec2F(-1.0f, 0.0f);
        public static Vec2F NegativeUnitY => new Vec2F(0.0f, -1.0f);

        public Vec2F(
            float x = 0.0f,
            float y = 0.0f)
        {
            X = x;
            Y = y;
        }

        public static Vec2F operator -(Vec2F vec0)
        {
            return new Vec2F(-vec0.X, -vec0.Y);
        }

        public static Vec2F operator +(Vec2F vec0, float value)
        {
            return new Vec2F(vec0.X + value, vec0.Y + value);
        }

        public static Vec2F operator +(Vec2F vec0, Vec2F vec1)
        {
            return new Vec2F(vec0.X + vec1.X, vec0.Y + vec1.Y);
        }

        public static Vec2F operator -(Vec2F vec0, float value)
        {
            return new Vec2F(vec0.X - value, vec0.Y - value);
        }

        public static Vec2F operator -(Vec2F vec0, Vec2F vec1)
        {
            return new Vec2F(vec0.X - vec1.X, vec0.Y - vec1.Y);
        }

        public static Vec2F operator*(Vec2F vec0, float value)
        {
            return new Vec2F(vec0.X * value, vec0.Y * value);
        }

        public static Vec2F operator *(Vec2F vec0, Vec2F vec1)
        {
            return new Vec2F(vec0.X * vec1.X, vec0.Y * vec1.Y);
        }

        public static Vec2F operator /(Vec2F vec0, float value)
        {
            return new Vec2F(vec0.X / value, vec0.Y / value);
        }

        public static Vec2F operator /(Vec2F vec0, Vec2F vec1)
        {
            return new Vec2F(vec0.X / vec1.X, vec0.Y / vec1.Y);
        }

        public float Length()
        {
            return (float)Math.Sqrt(X * X + Y * Y);
        }

        public float LengthSq()
        {
            return X * X + Y * Y;
        }

        public float Distance(Vec2F vec)
        {
            return (this - vec).Length();
        }

        public float DistanceSq(Vec2F vec)
        {
            return (this - vec).LengthSq();
        }

        public float Normalize()
        {
            float length = Length();

            if (length > 1e-08f)
            {
                float invLength = 1.0f / length;
                X *= invLength;
                Y *= invLength;
            }

            return length;
        }

        public Vec2F NormalizedCopy()
        {
            Vec2F ret = this;
            ret.Normalize();
            return ret;
        }

        public float Dot(Vec2F vec)
        {
            return X * vec.X + Y * vec.Y;
        }

        public float Cross(Vec2F vec)
        {
            return X * vec.Y - Y * vec.X;
        }

        public Vec2F Reflect(Vec2F normal)
        {
            return this - (normal * Dot(normal) * 2.0f);
        }

        public Vec2F RandomDirection()
        {
            Random random = new Random();

            double angle = 2.0 * Math.PI * random.NextDouble();
            return new Vec2F((float)Math.Cos(angle), (float)Math.Sin(angle));
        }

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[8];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 4);
            return bytes;
        }

        public static Vec2F FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 8)
                throw new ArgumentException("Byte array must contain at least 8 bytes");

            return new Vec2F
            {
                X = BitConverter.ToSingle(bytes, startIndex + 0),
                Y = BitConverter.ToSingle(bytes, startIndex + 4)
            };
        }

        public override string ToString()
        {
            return $"[{X}, {Y}]";
        }
    }
}
