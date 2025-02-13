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

        public static TMat operator *(TMat left, TMat right)
        {
            return new TMat(
                left.M00 * right.M00 + left.M01 * right.M10 + left.M02 * right.M20,
                left.M00 * right.M01 + left.M01 * right.M11 + left.M02 * right.M21,
                left.M00 * right.M02 + left.M01 * right.M12 + left.M02 * right.M22,

                left.M10 * right.M00 + left.M11 * right.M10 + left.M12 * right.M20,
                left.M10 * right.M01 + left.M11 * right.M11 + left.M12 * right.M21,
                left.M10 * right.M02 + left.M11 * right.M12 + left.M12 * right.M22,

                left.M20 * right.M00 + left.M21 * right.M10 + left.M22 * right.M20,
                left.M20 * right.M01 + left.M21 * right.M11 + left.M22 * right.M21,
                left.M20 * right.M02 + left.M21 * right.M12 + left.M22 * right.M22,

                left.M30 * right.M00 + left.M31 * right.M10 + left.M32 * right.M20,
                left.M30 * right.M01 + left.M31 * right.M11 + left.M32 * right.M21,
                left.M30 * right.M02 + left.M31 * right.M12 + left.M32 * right.M22
            );
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
