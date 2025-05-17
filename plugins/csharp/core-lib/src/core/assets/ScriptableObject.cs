using System;
using System.Runtime.InteropServices;
using Maze;

namespace Maze.Core
{
    public class ScriptableObject : IndexedResource
    {
        public ScriptableObject(int resourceId = -1)
        {
            m_ResourceId = resourceId;
        }

        public override void ReleaseResource() { }
    }
}
