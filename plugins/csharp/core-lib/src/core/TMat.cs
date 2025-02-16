using System;

namespace Maze.Core
{
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

        public static TMat operator *(TMat left, TMat right)
        {
            return left.Multiplied(right);
        }

        public static Vec3F operator *(TMat matrix, Vec3F vector)
        {
            return new Vec3F(
                matrix.M00 * vector.X + matrix.M01 * vector.Y + matrix.M02 * vector.Z + matrix.M30,
                matrix.M10 * vector.X + matrix.M11 * vector.Y + matrix.M12 * vector.Z + matrix.M31,
                matrix.M20 * vector.X + matrix.M21 * vector.Y + matrix.M22 * vector.Z + matrix.M32
            );
        }

        public override string ToString()
        {
            return $"[{M00}, {M01}, {M02}]\n" +
                   $"[{M10}, {M11}, {M12}]\n" +
                   $"[{M20}, {M21}, {M22}]\n" +
                   $"[{M30}, {M31}, {M32}]";
        }

    }
}
