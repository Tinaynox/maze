using Maze.Core;

namespace Maze.Graphics
{
    public class Sprite : IndexedResource
    {
        public Sprite()
        {
            m_ResourceId = InternalCalls.CreateSprite();
            m_OwnsResourceId = true;
        }

        public Sprite(int resourceId)
        {
            m_ResourceId = resourceId;
        }

        ~Sprite()
        {
            ReleaseResource();
        }

        public override void ReleaseResource()
        {
            if (m_OwnsResourceId && m_ResourceId != -1)
            {
                InternalCalls.DestroySprite(m_ResourceId);
                m_ResourceId = -1;
                m_OwnsResourceId = false;
            }
        }
    }
}
