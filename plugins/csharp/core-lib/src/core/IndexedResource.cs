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

        public abstract void ReleaseResource();
    }
}
