using System;
using System.Runtime.InteropServices;

namespace Maze.Graphics
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ColorU32
    {
        public byte R;
        public byte G;
        public byte B;
        public byte A;


        public static ColorU32 Zero => new ColorU32(0, 0, 0, 0);
        public static ColorU32 White => new ColorU32(255, 255, 255, 255);
        public static ColorU32 WhiteSoft => new ColorU32(252, 252, 252, 255);
        public static ColorU32 Gray => new ColorU32(127, 127, 127, 255);
        public static ColorU32 LightGray => new ColorU32(191, 191, 191, 255);
        public static ColorU32 DarkGray => new ColorU32(64, 64, 64, 255);
        public static ColorU32 Black => new ColorU32(0, 0, 0, 255);
        public static ColorU32 BlackSoft => new ColorU32(7, 9, 9, 255);
        public static ColorU32 Red => new ColorU32(255, 0, 0, 255);
        public static ColorU32 Green => new ColorU32(0, 255, 0, 255);
        public static ColorU32 Blue => new ColorU32(0, 0, 255, 255);
        public static ColorU32 Cyan => new ColorU32(0, 255, 255, 255);
        public static ColorU32 Magenta => new ColorU32(255, 0, 255, 255);
        public static ColorU32 Yellow => new ColorU32(255, 255, 0, 255);

        public ColorU32(
            byte r = 0,
            byte g = 0,
            byte b = 0,
            byte a = 0)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public override string ToString()
        {
            return $"{R}, {G}, {B}, {A}";
        }
    }
}
