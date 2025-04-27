using System;
using System.Runtime.InteropServices;


namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct TMat
    {
        public float M00, M01, M02;
        public float M10, M11, M12;
        public float M20, M21, M22;
        public float M30, M31, M32;

        public static readonly TMat Identity = new TMat(
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f);

        public TMat(
            float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22,
            float m30, float m31, float m32)
        {
            M00 = m00; M01 = m01; M02 = m02;
            M10 = m10; M11 = m11; M12 = m12;
            M20 = m20; M21 = m21; M22 = m22;
            M30 = m30; M31 = m31; M32 = m32;
        }

        public static TMat CreateRotationX(float angle)
        {
            // CCW rotation around X axis 
            float c = (float)Math.Cos(angle);
            float s = (float)Math.Sin(angle);
            return new TMat(
                1, 0, 0,
                0, c, s,
                0,  -s, c,
                0, 0, 0);
        }

        public static TMat CreateRotationY(float angle)
        {
            // CCW rotation around Y axis 
            float c = (float)Math.Cos(angle);
            float s = (float)Math.Sin(angle);
            return new TMat(
                c, 0,  -s,
                0, 1, 0,
                s, 0, c,
                0, 0, 0);
        }

        public static TMat CreateRotationZ(float angle)
        {
            // CCW rotation around Z axis 
            float c = (float)Math.Cos(angle);
            float s = (float)Math.Sin(angle);
            return new TMat(
                c, s, 0,
                -s, c, 0,
                0, 0, 1,
                0, 0, 0);
        }

        public static TMat CreateRotation(float angleX, float angleY, float angleZ)
        {
            // CCW rotation
            float cx = (float)Math.Cos(angleX);
            float sx = (float)Math.Sin(angleX);

            float cy = (float)Math.Cos(angleY);
            float sy = (float)Math.Sin(angleY);

            float cz = (float)Math.Cos(angleZ);
            float sz = (float)Math.Sin(angleZ);

            // #TODO: simplify
            return
                new TMat(
                    cy, 0,  -sy,
                    0, 1, 0,
                    sy, 0, cy,
                    0, 0, 0).Transform(
                    new TMat(
                        1, 0, 0,
                        0, cx, sx,
                        0,  -sx, cx,
                        0, 0, 0)).Transform(
                        new TMat(
                            cz, sz, 0,
                             -sz, cz, 0,
                            0, 0, 1,
                            0, 0, 0));
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
                else if (index == 3)
                    return new Vec3F(M30, M31, M32);
                else
                    throw new IndexOutOfRangeException("Index must be between 0 and 3.");
            }
        }

        //////////////////////////////////////////
        //
        // [ mat.M00 mat.M01 mat.M02 0 ]   [ M00 M01 M02 0 ]
        // | mat.M10 mat.M11 mat.M12 0 | * | M10 M11 M12 0 |
        // | mat.M20 mat.M21 mat.M22 0 |   | M20 M21 M22 0 |
        // [ mat.M30 mat.M31 mat.M32 1 ]   [ M30 M31 M32 1 ]
        // 
        //////////////////////////////////////////
        public void Transform(TMat mat, out TMat outMat)
        {
            outMat.M00 = mat.M00 * M00 + mat.M01 * M10 + mat.M02 * M20;
            outMat.M01 = mat.M00 * M01 + mat.M01 * M11 + mat.M02 * M21;
            outMat.M02 = mat.M00 * M02 + mat.M01 * M12 + mat.M02 * M22;

            outMat.M10 = mat.M10 * M00 + mat.M11 * M10 + mat.M12 * M20;
            outMat.M11 = mat.M10 * M01 + mat.M11 * M11 + mat.M12 * M21;
            outMat.M12 = mat.M10 * M02 + mat.M11 * M12 + mat.M12 * M22;

            outMat.M20 = mat.M20 * M00 + mat.M21 * M10 + mat.M22 * M20;
            outMat.M21 = mat.M20 * M01 + mat.M21 * M11 + mat.M22 * M21;
            outMat.M22 = mat.M20 * M02 + mat.M21 * M12 + mat.M22 * M22;

            outMat.M30 = mat.M30 * M00 + mat.M31 * M10 + mat.M32 * M20 + M30;
            outMat.M31 = mat.M30 * M01 + mat.M31 * M11 + mat.M32 * M21 + M31;
            outMat.M32 = mat.M30 * M02 + mat.M31 * M12 + mat.M32 * M22 + M32;
        }

        public TMat Transform(TMat mat)
        {
            Transform(mat, out TMat res);
            return res;
        }

        //////////////////////////////////////////
        public TMat Multiplied(TMat m)
        {
            m.Transform(this, out TMat res);
            return res;
        }

        //////////////////////////////////////////
        //
        //                           [ m[0][0] m[0][1] m[0][2] 0 ]
        // [_v.x, _v.y, _v.z, 1.0] * | m[1][0] m[1][1] m[1][2] 0 |
        //                           | m[2][0] m[2][1] m[2][2] 0 |
        //                           [ m[3][0] m[3][1] m[3][2] 1 ]
        // 
        //////////////////////////////////////////
        public Vec3F Transform(Vec3F v)
        {
            return new Vec3F(
                v.X * M00 + v.Y * M10 + v.Z * M20 + M30,
                v.X * M01 + v.Y * M11 + v.Z * M21 + M31,
                v.X * M02 + v.Y * M12 + v.Z * M22 + M32);
        }

        public TMat Inversed()
        {
            float m01 = M01, m11 = M11, m21 = M21;
            float m02 = M02, m12 = M12, m22 = M22;

            float t00 = m22 * m11 - m12 * m21;
            float t01 = m02 * m21 - m22 * m01;
            float t02 = m12 * m01 - m02 * m11;

            float m00 = M00, m10 = M10, m20 = M20;

            float invDet = 1 / (m00 * t00 + m10 * t01 + m20 * t02);

            t00 *= invDet; t01 *= invDet; t02 *= invDet;

            m00 *= invDet; m10 *= invDet; m20 *= invDet;

            float r00 = t00;
            float r10 = m20 * m12 - m10 * m22;
            float r20 = m10 * m21 - m20 * m11;

            float r01 = t01;
            float r11 = m00 * m22 - m20 * m02;
            float r21 = m20 * m01 - m00 * m21;

            float r02 = t02;
            float r12 = m10 * m02 - m00 * m12;
            float r22 = m00 * m11 - m10 * m01;

            float m30 = M30, m31 = M31, m32 = M32;

            float r30 = -(r00 * m30 + r10 * m31 + r20 * m32);
            float r31 = -(r01 * m30 + r11 * m31 + r21 * m32);
            float r32 = -(r02 * m30 + r12 * m31 + r22 * m32);

            return new TMat(
                r00, r01, r02,
                r10, r11, r12,
                r20, r21, r22,
                r30, r31, r32);
        }

        public static TMat operator *(TMat left, TMat right)
        {
            return left.Multiplied(right);
        }

        public static Vec3F operator *(Vec3F vector, TMat matrix)
        {
            return matrix.Transform(vector);
        }

        public static Vec3F operator *(TMat matrix, Vec3F vector)
        {
            return new Vec3F(
                matrix.M00 * vector.X + matrix.M01 * vector.Y + matrix.M02 * vector.Z + matrix.M30,
                matrix.M10 * vector.X + matrix.M11 * vector.Y + matrix.M12 * vector.Z + matrix.M31,
                matrix.M20 * vector.X + matrix.M21 * vector.Y + matrix.M22 * vector.Z + matrix.M32
            );
        }

        public Mat3F GetMat3()
        {
            return new Mat3F(
                M00, M01, M02,
                M10, M11, M12,
                M20, M21, M22);
        }

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[48];
            BitConverter.GetBytes(M00).CopyTo(bytes, 0);
            BitConverter.GetBytes(M01).CopyTo(bytes, 4);
            BitConverter.GetBytes(M02).CopyTo(bytes, 8);
            BitConverter.GetBytes(M10).CopyTo(bytes, 12);
            BitConverter.GetBytes(M11).CopyTo(bytes, 16);
            BitConverter.GetBytes(M12).CopyTo(bytes, 20);
            BitConverter.GetBytes(M20).CopyTo(bytes, 24);
            BitConverter.GetBytes(M21).CopyTo(bytes, 28);
            BitConverter.GetBytes(M22).CopyTo(bytes, 32);
            BitConverter.GetBytes(M30).CopyTo(bytes, 36);
            BitConverter.GetBytes(M31).CopyTo(bytes, 40);
            BitConverter.GetBytes(M32).CopyTo(bytes, 44);
            return bytes;
        }

        public static TMat FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 48)
                throw new ArgumentException($"Byte array must contain at least 48 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new TMat
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
                M30 = BitConverter.ToSingle(bytes, startIndex + 36),
                M31 = BitConverter.ToSingle(bytes, startIndex + 40),
                M32 = BitConverter.ToSingle(bytes, startIndex + 44)
            };
        }
        public override string ToString()
        {
            return $"[{M00}, {M01}, {M02}]" +
                   $"[{M10}, {M11}, {M12}]" +
                   $"[{M20}, {M21}, {M22}]" +
                   $"[{M30}, {M31}, {M32}]";
        }

    }
}
