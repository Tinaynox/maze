using System;
using System.Runtime.InteropServices;


namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct AABB3D
    {
        public float MinX;
        public float MinY;
        public float MinZ;
        public float MaxX;
        public float MaxY;
        public float MaxZ;

        public AABB3D(
            float minX = 0.0f,
            float minY = 0.0f,
            float minZ = 0.0f,
            float maxX = 0.0f,
            float maxY = 0.0f,
            float maxZ = 0.0f)
        {
            MinX = minX;
            MinY = minY;
            MinZ = minZ;
            MaxX = maxX;
            MaxY = maxY;
            MaxZ = maxZ;
        }

        public override string ToString()
        {
            return $"[{MinX}, {MinY}, {MinZ}][{MaxX}, {MaxY}, {MaxZ}]";
        }

        public string ToStringCompact()
        {
            return $"{MinX}, {MinY}, {MinZ}, {MaxX}, {MaxY}, {MaxZ}";
        }
    }
}
