using Maze.Core;

namespace Maze.Graphics
{
    public class RenderWindow : RenderTarget
    {
        public RenderWindow(int resourceId = -1)
            : base(resourceId)
        {
        }

        public static RenderWindow Create(Vec2U windowSize, string title)
        {
            int resourceId = InternalCalls.CreateRenderWindow(ref windowSize, title);
            return new RenderWindow(resourceId);
        }

        public override void ReleaseResource()
        {
            if (m_ResourceId != -1)
            {
                InternalCalls.DestroyRenderWindow(m_ResourceId);
                m_ResourceId = -1;
            }
        }

    }
}
