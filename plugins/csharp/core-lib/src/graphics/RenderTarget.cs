using Maze.Core;

namespace Maze.Graphics
{
    public abstract class RenderTarget : IndexedResource
    {
        public RenderTarget(int resourceId = -1)
        {
            m_ResourceId = resourceId;
        }

        ~RenderTarget()
        {
            ReleaseResource();
        }

    }
}
