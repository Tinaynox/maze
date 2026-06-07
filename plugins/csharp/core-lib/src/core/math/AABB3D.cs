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

        public Vec3F Min => new Vec3F(MinX, MinY, MinZ);
        public Vec3F Max => new Vec3F(MaxX, MaxY, MaxZ);

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

        public bool Contains(Vec3F p) =>
            p.X >= MinX && p.X <= MaxX &&
            p.Y >= MinY && p.Y <= MaxY &&
            p.Z >= MinZ && p.Z <= MaxZ;

        public bool IntersectsSphere(Vec3F p, float r)
        {
            var dx = Math.Max(Math.Max(MinX - p.X, 0.0f), p.X - MaxX);
            var dy = Math.Max(Math.Max(MinY - p.Y, 0.0f), p.Y - MaxY);
            var dz = Math.Max(Math.Max(MinZ - p.Z, 0.0f), p.Z - MaxZ);
            return dx * dx + dy * dy + dz * dz <= r * r;
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
