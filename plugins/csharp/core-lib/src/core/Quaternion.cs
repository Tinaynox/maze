using System;

namespace Maze.Core
{
    public struct Quaternion
    {
        public float W;
        public float X;
        public float Y;
        public float Z;


        public Quaternion(
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f,
            float w = 1.0f)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Quaternion(
            Vec3F vec,
            float w = 1.0f)
        {
            X = vec.X;
            Y = vec.Y;
            Z = vec.Z;
            W = w;
        }

        public Quaternion(
            Vec2F vec,
            float z = 0.0f,
            float w = 1.0f)
        {
            X = vec.X;
            Y = vec.Y;
            Z = z;
            W = w;
        }


        public static Quaternion operator -(Quaternion vec0)
        {
            return new Quaternion(-vec0.X, -vec0.Y, -vec0.Z, -vec0.W);
        }

        public static Quaternion operator +(Quaternion vec0, float value)
        {
            return new Quaternion(vec0.X + value, vec0.Y + value, vec0.Z + value, vec0.W + value);
        }

        public static Quaternion operator +(Quaternion vec0, Quaternion vec1)
        {
            return new Quaternion(vec0.X + vec1.X, vec0.Y + vec1.Y, vec0.Z + vec1.Z, vec0.W + vec1.W);
        }

        public static Quaternion operator -(Quaternion vec0, float value)
        {
            return new Quaternion(vec0.X - value, vec0.Y - value, vec0.Z - value, vec0.W - value);
        }

        public static Quaternion operator -(Quaternion vec0, Quaternion vec1)
        {
            return new Quaternion(vec0.X - vec1.X, vec0.Y - vec1.Y, vec0.Z - vec1.Z, vec0.W - vec1.W);
        }

        public static Quaternion operator *(Quaternion vec0, float value)
        {
            return new Quaternion(vec0.X * value, vec0.Y * value, vec0.Z * value, vec0.W * value);
        }

        public static Quaternion operator *(Quaternion vec0, Quaternion vec1)
        {
            return new Quaternion(vec0.X * vec1.X, vec0.Y * vec1.Y, vec0.Z * vec1.Z, vec0.W * vec1.W);
        }

        public static Quaternion operator /(Quaternion vec0, float value)
        {
            return new Quaternion(vec0.X / value, vec0.Y / value, vec0.Z / value, vec0.W / value);
        }

        public static Quaternion operator /(Quaternion vec0, Quaternion vec1)
        {
            return new Quaternion(vec0.X / vec1.X, vec0.Y / vec1.Y, vec0.Z / vec1.Z, vec0.W / vec1.W);
        }

        public void setEulerAngles(
            float xAngle,
            float yAngle,
            float zAngle)
        {
            float pitch = xAngle;
            float yaw = yAngle;
            float roll = zAngle;

            float rollOver2 = roll * 0.5f;
            float sinRollOver2 = (float)Math.Sin(rollOver2);
            float cosRollOver2 = (float)Math.Cos(rollOver2);
            float pitchOver2 = pitch * 0.5f;
            float sinPitchOver2 = (float)Math.Sin(pitchOver2);
            float cosPitchOver2 = (float)Math.Cos(pitchOver2);
            float yawOver2 = yaw * 0.5f;
            float sinYawOver2 = (float)Math.Sin(yawOver2);
            float cosYawOver2 = (float)Math.Cos(yawOver2);
            W = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
            X = cosYawOver2 * sinPitchOver2 * cosRollOver2 + sinYawOver2 * cosPitchOver2 * sinRollOver2;
            Y = sinYawOver2 * cosPitchOver2 * cosRollOver2 - cosYawOver2 * sinPitchOver2 * sinRollOver2;
            Z = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;
        }

        public Vec3F getEuler()
        {
            // If the input quaternion is normalized, this is exactly one. Otherwise, this acts as a correction factor for the quaternion not-normalizedness
            float unit = (X * X) + (Y * Y) + (Z * Z) + (W * W);

            // This will have a magnitude of 0.5 or greater if and only if this is a singularity case
            float test = X * W - Y * Z;

            Vec3F euler;

            // Singularity at north pole
            if (test > 0.49995f * unit)
            {
                euler.X = (float)Math.PI * 0.5f;
                euler.Y = 2.0f * (float)Math.Atan2(Y, X);
                euler.Z = 0;
            }
            else
            // Singularity at south pole
            if (test < -0.49995f * unit)
            {
                euler.X = -(float)Math.PI * 0.5f;
                euler.Y = -2.0f * (float)Math.Atan2(Y, X);
                euler.Z = 0;
            }
            // No singularity - this is the majority of cases
            else
            {
                Quaternion q = new Quaternion(Y, W, Z, X);
                euler.X = (float)Math.Asin(2.0f * (q.X * q.Z - q.W * q.Y));
                euler.Y = (float)Math.Atan2(2.0f * q.X * q.W + 2.0f * q.Y * q.Z, 1.0f - 2.0f * (q.Z * q.Z + q.W * q.W));
                euler.Z = (float)Math.Atan2(2.0f * q.X * q.Y + 2.0f * q.Z * q.W, 1.0f - 2.0f * (q.Y * q.Y + q.Z * q.Z));
            }
            euler.X = MathHelper.NormalizedAnglePI(euler.X);
            euler.Y = MathHelper.NormalizedAnglePI(euler.Y);
            euler.Z = MathHelper.NormalizedAnglePI(euler.Z);

            return euler;
        }
    }
}
