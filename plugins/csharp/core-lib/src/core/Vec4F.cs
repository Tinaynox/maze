using System;

namespace Maze.Core
{
    public struct Vec4F
    {
        public float X;
        public float Y;
        public float Z;
        public float W;

        public Vec4F(
            float _x = 0.0f,
            float _y = 0.0f,
            float _z = 0.0f,
            float _w = 0.0f)
        {
            X = _x;
            Y = _y;
            Z = _z;
            W = _w;
        }
    }
}
