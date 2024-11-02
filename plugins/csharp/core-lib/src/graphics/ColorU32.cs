using System;

namespace Maze.Graphics
{
    public struct ColorU32
    {
        public byte R;
        public byte G;
        public byte B;
        public byte A;

        public ColorU32(
            byte _r = 0,
            byte _g = 0,
            byte _b = 0,
            byte _a = 0)
        {
            R = _r;
            G = _g;
            B = _b;
            A = _a;
        }
    }
}
