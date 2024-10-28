using System;

namespace Maze
{
    public struct Vec3F
    {
        public float x;
        public float y;
        public float z;

        public Vec3F(
            float _x = 0.0f,
            float _y = 0.0f,
            float _z = 0.0f)
        {
            x = _x;
            y = _y;
            z = _z;
        }
    }
}
