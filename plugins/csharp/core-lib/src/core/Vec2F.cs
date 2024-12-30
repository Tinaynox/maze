using System;

namespace Maze.Core
{
    public struct Vec2F
    {
        public float X;
        public float Y;

        public Vec2F(
            float _x = 0.0f,
            float _y = 0.0f)
        {
            X = _x;
            Y = _y;
        }

        public static Vec2F operator +(Vec2F _vec0, float _value)
        {
            return new Vec2F(_vec0.X + _value, _vec0.Y + _value);
        }

        public static Vec2F operator +(Vec2F _vec0, Vec2F _vec1)
        {
            return new Vec2F(_vec0.X + _vec1.X, _vec0.Y + _vec1.Y);
        }

        public static Vec2F operator -(Vec2F _vec0, float _value)
        {
            return new Vec2F(_vec0.X - _value, _vec0.Y - _value);
        }

        public static Vec2F operator -(Vec2F _vec0, Vec2F _vec1)
        {
            return new Vec2F(_vec0.X - _vec1.X, _vec0.Y - _vec1.Y);
        }

        public static Vec2F operator*(Vec2F _vec0, float _value)
        {
            return new Vec2F(_vec0.X * _value, _vec0.Y * _value);
        }

        public static Vec2F operator *(Vec2F _vec0, Vec2F _vec1)
        {
            return new Vec2F(_vec0.X * _vec1.X, _vec0.Y * _vec1.Y);
        }

        public static Vec2F operator /(Vec2F _vec0, float _value)
        {
            return new Vec2F(_vec0.X / _value, _vec0.Y / _value);
        }

        public static Vec2F operator /(Vec2F _vec0, Vec2F _vec1)
        {
            return new Vec2F(_vec0.X / _vec1.X, _vec0.Y / _vec1.Y);
        }
    }
}
