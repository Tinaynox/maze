using System;

namespace Maze.Core
{
    public abstract class IndexedResource
    {
        protected int m_ResourceId = -1;
        public int ResourceId
        {
            get => m_ResourceId;
            private set => m_ResourceId = value;
        }

        protected bool m_OwnsResourceId = false;

        public abstract void ReleaseResource();

        ~IndexedResource()
        {
            if (m_OwnsResourceId && m_ResourceId != -1)
            {
                Debug.LogError($"{GetType()}: owned resource id({m_ResourceId}) wasn't released!");
            }
        }
    }
}
