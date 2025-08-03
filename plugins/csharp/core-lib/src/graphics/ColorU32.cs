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
