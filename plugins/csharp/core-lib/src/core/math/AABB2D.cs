using System;
using System.Runtime.InteropServices;


namespace Maze.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct AABB2D
    {
        public static AABB2D Zero => new AABB2D();

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

        public AABB2D(Vec2F point)
        {
            MinX = point.X;
            MinY = point.Y;
            MaxX = point.X;
            MaxY = point.Y;
        }

        public AABB2D(Vec2F point0, Vec2F point1)
        {
            MinX = Math.Min(point0.X, point1.X);
            MinY = Math.Min(point0.Y, point1.Y);
            MaxX = Math.Max(point0.X, point1.X);
            MaxY = Math.Max(point0.Y, point1.Y);
        }

        public static bool operator ==(AABB2D value0, AABB2D value1)
        {
            return
                value0.MinX == value1.MinX && value0.MinY == value1.MinY &&
                value0.MaxX == value1.MaxX && value0.MaxY == value1.MaxY;
        }

        public static bool operator !=(AABB2D value0, AABB2D value1)
        {
            return
                value0.MinX != value1.MinX || value0.MinY != value1.MinY ||
                value0.MaxX != value1.MaxX || value0.MaxY != value1.MaxY;
        }

        public override bool Equals(object obj)
        {
            if (obj is AABB2D other)
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
                hash = hash * 23 + MaxX.GetHashCode();
                hash = hash * 23 + MaxY.GetHashCode();
                return hash;
            }
        }

        public void ApplyIntersection(AABB2D other)
        {
            MinX = Math.Max(MinX, other.MinX);
            MinY = Math.Max(MinY, other.MinY);
            MaxX = Math.Min(MaxX, other.MaxX);
            MaxY = Math.Min(MaxY, other.MaxY);
        }

        public void ApplyUnion(AABB2D other)
        {
            MinX = Math.Min(MinX, other.MinX);
            MinY = Math.Min(MinY, other.MinY);
            MaxX = Math.Max(MaxX, other.MaxX);
            MaxY = Math.Max(MaxY, other.MaxY);
        }

        public void ApplyUnion(Vec2F point)
        {
            MinX = Math.Min(MinX, point.X);
            MinY = Math.Min(MinY, point.Y);
            MaxX = Math.Max(MaxX, point.X);
            MaxY = Math.Max(MaxY, point.Y);
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
