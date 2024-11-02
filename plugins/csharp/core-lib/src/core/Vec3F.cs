using System;

namespace Maze.Core
{
    public struct Vec3F
    {
        public float X;
        public float Y;
        public float Z;

        public Vec3F(
            float _x = 0.0f,
            float _y = 0.0f,
            float _z = 0.0f)
        {
            X = _x;
            Y = _y;
            Z = _z;
        }
    }
}
