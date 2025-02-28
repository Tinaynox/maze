using System;

namespace Maze.Core
{
    public abstract class IndexedResource
    {
        protected int m_ResourceId = -1;
        public int ResourceId => m_ResourceId;

        public abstract void ReleaseResource();
    }
}
