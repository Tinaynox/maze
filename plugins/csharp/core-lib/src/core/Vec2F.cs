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

        public static Vec2F operator -(Vec2F _vec0)
        {
            return new Vec2F(-_vec0.X, -_vec0.Y);
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

        public float Normalize()
        {
            float length = (float)Math.Sqrt(X * X + Y * Y);

            if (length > 1e-08f)
            {
                float invLength = 1.0f / length;
                X *= invLength;
                Y *= invLength;
            }

            return length;
        }

        public float Dot(Vec2F _vec)
        {
            return X * _vec.X + Y * _vec.Y;
        }

        public Vec2F Reflect(Vec2F _normal)
        {
            return this - (_normal * Dot(_normal) * 2.0f);
        }

        public Vec2F RandomDirection()
        {
            Random random = new Random();

            double angle = 2.0 * Math.PI * random.NextDouble();
            return new Vec2F((float)Math.Cos(angle), (float)Math.Sin(angle));
        }
    }
}
