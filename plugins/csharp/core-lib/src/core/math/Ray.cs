using System;
using System.Runtime.InteropServices;


namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Ray
    {
        public Vec3F Point;
        public Vec3F Direction;

        public Ray(
            Vec3F point = default,
            Vec3F direction = default)
        {
            Point = point;
            Direction = direction;
        }

        public override string ToString()
        {
            return $"{Point}{Direction}";
        }
    }
}
