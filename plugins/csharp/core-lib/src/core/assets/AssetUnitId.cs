using System;

namespace Maze.Core
{
    public struct AssetUnitId
    {
        private uint m_Id;

        public AssetUnitId(uint id)
        {
            m_Id = id;
        }

        public override bool Equals(object obj)
        {
            return m_Id.Equals(obj);
        }

        public override int GetHashCode()
        {
            return m_Id.GetHashCode();
        }

        public static bool operator ==(AssetUnitId value0, AssetUnitId value1)
        {
            return value0.m_Id == value1.m_Id;
        }

        public static bool operator !=(AssetUnitId value0, AssetUnitId value1)
        {
            return value0.m_Id != value1.m_Id;
        }
    }
}
