using System;

namespace Maze.Core
{
    public struct AssetUnitId
    {
        uint m_Id;
        public uint Id => m_Id;

        public static AssetUnitId InvalidId => new AssetUnitId(0u);

        public AssetUnitId(uint id)
        {
            m_Id = id;
        }

        public static AssetUnitId Get(string assetUnitName)
        {
            return new AssetUnitId(InternalCalls.GetAssetUnitId(assetUnitName));
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

        public override string ToString()
        {
            return m_Id.ToString();
        }
    }
}
