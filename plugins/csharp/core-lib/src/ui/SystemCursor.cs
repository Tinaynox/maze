using Maze.Core;

namespace Maze.UI
{
    public class SystemCursor : IndexedResource
    {
        public SystemCursor(int resourceId = -1)
        {
            m_ResourceId = resourceId;
        }

        public static SystemCursor Get(int resourceId)
        {
            if (InternalCalls.SystemCursorIsValid(resourceId))
                return new SystemCursor(resourceId);

            return null;
        }

        public static SystemCursor GetOrLoad(string systemCursorName)
        {
            int resourceId = InternalCalls.SystemCursorGetOrLoad(systemCursorName);
            if (resourceId == -1)
                return null;

            return new SystemCursor(resourceId);
        }

        public override void ReleaseResource() { }

    }
}
