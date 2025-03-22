using System;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec2B
    {
        public bool X;
        public bool Y;

        public static Vec2B False => new Vec2B(false, false);
        public static Vec2B True => new Vec2B(true, true);

        public Vec2B(
            bool x = false,
            bool y = false)
        {
            X = x;
            Y = y;
        }

        public byte[] GetBytes()
        {
            byte[] bytes = new byte[2];
            BitConverter.GetBytes(X).CopyTo(bytes, 0);
            BitConverter.GetBytes(Y).CopyTo(bytes, 1);
            return bytes;
        }

        public static Vec2B FromBytes(byte[] bytes, int startIndex = 0)
        {
            if (bytes == null || bytes.Length - startIndex < 2)
                throw new ArgumentException("Byte array must contain at least 2 bytes");

            return new Vec2B
            {
                X = Convert.ToBoolean(bytes[startIndex + 0]),
                Y = Convert.ToBoolean(bytes[startIndex + 1])
            };
        }

        public override string ToString()
        {
            return $"[{X}, {Y}]";
        }
    }
}
