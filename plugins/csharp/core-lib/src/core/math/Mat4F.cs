using System;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Mat4F
    {
        public float M00, M01, M02, M03;
        public float M10, M11, M12, M13;
        public float M20, M21, M22, M23;
        public float M30, M31, M32, M33;

        public static Mat4F Identity => new Mat4F(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        public Mat4F(
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33)
        {
            M00 = m00; M01 = m01; M02 = m02; M03 = m03;
            M10 = m10; M11 = m11; M12 = m12; M13 = m13;
            M20 = m20; M21 = m21; M22 = m22; M23 = m23;
            M30 = m30; M31 = m31; M32 = m32; M33 = m33;
        }

        public Vec4F this[int index]
        {
            get
            {
                if (index == 0)
                    return new Vec4F(M00, M01, M02, M03);
                else if (index == 1)
                    return new Vec4F(M10, M11, M12, M13);
                else if (index == 2)
                    return new Vec4F(M20, M21, M22, M23);
                else if (index == 3)
                    return new Vec4F(M30, M31, M32, M33);
                else
                    throw new IndexOutOfRangeException("Index must be between 0 and 3.");
            }
        }

        public static Mat4F operator *(Mat4F left, Mat4F right)
        {
            return new Mat4F(
                left.M00 * right.M00 + left.M01 * right.M10 + left.M02 * right.M20 + left.M03 * right.M30,
                left.M00 * right.M01 + left.M01 * right.M11 + left.M02 * right.M21 + left.M03 * right.M31,
                left.M00 * right.M02 + left.M01 * right.M12 + left.M02 * right.M22 + left.M03 * right.M32,
                left.M00 * right.M03 + left.M01 * right.M13 + left.M02 * right.M23 + left.M03 * right.M33,

                left.M10 * right.M00 + left.M11 * right.M10 + left.M12 * right.M20 + left.M13 * right.M30,
                left.M10 * right.M01 + left.M11 * right.M11 + left.M12 * right.M21 + left.M13 * right.M31,
                left.M10 * right.M02 + left.M11 * right.M12 + left.M12 * right.M22 + left.M13 * right.M32,
                left.M10 * right.M03 + left.M11 * right.M13 + left.M12 * right.M23 + left.M13 * right.M33,

                left.M20 * right.M00 + left.M21 * right.M10 + left.M22 * right.M20 + left.M23 * right.M30,
                left.M20 * right.M01 + left.M21 * right.M11 + left.M22 * right.M21 + left.M23 * right.M31,
                left.M20 * right.M02 + left.M21 * right.M12 + left.M22 * right.M22 + left.M23 * right.M32,
                left.M20 * right.M03 + left.M21 * right.M13 + left.M22 * right.M23 + left.M23 * right.M33,

                left.M30 * right.M00 + left.M31 * right.M10 + left.M32 * right.M20 + left.M33 * right.M30,
                left.M30 * right.M01 + left.M31 * right.M11 + left.M32 * right.M21 + left.M33 * right.M31,
                left.M30 * right.M02 + left.M31 * right.M12 + left.M32 * right.M22 + left.M33 * right.M32,
                left.M30 * right.M03 + left.M31 * right.M13 + left.M32 * right.M23 + left.M33 * right.M33
            );
        }

        public float Determinant()
        {
            return
                M00 * (M11 * (M22 * M33 - M23 * M32) - M12 * (M21 * M33 - M23 * M31) + M13 * (M21 * M32 - M22 * M31)) -
                M01 * (M10 * (M22 * M33 - M23 * M32) - M12 * (M20 * M33 - M23 * M30) + M13 * (M20 * M32 - M22 * M30)) +
                M02 * (M10 * (M21 * M33 - M23 * M31) - M11 * (M20 * M33 - M23 * M30) + M13 * (M20 * M31 - M21 * M30)) -
                M03 * (M10 * (M21 * M32 - M22 * M31) - M11 * (M20 * M32 - M22 * M30) + M12 * (M20 * M31 - M21 * M30));
        }

        public Mat4F Transpose()
        {
            return new Mat4F(
                M00, M10, M20, M30,
                M01, M11, M21, M31,
                M02, M12, M22, M32,
                M03, M13, M23, M33);
        }

        public Mat4F Inverse()
        {
            float det = Determinant();
            if (Math.Abs(det) < 1e-6f)
                throw new InvalidOperationException("Matrix is not invertible.");

            float invDet = 1f / det;

            return new Mat4F(
                (M11 * (M22 * M33 - M23 * M32) - M12 * (M21 * M33 - M23 * M31) + M13 * (M21 * M32 - M22 * M31)) * invDet,
                (M01 * (M22 * M33 - M23 * M32) - M02 * (M21 * M33 - M23 * M31) + M03 * (M21 * M32 - M22 * M31)) * invDet,
                (M01 * (M12 * M33 - M13 * M32) - M02 * (M11 * M33 - M13 * M31) + M03 * (M11 * M32 - M12 * M31)) * invDet,
                (M01 * (M12 * M23 - M13 * M22) - M02 * (M11 * M23 - M13 * M21) + M03 * (M11 * M22 - M12 * M21)) * invDet,

                (M10 * (M22 * M33 - M23 * M32) - M12 * (M20 * M33 - M23 * M30) + M13 * (M20 * M32 - M22 * M30)) * invDet,
                (M00 * (M22 * M33 - M23 * M32) - M02 * (M20 * M33 - M23 * M30) + M03 * (M20 * M32 - M22 * M30)) * invDet,
                (M00 * (M12 * M33 - M13 * M32) - M02 * (M10 * M33 - M13 * M30) + M03 * (M10 * M32 - M12 * M30)) * invDet,
                (M00 * (M12 * M23 - M13 * M22) - M02 * (M10 * M23 - M13 * M20) + M03 * (M10 * M22 - M12 * M20)) * invDet,

                (M10 * (M21 * M33 - M23 * M31) - M11 * (M20 * M33 - M23 * M30) + M13 * (M20 * M31 - M21 * M30)) * invDet,
                (M00 * (M21 * M33 - M23 * M31) - M01 * (M20 * M33 - M23 * M30) + M03 * (M20 * M31 - M21 * M30)) * invDet,
                (M00 * (M11 * M33 - M13 * M31) - M01 * (M10 * M33 - M13 * M30) + M03 * (M10 * M31 - M11 * M30)) * invDet,
                (M00 * (M11 * M23 - M13 * M21) - M01 * (M10 * M23 - M13 * M20) + M03 * (M10 * M21 - M11 * M20)) * invDet,

                (M10 * (M21 * M32 - M22 * M31) - M11 * (M20 * M32 - M22 * M30) + M12 * (M20 * M31 - M21 * M30)) * invDet,
                (M00 * (M21 * M32 - M22 * M31) - M01 * (M20 * M32 - M22 * M30) + M02 * (M20 * M31 - M21 * M30)) * invDet,
                (M00 * (M11 * M32 - M12 * M31) - M01 * (M10 * M32 - M12 * M30) + M02 * (M10 * M31 - M11 * M30)) * invDet,
                (M00 * (M11 * M22 - M12 * M21) - M01 * (M10 * M22 - M12 * M20) + M02 * (M10 * M21 - M11 * M20)) * invDet
            );
        }

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[64];
            BitConverter.GetBytes(M00).CopyTo(bytes, 0);
            BitConverter.GetBytes(M01).CopyTo(bytes, 4);
            BitConverter.GetBytes(M02).CopyTo(bytes, 8);
            BitConverter.GetBytes(M03).CopyTo(bytes, 12);
            BitConverter.GetBytes(M10).CopyTo(bytes, 16);
            BitConverter.GetBytes(M11).CopyTo(bytes, 20);
            BitConverter.GetBytes(M12).CopyTo(bytes, 24);
            BitConverter.GetBytes(M13).CopyTo(bytes, 28);
            BitConverter.GetBytes(M20).CopyTo(bytes, 32);
            BitConverter.GetBytes(M21).CopyTo(bytes, 36);
            BitConverter.GetBytes(M22).CopyTo(bytes, 40);
            BitConverter.GetBytes(M23).CopyTo(bytes, 44);
            BitConverter.GetBytes(M30).CopyTo(bytes, 48);
            BitConverter.GetBytes(M31).CopyTo(bytes, 52);
            BitConverter.GetBytes(M32).CopyTo(bytes, 56);
            BitConverter.GetBytes(M33).CopyTo(bytes, 60);
            return bytes;
        }

        public static Mat4F FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 64)
                throw new ArgumentException($"Byte array must contain at least 64 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Mat4F
            {
                M00 = BitConverter.ToSingle(bytes, startIndex + 0),
                M01 = BitConverter.ToSingle(bytes, startIndex + 4),
                M02 = BitConverter.ToSingle(bytes, startIndex + 8),
                M03 = BitConverter.ToSingle(bytes, startIndex + 12),
                M10 = BitConverter.ToSingle(bytes, startIndex + 16),
                M11 = BitConverter.ToSingle(bytes, startIndex + 20),
                M12 = BitConverter.ToSingle(bytes, startIndex + 24),
                M13 = BitConverter.ToSingle(bytes, startIndex + 28),
                M20 = BitConverter.ToSingle(bytes, startIndex + 32),
                M21 = BitConverter.ToSingle(bytes, startIndex + 36),
                M22 = BitConverter.ToSingle(bytes, startIndex + 40),
                M23 = BitConverter.ToSingle(bytes, startIndex + 44),
                M30 = BitConverter.ToSingle(bytes, startIndex + 48),
                M31 = BitConverter.ToSingle(bytes, startIndex + 52),
                M32 = BitConverter.ToSingle(bytes, startIndex + 56),
                M33 = BitConverter.ToSingle(bytes, startIndex + 60)
            };
        }

        public Mat3F GetMat3()
        {
            return new Mat3F(
                M00, M01, M02,
                M10, M11, M12,
                M20, M21, M22);
        }

        public override string ToString()
        {
            return
                $"[{M00}, {M01}, {M02}, {M03}]" +
                $"[{M10}, {M11}, {M12}, {M13}]" +
                $"[{M20}, {M21}, {M22}, {M23}]" +
                $"[{M30}, {M31}, {M32}, {M33}]";
        }
    }
}
