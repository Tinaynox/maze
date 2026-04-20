using Maze.Core;

namespace Maze.Graphics
{
    public class Sprite : IndexedResource
    {
        public Vec2F ColorOffset { get { InternalCalls.SpriteGetColorOffset(m_ResourceId, out Vec2F outValue); return outValue; } }
        public Vec2F ColorPosition { get { InternalCalls.SpriteGetColorPosition(m_ResourceId, out Vec2F outValue); return outValue; } }
        public Vec2F ColorSize { get { InternalCalls.SpriteGetColorSize(m_ResourceId, out Vec2F outValue); return outValue; } }
        public Vec2F NativeSize { get { InternalCalls.SpriteGetNativeSize(m_ResourceId, out Vec2F outValue); return outValue; } }

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

        public static Sprite Get(int resourceId)
        {
            if (InternalCalls.SpriteIsValid(resourceId))
                return new Sprite(resourceId);

            return null;
        }

        public static Sprite Get(string spriteName)
        {
            if (InternalCalls.SpriteGetId(spriteName, out int outSpriteId))
                return new Sprite(outSpriteId);

            return null;
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
