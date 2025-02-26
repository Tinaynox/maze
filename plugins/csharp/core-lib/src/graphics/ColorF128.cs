using Maze.Core;
using System;
using System.Runtime.InteropServices;

namespace Maze.Graphics
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ColorF128
    {
        public float R;
        public float G;
        public float B;
        public float A;

        public static ColorF128 Zero => new ColorF128(0.0f, 0.0f, 0.0f, 0.0f);
        public static ColorF128 White => new ColorF128(1.0f, 1.0f, 1.0f, 1.0f);
        public static ColorF128 WhiteSoft => new ColorF128(0.988f, 0.988f, 0.988f, 1.0f);
        public static ColorF128 Gray => new ColorF128(0.5f, 0.5f, 0.5f, 1.0f);
        public static ColorF128 LightGray => new ColorF128(0.75f, 0.75f, 0.75f, 1.0f);
        public static ColorF128 DarkGray => new ColorF128(0.25f, 0.25f, 0.25f, 1.0f);
        public static ColorF128 Black => new ColorF128(0, 0, 0, 1.0f);
        public static ColorF128 BlackSoft => new ColorF128(0.027f, 0.035f, 0.035f, 1.0f);
        public static ColorF128 Red => new ColorF128(1.0f, 0, 0, 1.0f);
        public static ColorF128 Green => new ColorF128(0, 1.0f, 0, 1.0f);
        public static ColorF128 Blue => new ColorF128(0, 0, 1.0f, 1.0f);
        public static ColorF128 Cyan => new ColorF128(0, 1.0f, 1.0f, 1.0f);
        public static ColorF128 Magenta => new ColorF128(1.0f, 0, 1.0f, 1.0f);
        public static ColorF128 Yellow => new ColorF128(1.0f, 1.0f, 0, 1.0f);

        public ColorF128(
            float r = 0.0f,
            float g = 0.0f,
            float b = 0.0f,
            float a = 0.0f)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public Vec4F ToVec4F()
        {
            return new Vec4F(R, G, B, A);
        }
    }
}
