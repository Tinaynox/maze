using System;

namespace Maze.Core
{
    public struct Vec4F
    {
        public float X;
        public float Y;
        public float Z;
        public float W;

        public Vec2F XY => new Vec2F(X, Y);
        public Vec3F XYZ => new Vec3F(X, Y, Z);


        public Vec4F(
            float _x = 0.0f,
            float _y = 0.0f,
            float _z = 0.0f,
            float _w = 0.0f)
        {
            X = _x;
            Y = _y;
            Z = _z;
            W = _w;
        }

        public Vec4F(
            Vec3F _vec,
            float _w = 0.0f)
        {
            X = _vec.X;
            Y = _vec.Y;
            Z = _vec.Z;
            W = _w;
        }

        public Vec4F(
            Vec2F _vec,
            float _z = 0.0f,
            float _w = 0.0f)
        {
            X = _vec.X;
            Y = _vec.Y;
            Z = _z;
            W = _w;
        }


        public static Vec4F operator -(Vec4F _vec0)
        {
            return new Vec4F(-_vec0.X, -_vec0.Y, -_vec0.Z, -_vec0.W);
        }

        public static Vec4F operator +(Vec4F _vec0, float _value)
        {
            return new Vec4F(_vec0.X + _value, _vec0.Y + _value, _vec0.Z + _value, _vec0.W + _value);
        }

        public static Vec4F operator +(Vec4F _vec0, Vec4F _vec1)
        {
            return new Vec4F(_vec0.X + _vec1.X, _vec0.Y + _vec1.Y, _vec0.Z + _vec1.Z, _vec0.W + _vec1.W);
        }

        public static Vec4F operator -(Vec4F _vec0, float _value)
        {
            return new Vec4F(_vec0.X - _value, _vec0.Y - _value, _vec0.Z - _value, _vec0.W - _value);
        }

        public static Vec4F operator -(Vec4F _vec0, Vec4F _vec1)
        {
            return new Vec4F(_vec0.X - _vec1.X, _vec0.Y - _vec1.Y, _vec0.Z - _vec1.Z, _vec0.W - _vec1.W);
        }

        public static Vec4F operator *(Vec4F _vec0, float _value)
        {
            return new Vec4F(_vec0.X * _value, _vec0.Y * _value, _vec0.Z * _value, _vec0.W * _value);
        }

        public static Vec4F operator *(Vec4F _vec0, Vec4F _vec1)
        {
            return new Vec4F(_vec0.X * _vec1.X, _vec0.Y * _vec1.Y, _vec0.Z * _vec1.Z, _vec0.W * _vec1.W);
        }

        public static Vec4F operator /(Vec4F _vec0, float _value)
        {
            return new Vec4F(_vec0.X / _value, _vec0.Y / _value, _vec0.Z / _value, _vec0.W / _value);
        }

        public static Vec4F operator /(Vec4F _vec0, Vec4F _vec1)
        {
            return new Vec4F(_vec0.X / _vec1.X, _vec0.Y / _vec1.Y, _vec0.Z / _vec1.Z, _vec0.W / _vec1.W);
        }
    }
}
