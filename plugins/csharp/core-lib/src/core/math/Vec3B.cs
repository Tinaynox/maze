using System;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec3B
    {
        public bool X;
        public bool Y;
        public bool Z;

        public static Vec3B False => new Vec3B(false, false, false);
        public static Vec3B True => new Vec3B(true, true, true);

        public Vec3B(
            bool x = false,
            bool y = false,
            bool z = false)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public bool this[int index]
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

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[3];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 1);
            BitConverter.GetBytes(Z).CopyTo(bytes, 2);
            return bytes;
        }

        public static Vec3B FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 3)
                throw new ArgumentException($"Byte array must contain at least 3 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Vec3B
            {
                X = Convert.ToBoolean(bytes[startIndex + 0]),
                Y = Convert.ToBoolean(bytes[startIndex + 1]),
                Z = Convert.ToBoolean(bytes[startIndex + 2])
            };
        }

        public override string ToString()
        {
            return $"[{X}, {Y}, {Z}]";
        }
    }
}
