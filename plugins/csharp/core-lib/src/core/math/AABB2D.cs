using System;
using System.Runtime.InteropServices;


namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct AABB2D
    {
        public float MinX;
        public float MinY;
        public float MaxX;
        public float MaxY;

        public AABB2D(
            float minX = 0.0f,
            float minY = 0.0f,
            float maxX = 0.0f,
            float maxY = 0.0f)
        {
            MinX = minX;
            MinY = minY;
            MaxX = maxX;
            MaxY = maxY;
        }

        public override string ToString()
        {
            return $"[{MinX}, {MinY}][{MaxX}, {MaxY}]";
        }

        public string ToStringCompact()
        {
            return $"{MinX}, {MinY}, {MaxX}, {MaxY}";
        }
    }
}
