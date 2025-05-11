using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
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

        public float this[int index]
        {
            get
            {
                if (index == 0)
                    return X;
                else if (index == 1)
                    return Y;
                else if (index == 2)
                    return Z;
                else
                    throw new IndexOutOfRangeException("Index must be between 0 and 2.");
            }
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

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[12];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 4);
            BitConverter.GetBytes(Z).CopyTo(bytes, 8);
            return bytes;
        }

        public static Vec3F FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 12)
                throw new ArgumentException($"Byte array must contain at least 12 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Vec3F
            {
                X = BitConverter.ToSingle(bytes, startIndex + 0),
                Y = BitConverter.ToSingle(bytes, startIndex + 4),
                Z = BitConverter.ToSingle(bytes, startIndex + 8)
            };
        }

        public static int ParseString(string str, int start, int end, out Vec3F outValue, char separator = ',')
        {
            start = StringHelper.ParseFloat(str, start, end, out float x); outValue.X = x;
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseFloat(str, start, end, out float y); outValue.Y = y;
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseFloat(str, start, end, out float z); outValue.Z = z;

            return start;
        }

        public static int ParseString(string str, out Vec3F outValue, char separator = ',')
        {
            return ParseString(str, 0, str.Length, out outValue, separator);
        }

        public static int ParseString(List<byte> str, int start, int end, out Vec3F outValue, byte separator = (byte)',')
        {
            start = StringHelper.ParseFloat(str, start, end, out float x); outValue.X = x;
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseFloat(str, start, end, out float y); outValue.Y = y;
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseFloat(str, start, end, out float z); outValue.Z = z;

            return start;
        }

        public static int ParseString(List<byte> str, out Vec3F outValue, byte separator = (byte)',')
        {
            return ParseString(str, 0, str.Count, out outValue, separator);
        }

        public override string ToString()
        {
            return $"[{X}, {Y}, {Z}]";
        }
    }
}
