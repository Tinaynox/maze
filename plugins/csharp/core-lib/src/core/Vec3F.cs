using System;

namespace Maze.Core
{
    public struct Vec3F
    {
        public float X;
        public float Y;
        public float Z;

        public Vec2F XY => new Vec2F(X, Y);

        public Vec3F(
            float _x = 0.0f,
            float _y = 0.0f,
            float _z = 0.0f)
        {
            X = _x;
            Y = _y;
            Z = _z;
        }

        public Vec3F(
            Vec2F _vec,
            float _z = 0.0f)
        {
            X = _vec.X;
            Y = _vec.Y;
            Z = _z;
        }

        public static Vec3F operator -(Vec3F _vec0)
        {
            return new Vec3F(-_vec0.X, -_vec0.Y, -_vec0.Z);
        }

        public static Vec3F operator +(Vec3F _vec0, float _value)
        {
            return new Vec3F(_vec0.X + _value, _vec0.Y + _value, _vec0.Z + _value);
        }

        public static Vec3F operator +(Vec3F _vec0, Vec3F _vec1)
        {
            return new Vec3F(_vec0.X + _vec1.X, _vec0.Y + _vec1.Y, _vec0.Z + _vec1.Z);
        }

        public static Vec3F operator -(Vec3F _vec0, float _value)
        {
            return new Vec3F(_vec0.X - _value, _vec0.Y - _value, _vec0.Z - _value);
        }

        public static Vec3F operator -(Vec3F _vec0, Vec3F _vec1)
        {
            return new Vec3F(_vec0.X - _vec1.X, _vec0.Y - _vec1.Y, _vec0.Z - _vec1.Z);
        }

        public static Vec3F operator *(Vec3F _vec0, float _value)
        {
            return new Vec3F(_vec0.X * _value, _vec0.Y * _value, _vec0.Z * _value);
        }

        public static Vec3F operator *(Vec3F _vec0, Vec3F _vec1)
        {
            return new Vec3F(_vec0.X * _vec1.X, _vec0.Y * _vec1.Y, _vec0.Z * _vec1.Z);
        }

        public static Vec3F operator /(Vec3F _vec0, float _value)
        {
            return new Vec3F(_vec0.X / _value, _vec0.Y / _value, _vec0.Z / _value);
        }

        public static Vec3F operator /(Vec3F _vec0, Vec3F _vec1)
        {
            return new Vec3F(_vec0.X / _vec1.X, _vec0.Y / _vec1.Y, _vec0.Z / _vec1.Z);
        }
    }
}
