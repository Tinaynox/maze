using System;

namespace Maze.Core
{
    public struct Vec4F
    {
        public float X;
        public float Y;
        public float Z;
        public float W;

        public static Vec4F Zero = new Vec4F(0.0f, 0.0f, 0.0f, 1.0f);
        public static Vec4F One => new Vec4F(1.0f, 1.0f, 1.0f, 1.0f);
        public static Vec4F NegativeOne => new Vec4F(-1.0f, -1.0f, -1.0f, -1.0f);
        public static Vec4F UnitX => new Vec4F(1.0f, 0.0f, 0.0f, 1.0f);
        public static Vec4F UnitY => new Vec4F(0.0f, 1.0f, 0.0f, 1.0f);
        public static Vec4F UnitZ => new Vec4F(0.0f, 0.0f, 1.0f, 1.0f);
        public static Vec4F NegativeUnitX => new Vec4F(-1.0f, 0.0f, 0.0f, 1.0f);
        public static Vec4F NegativeUnitY => new Vec4F(0.0f, -1.0f, 0.0f, 1.0f);
        public static Vec4F NegativeUnitZ => new Vec4F(0.0f, 0.0f, -1.0f, 1.0f);

        public Vec2F XY => new Vec2F(X, Y);
        public Vec3F XYZ => new Vec3F(X, Y, Z);


        public Vec4F(
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f,
            float w = 0.0f)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vec4F(
            Vec3F vec,
            float w = 0.0f)
        {
            X = vec.X;
            Y = vec.Y;
            Z = vec.Z;
            W = w;
        }

        public Vec4F(
            Vec2F vec,
            float z = 0.0f,
            float w = 0.0f)
        {
            X = vec.X;
            Y = vec.Y;
            Z = z;
            W = w;
        }

        public float Length()
        {
            return (float)Math.Sqrt(X * X + Y * Y + Z * Z + W * W);
        }

        public float LengthSq()
        {
            return X * X + Y * Y + Z * Z + W * W;
        }

        public float Distance(Vec4F vec)
        {
            return (this - vec).Length();
        }

        public float DistanceSq(Vec4F vec)
        {
            return (this - vec).LengthSq();
        }
        public static Vec4F operator -(Vec4F vec0)
        {
            return new Vec4F(-vec0.X, -vec0.Y, -vec0.Z, -vec0.W);
        }

        public static Vec4F operator +(Vec4F vec0, float value)
        {
            return new Vec4F(vec0.X + value, vec0.Y + value, vec0.Z + value, vec0.W + value);
        }

        public static Vec4F operator +(Vec4F vec0, Vec4F vec1)
        {
            return new Vec4F(vec0.X + vec1.X, vec0.Y + vec1.Y, vec0.Z + vec1.Z, vec0.W + vec1.W);
        }

        public static Vec4F operator -(Vec4F vec0, float value)
        {
            return new Vec4F(vec0.X - value, vec0.Y - value, vec0.Z - value, vec0.W - value);
        }

        public static Vec4F operator -(Vec4F vec0, Vec4F vec1)
        {
            return new Vec4F(vec0.X - vec1.X, vec0.Y - vec1.Y, vec0.Z - vec1.Z, vec0.W - vec1.W);
        }

        public static Vec4F operator *(Vec4F vec0, float value)
        {
            return new Vec4F(vec0.X * value, vec0.Y * value, vec0.Z * value, vec0.W * value);
        }

        public static Vec4F operator *(Vec4F vec0, Vec4F vec1)
        {
            return new Vec4F(vec0.X * vec1.X, vec0.Y * vec1.Y, vec0.Z * vec1.Z, vec0.W * vec1.W);
        }

        public static Vec4F operator /(Vec4F vec0, float value)
        {
            return new Vec4F(vec0.X / value, vec0.Y / value, vec0.Z / value, vec0.W / value);
        }

        public static Vec4F operator /(Vec4F vec0, Vec4F vec1)
        {
            return new Vec4F(vec0.X / vec1.X, vec0.Y / vec1.Y, vec0.Z / vec1.Z, vec0.W / vec1.W);
        }
    }
}
