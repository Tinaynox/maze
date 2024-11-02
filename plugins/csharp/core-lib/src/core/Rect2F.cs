using System;

namespace Maze.Core
{
    public struct Rect2F
    {
        public float X;
        public float Y;
        public float W;
        public float H;

        public Rect2F(
            float _x = 0.0f,
            float _y = 0.0f,
            float _w = 0.0f,
            float _h = 0.0f)
        {
            X = _x;
            Y = _y;
            W = _w;
            H = _h;
        }
    }
}
