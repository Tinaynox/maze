using System;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec4B
    {
        public bool X;
        public bool Y;
        public bool Z;
        public bool W;

        public static Vec4B False => new Vec4B(false, false, false, false);
        public static Vec4B True => new Vec4B(true, true, true, true);

        public Vec4B(
            bool x = false,
            bool y = false,
            bool z = false,
            bool w = false)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
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
                else if (index == 3)
                    return W;
                else
                    throw new IndexOutOfRangeException("Index must be between 0 and 3.");
            }
        }

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[4];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 1);
            BitConverter.GetBytes(Z).CopyTo(bytes, 2);
            BitConverter.GetBytes(W).CopyTo(bytes, 3);
            return bytes;
        }

        public static Vec4B FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 4)
                throw new ArgumentException($"Byte array must contain at least 4 bytes! bytes={bytes}({bytes?.Length}), startIndex={startIndex}");

            return new Vec4B
            {
                X = Convert.ToBoolean(bytes[startIndex + 0]),
                Y = Convert.ToBoolean(bytes[startIndex + 1]),
                Z = Convert.ToBoolean(bytes[startIndex + 2]),
                W = Convert.ToBoolean(bytes[startIndex + 3])
            };
        }

        public override string ToString()
        {
            return $"[{X}, {Y}, {Z}, {W}]";
        }
    }
}
