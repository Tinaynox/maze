using System;
using System.Runtime.InteropServices;


namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct AABB3D
    {
        public static AABB3D Zero => new AABB3D();

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

        public static bool operator ==(AABB3D value0, AABB3D value1)
        {
            return
                value0.MinX == value1.MinX && value0.MinY == value1.MinY && value0.MinZ == value1.MinZ &&
                value0.MaxX == value1.MaxX && value0.MaxY == value1.MaxY && value0.MaxZ == value1.MaxZ;
        }

        public static bool operator !=(AABB3D value0, AABB3D value1)
        {
            return
                value0.MinX != value1.MinX || value0.MinY != value1.MinY || value0.MinZ != value1.MinZ ||
                value0.MaxX != value1.MaxX || value0.MaxY != value1.MaxY || value0.MaxZ != value1.MaxZ;
        }

        public override bool Equals(object obj)
        {
            if (obj is AABB3D other)
                return this == other;
            return false;
        }

        public override int GetHashCode()
        {
            unchecked // Handle arithmetic overflow
            {
                int hash = 17;
                hash = hash * 23 + MinX.GetHashCode();
                hash = hash * 23 + MinY.GetHashCode();
                hash = hash * 23 + MinZ.GetHashCode();
                hash = hash * 23 + MaxX.GetHashCode();
                hash = hash * 23 + MaxY.GetHashCode();
                hash = hash * 23 + MaxZ.GetHashCode();
                return hash;
            }
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
