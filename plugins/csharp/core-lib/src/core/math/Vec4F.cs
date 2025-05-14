using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;


namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
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
                else if (index == 3)
                    return W;
                else
                    throw new IndexOutOfRangeException("Index must be between 0 and 3.");
            }
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

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[16];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 4);
            BitConverter.GetBytes(Z).CopyTo(bytes, 8);
            BitConverter.GetBytes(W).CopyTo(bytes, 12);
            return bytes;
        }

        public static Vec4F FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 16)
                throw new ArgumentException($"Byte array must contain at least 16 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Vec4F
            {
                X = BitConverter.ToSingle(bytes, startIndex + 0),
                Y = BitConverter.ToSingle(bytes, startIndex + 4),
                Z = BitConverter.ToSingle(bytes, startIndex + 8),
                W = BitConverter.ToSingle(bytes, startIndex + 12)
            };
        }


        public static int ParseString(string str, int start, int end, out Vec4F outValue, char separator = ',')
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
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ParseFloat(str, start, end, out float w); outValue.W = w;

            return start;
        }

        public static int ParseString(string str, out Vec4F outValue, char separator = ',')
        {
            return ParseString(str, 0, str.Length, out outValue, separator);
        }

        public static int ParseString(List<byte> str, int start, int end, out Vec4F outValue, byte separator = (byte)',')
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
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, separator);
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ParseFloat(str, start, end, out float w); outValue.W = w;

            return start;
        }

        public static int ParseString(List<byte> str, out Vec4F outValue, byte separator = (byte)',')
        {
            return ParseString(str, 0, str.Count, out outValue, separator);
        }

        public override string ToString()
        {
            return $"{X}, {Y}, {Z}, {W}";
        }
    }
}
