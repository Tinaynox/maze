using System;
using System.Runtime.InteropServices;


namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Quaternion
    {
        const float Epsilon = 1e-03f;
        public static Quaternion Identity = new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);


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

        public static Quaternion FromRotationMatrix(Mat3F rotationMatrix)
        {
            Quaternion q = new Quaternion();
            q.SetRotationMatrix(rotationMatrix);
            return q;
        }

        public static Quaternion FromRotationMatrix(Mat4F rotationMatrix)
        {
            Quaternion q = new Quaternion();
            q.SetRotationMatrix(rotationMatrix);
            return q;
        }

        public static Quaternion FromRotationMatrix(TMat rotationMatrix)
        {
            Quaternion q = new Quaternion();
            q.SetRotationMatrix(rotationMatrix);
            return q;
        }

        public static Quaternion LookRotation(Vec3F forward, Vec3F up)
        {
            Vec3F right = up.Cross(forward).NormalizedCopy();
            up = forward.Cross(right).NormalizedCopy();
            
            Mat3F rotationMatrix = new Mat3F();
            rotationMatrix.SetRow(0, right);
            rotationMatrix.SetRow(1, up);
            rotationMatrix.SetRow(2, forward);

            return FromRotationMatrix(rotationMatrix);
        }

        public static Quaternion Slerp(
            float t,
            Quaternion p,
            Quaternion q,
            bool shortestPath = true)
        {
            t = MathHelper.Clamp(t, 0.0f, 1.0f);

            float c = p.Dot(q);
            Quaternion r;

            if (c < 0.0f && shortestPath)
            {
                c = -c;
                r = -q;
            }
            else
            {
                r = q;
            }

            if (Math.Abs(c) < 1.0 - Epsilon)
            {
                float s = (float)Math.Sqrt(1.0 - c * c);
                float angle = (float)Math.Atan2(s, c);
                float invSin = 1.0f / s;
                float coeff0 = (float)Math.Sin((1.0f - t) * angle) * invSin;
                float coeff1 = (float)Math.Sin(t * angle) * invSin;
                return coeff0 * p + coeff1 * r;
            }
            else
            {
                Quaternion res = (1.0f - t) * p + t * r;

                res.Normalize();

                return res;
            }
        }

        public static Quaternion operator -(Quaternion vec0)
        {
            return new Quaternion(-vec0.X, -vec0.Y, -vec0.Z, -vec0.W);
        }

        public static Quaternion operator +(Quaternion vec0, float value)
        {
            return new Quaternion(vec0.X + value, vec0.Y + value, vec0.Z + value, vec0.W + value);
        }

        public static Quaternion operator +(float value, Quaternion vec0)
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

        public static Quaternion operator -(float value, Quaternion vec0)
        {
            return new Quaternion(value - vec0.X, value - vec0.Y, value - vec0.Z, value - vec0.W);
        }

        public static Quaternion operator -(Quaternion vec0, Quaternion vec1)
        {
            return new Quaternion(vec0.X - vec1.X, vec0.Y - vec1.Y, vec0.Z - vec1.Z, vec0.W - vec1.W);
        }

        public static Quaternion operator *(Quaternion vec0, float value)
        {
            return new Quaternion(vec0.X * value, vec0.Y * value, vec0.Z * value, vec0.W * value);
        }

        public static Quaternion operator *(float value, Quaternion vec0)
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

        public float Dot(Quaternion q)
        {
            return W * q.W + X * q.X + Y * q.Y + Z * q.Z;
        }

        public float AngleBetween(Quaternion b)
        {
            float dot = MathHelper.Clamp(Math.Abs(Dot(b)), -1.0f, 1.0f);
            return (float)Math.Acos(dot) * 2.0f;
        }

        public float Length()
        {
            return (float)Math.Sqrt(W * W + X * X + Y * Y + Z * Z);
        }

        public float Normalize()
        {
            float l = Length();
            this = 1.0f / l * this;
            return l;
        }

        public void SetEulerAngles(
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

        public Vec3F GetEuler()
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

        
        public void SetRotationMatrix(Mat3F rotationMatrix)
        {
            float trace = rotationMatrix[0][0] + rotationMatrix[1][1] + rotationMatrix[2][2];
            float root;

            // |w| > 1/2, may as well choose w > 1/2
            if (trace > 0.0)
            {
                // 2w
                root = (float)Math.Sqrt(trace + 1.0f);

                W = 0.5f * root;

                // 1/(4w)
                root = 0.5f / root;  

                X = (rotationMatrix[1][2] - rotationMatrix[2][1]) * root;
                Y = (rotationMatrix[2][0] - rotationMatrix[0][2]) * root;
                Z = (rotationMatrix[0][1] - rotationMatrix[1][0]) * root;
            }
            // |w| <= 1/2
            else
            {
                int[] next = new int[] { 1, 2, 0 };
                int i = 0;

                if (rotationMatrix[1][1] > rotationMatrix[0][0])
                    i = 1;

                if (rotationMatrix[2][2] > rotationMatrix[i][i])
                    i = 2;

                int j = next[i];
                int k = next[j];

                root = (float)Math.Sqrt(rotationMatrix[i][i] - rotationMatrix[j][j] - rotationMatrix[k][k] + 1.0f);
                float value = 0.5f * root;
                if (i == 0)
                    X = value;
                else if (i == 1)
                    Y = value;
                else
                    Z = value;

                root = 0.5f / root;
                W = (rotationMatrix[j][k] - rotationMatrix[k][j]) * root;

                value = (rotationMatrix[i][j] + rotationMatrix[j][i]) * root;
                if (j == 0)
                    X = value;
                else if (j == 1)
                    Y = value;
                else
                    Z = value;

                value = (rotationMatrix[i][k] + rotationMatrix[k][i]) * root;
                if (k == 0)
                    X = value;
                else if (k == 1)
                    Y = value;
                else
                    Z = value;
            }
        }

        public void SetRotationMatrix(Mat4F rotationMatrix)
        {
            SetRotationMatrix(rotationMatrix.GetMat3());
        }

        public void SetRotationMatrix(TMat rotationMatrix)
        {
            SetRotationMatrix(rotationMatrix.GetMat3());
        }

        public override string ToString()
        {
            return $"[{X}, {Y}, {Z}, {W}]";
        }
    }
}
