using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;


namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Mat3F
    {
        public float M00, M01, M02;
        public float M10, M11, M12;
        public float M20, M21, M22;

        public static Mat3F Identity => new Mat3F(
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f);

        public Mat3F(
            float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22)
        {
            M00 = m00; M01 = m01; M02 = m02;
            M10 = m10; M11 = m11; M12 = m12;
            M20 = m20; M21 = m21; M22 = m22;
        }

        public Vec3F this[int index]
        {
            get
            {
                if (index == 0)
                    return new Vec3F(M00, M01, M02);
                else if (index == 1)
                    return new Vec3F(M10, M11, M12);
                else if (index == 2)
                    return new Vec3F(M20, M21, M22);
                else
                    throw new IndexOutOfRangeException("Index must be between 0 and 2.");
            }
        }

        public void SetRow(int i, Vec3F row)
        {
            if (i == 0)
            { 
                M00 = row.X;
                M01 = row.Y;
                M02 = row.Z;
            }
            else if (i == 1)
            {
                M10 = row.X;
                M11 = row.Y;
                M12 = row.Z;
            }
            else if (i == 2)
            {
                M20 = row.X;
                M21 = row.Y;
                M22 = row.Z;
            }
            else
                throw new IndexOutOfRangeException("Index must be between 0 and 2.");
        }

        public static Mat3F Multiply(Mat3F a, Mat3F b)
        {
            return new Mat3F(
                a.M00 * b.M00 + a.M01 * b.M10 + a.M02 * b.M20,
                a.M00 * b.M01 + a.M01 * b.M11 + a.M02 * b.M21,
                a.M00 * b.M02 + a.M01 * b.M12 + a.M02 * b.M22,

                a.M10 * b.M00 + a.M11 * b.M10 + a.M12 * b.M20,
                a.M10 * b.M01 + a.M11 * b.M11 + a.M12 * b.M21,
                a.M10 * b.M02 + a.M11 * b.M12 + a.M12 * b.M22,

                a.M20 * b.M00 + a.M21 * b.M10 + a.M22 * b.M20,
                a.M20 * b.M01 + a.M21 * b.M11 + a.M22 * b.M21,
                a.M20 * b.M02 + a.M21 * b.M12 + a.M22 * b.M22
            );
        }

        public Mat3F Transpose()
        {
            return new Mat3F(
                M00, M10, M20,
                M01, M11, M21,
                M02, M12, M22
            );
        }

        public float Determinant()
        {
            return 
                  M00 * (M11 * M22 - M12 * M21)
                - M01 * (M10 * M22 - M12 * M20)
                + M02 * (M10 * M21 - M11 * M20);
        }

        public Mat3F Inverse()
        {
            float det = Determinant();
            if (Math.Abs(det) < float.Epsilon)
                throw new InvalidOperationException("Matrix is singular and cannot be inverted.");

            float invDet = 1.0f / det;

            return new Mat3F(
                (M11 * M22 - M12 * M21) * invDet,
                (M02 * M21 - M01 * M22) * invDet,
                (M01 * M12 - M02 * M11) * invDet,

                (M12 * M20 - M10 * M22) * invDet,
                (M00 * M22 - M02 * M20) * invDet,
                (M02 * M10 - M00 * M12) * invDet,

                (M10 * M21 - M11 * M20) * invDet,
                (M01 * M20 - M00 * M21) * invDet,
                (M00 * M11 - M01 * M10) * invDet
            );
        }


        public byte[] GetBytes()
        {
            byte[] bytes = new byte[36];
            BitConverter.GetBytes(M00).CopyTo(bytes, 0);
            BitConverter.GetBytes(M01).CopyTo(bytes, 4);
            BitConverter.GetBytes(M02).CopyTo(bytes, 8);
            BitConverter.GetBytes(M10).CopyTo(bytes, 12);
            BitConverter.GetBytes(M11).CopyTo(bytes, 16);
            BitConverter.GetBytes(M12).CopyTo(bytes, 20);
            BitConverter.GetBytes(M20).CopyTo(bytes, 24);
            BitConverter.GetBytes(M21).CopyTo(bytes, 28);
            BitConverter.GetBytes(M22).CopyTo(bytes, 32);
            return bytes;
        }

        public static Mat3F FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 36)
                throw new ArgumentException($"Byte array must contain at least 36 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Mat3F
            {
                M00 = BitConverter.ToSingle(bytes, startIndex + 0),
                M01 = BitConverter.ToSingle(bytes, startIndex + 4),
                M02 = BitConverter.ToSingle(bytes, startIndex + 8),
                M10 = BitConverter.ToSingle(bytes, startIndex + 12),
                M11 = BitConverter.ToSingle(bytes, startIndex + 16),
                M12 = BitConverter.ToSingle(bytes, startIndex + 20),
                M20 = BitConverter.ToSingle(bytes, startIndex + 24),
                M21 = BitConverter.ToSingle(bytes, startIndex + 28),
                M22 = BitConverter.ToSingle(bytes, startIndex + 32),
            };
        }

        public static int ParseStringPretty(string str, int start, int end, out Mat3F outValue, char separator = ',')
        {
            outValue = new Mat3F();

            start = StringHelper.ExpectSkipChar(str, start, end, '[');
            for (int r = 0; r < 3; ++r)
            {
                Vec3F row = new Vec3F();

                start = StringHelper.SkipChar(str, start, end, ' ');
                start = StringHelper.ExpectSkipChar(str, start, end, '[');
                start = StringHelper.SkipChar(str, start, end, ' ');

                start = StringHelper.ParseFloat(str, start, end, out float x);
                row.X = x;
                start = StringHelper.SkipChar(str, start, end, ' ');
                start = StringHelper.ExpectSkipChar(str, start, end, separator);
                start = StringHelper.SkipChar(str, start, end, ' ');
                start = StringHelper.ParseFloat(str, start, end, out float y);
                row.Y = y;
                start = StringHelper.SkipChar(str, start, end, ' ');
                start = StringHelper.ExpectSkipChar(str, start, end, separator);
                start = StringHelper.SkipChar(str, start, end, ' ');
                start = StringHelper.ParseFloat(str, start, end, out float z);
                row.Z = z;

                outValue.SetRow(r, row);

                start = StringHelper.SkipChar(str, start, end, ' ');
                start = StringHelper.ExpectSkipChar(str, start, end, ']');
            }
            start = StringHelper.SkipChar(str, start, end, ' ');
            start = StringHelper.ExpectSkipChar(str, start, end, ']');

            return start;
        }

        public static int ParseStringPretty(string str, out Mat3F outValue, char separator = ',')
        {
            return ParseStringPretty(str, 0, str.Length, out outValue, separator);
        }

        public static int ParseStringPretty(List<byte> str, int start, int end, out Mat3F outValue, byte separator = (byte)',')
        {
            outValue = new Mat3F();

            start = StringHelper.ExpectSkipChar(str, start, end, (byte)'[');
            for (int r = 0; r < 3; ++r)
            {
                Vec3F row = new Vec3F();

                start = StringHelper.SkipChar(str, start, end, (byte)' ');
                start = StringHelper.ExpectSkipChar(str, start, end, (byte)'[');
                start = StringHelper.SkipChar(str, start, end, (byte)' ');

                start = StringHelper.ParseFloat(str, start, end, out float x);
                row.X = x;
                start = StringHelper.SkipChar(str, start, end, (byte)' ');
                start = StringHelper.ExpectSkipChar(str, start, end, separator);
                start = StringHelper.SkipChar(str, start, end, (byte)' ');
                start = StringHelper.ParseFloat(str, start, end, out float y);
                row.Y = y;
                start = StringHelper.SkipChar(str, start, end, (byte)' ');
                start = StringHelper.ExpectSkipChar(str, start, end, separator);
                start = StringHelper.SkipChar(str, start, end, (byte)' ');
                start = StringHelper.ParseFloat(str, start, end, out float z);
                row.Z = z;

                outValue.SetRow(r, row);

                start = StringHelper.SkipChar(str, start, end, (byte)' ');
                start = StringHelper.ExpectSkipChar(str, start, end, (byte)']');
            }
            start = StringHelper.SkipChar(str, start, end, (byte)' ');
            start = StringHelper.ExpectSkipChar(str, start, end, (byte)']');

            return start;
        }

        public static int ParseStringPretty(List<byte> str, out Mat3F outValue, byte separator = (byte)',')
        {
            return ParseStringPretty(str, 0, str.Count, out outValue, separator);
        }

        public override string ToString()
        {
            return
                $"[{M00}, {M01}, {M02}]" +
                $"[{M10}, {M11}, {M12}]" +
                $"[{M20}, {M21}, {M22}]";
        }
    }
}
