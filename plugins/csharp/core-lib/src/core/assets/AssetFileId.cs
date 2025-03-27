using System;

namespace Maze.Core
{
    public struct AssetFileId
    {
        private uint m_Id;

        public AssetFileId(uint id)
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

        public static bool operator ==(AssetFileId value0, AssetFileId value1)
        {
            return value0.m_Id == value1.m_Id;
        }

        public static bool operator !=(AssetFileId value0, AssetFileId value1)
        {
            return value0.m_Id != value1.m_Id;
        }
    }
}
