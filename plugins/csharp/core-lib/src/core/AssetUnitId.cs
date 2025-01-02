using System;

namespace Maze.Core
{
    public struct AssetUnitId
    {
        private uint m_Id;

        public AssetUnitId(uint _id)
        {
            m_Id = _id;
        }

        public override bool Equals(object obj)
        {
            return m_Id.Equals(obj);
        }

        public override int GetHashCode()
        {
            return m_Id.GetHashCode();
        }

        public static bool operator ==(AssetUnitId value1, AssetUnitId value2)
        {
            return value1.m_Id == value2.m_Id;
        }

        public static bool operator !=(AssetUnitId value1, AssetUnitId value2)
        {
            return value1.m_Id != value2.m_Id;
        }
    }
}
