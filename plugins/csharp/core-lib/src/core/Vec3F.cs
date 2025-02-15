using System;

namespace Maze.Core
{
    public struct Vec3F
    {
        public float X;
        public float Y;
        public float Z;

        public static Vec3F Zero => new Vec3F(0.0f, 0.0f, 0.0f);
        public static Vec3F One => new Vec3F(1.0f, 1.0f, 1.0f);
        public static Vec3F NegativeOne => new Vec3F(-1.0f, -1.0f, -1.0f);
        public static Vec3F UnitX => new Vec3F(1.0f, 0.0f, 0.0f);
        public static Vec3F UnitY => new Vec3F(0.0f, 1.0f, 0.0f);
        public static Vec3F UnitZ => new Vec3F(0.0f, 0.0f, 1.0f);
        public static Vec3F NegativeUnitX => new Vec3F(-1.0f, 0.0f, 0.0f);
        public static Vec3F NegativeUnitY => new Vec3F(0.0f, -1.0f, 0.0f);
        public static Vec3F NegativeUnitZ => new Vec3F(0.0f, 0.0f, -1.0f);

        public Vec2F XY => new Vec2F(X, Y);

        public Vec3F(
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vec3F(
            Vec2F vec,
            float z = 0.0f)
        {
            X = vec.X;
            Y = vec.Y;
            Z = z;
        }

        public float Length()
        {
            return (float)Math.Sqrt(X * X + Y * Y + Z * Z);
        }

        public float LengthSq()
        {
            return X * X + Y * Y + Z * Z;
        }

        public float Distance(Vec3F vec)
        {
            return (this - vec).Length();
        }

        public float DistanceSq(Vec3F vec)
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
                Z *= invLength;
            }

            return length;
        }

        public Vec3F NormalizedCopy()
        {
            Vec3F ret = this;
            ret.Normalize();
            return ret;
        }

        public float Dot(Vec3F vec)
        {
            return X * vec.X + Y * vec.Y + Z * vec.Z;
        }

        public Vec3F Cross(Vec3F vec)
        { 
           return new Vec3F(
               Y * vec.Z - Z * vec.Y,
               Z * vec.X - X * vec.Z,
               X * vec.Y - Y * vec.X);
        }

        public static Vec3F operator -(Vec3F vec0)
        {
            return new Vec3F(-vec0.X, -vec0.Y, -vec0.Z);
        }

        public static Vec3F operator +(Vec3F vec0, float value)
        {
            return new Vec3F(vec0.X + value, vec0.Y + value, vec0.Z + value);
        }

        public static Vec3F operator +(Vec3F vec0, Vec3F vec1)
        {
            return new Vec3F(vec0.X + vec1.X, vec0.Y + vec1.Y, vec0.Z + vec1.Z);
        }

        public static Vec3F operator -(Vec3F vec0, float value)
        {
            return new Vec3F(vec0.X - value, vec0.Y - value, vec0.Z - value);
        }

        public static Vec3F operator -(Vec3F vec0, Vec3F vec1)
        {
            return new Vec3F(vec0.X - vec1.X, vec0.Y - vec1.Y, vec0.Z - vec1.Z);
        }

        public static Vec3F operator *(Vec3F vec0, float value)
        {
            return new Vec3F(vec0.X * value, vec0.Y * value, vec0.Z * value);
        }

        public static Vec3F operator *(float value, Vec3F vec0)
        {
            return new Vec3F(vec0.X * value, vec0.Y * value, vec0.Z * value);
        }

        public static Vec3F operator *(Vec3F vec0, Vec3F vec1)
        {
            return new Vec3F(vec0.X * vec1.X, vec0.Y * vec1.Y, vec0.Z * vec1.Z);
        }

        public static Vec3F operator /(Vec3F vec0, float value)
        {
            return new Vec3F(vec0.X / value, vec0.Y / value, vec0.Z / value);
        }

        public static Vec3F operator /(Vec3F vec0, Vec3F vec1)
        {
            return new Vec3F(vec0.X / vec1.X, vec0.Y / vec1.Y, vec0.Z / vec1.Z);
        }
    }
}
