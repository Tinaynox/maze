using System;

namespace Maze.Graphics
{
    public struct ColorF128
    {
        public float R;
        public float G;
        public float B;
        public float A;

        public ColorF128(
            float _r = 0.0f,
            float _g = 0.0f,
            float _b = 0.0f,
            float _a = 0.0f)
        {
            R = _r;
            G = _g;
            B = _b;
            A = _a;
        }
    }
}
