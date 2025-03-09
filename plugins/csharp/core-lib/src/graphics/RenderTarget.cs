using Maze.Core;

namespace Maze.Graphics
{
    public class RenderTarget : IndexedResource
    {
        public RenderTarget(int resourceId = -1)
        {
            m_ResourceId = resourceId;
        }

        ~RenderTarget()
        {
            ReleaseResource();
        }

        public override void ReleaseResource() {}
    }
}
