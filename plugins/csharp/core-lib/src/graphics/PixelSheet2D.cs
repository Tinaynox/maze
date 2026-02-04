using Maze.Core;

namespace Maze.Graphics
{
    public class PixelSheet2D : IndexedResource
    {
        public Vec2S Size
        {
            get { InternalCalls.PixelSheet2DGetSize(m_ResourceId, out Vec2S size); return size; }
            set => InternalCalls.PixelSheet2DSetSize(m_ResourceId, ref value);
        }
        public int Width => Size.X;
        public int Height => Size.Y;

        public PixelFormat Format
        {
            get => (PixelFormat)InternalCalls.PixelSheet2DGetFormat(m_ResourceId);
            set => InternalCalls.PixelSheet2DSetFormat(m_ResourceId, (int)value);
        }


        public PixelSheet2D()
        {
            m_ResourceId = InternalCalls.CreatePixelSheet2D();
            m_OwnsResourceId = true;
        }

        public PixelSheet2D(
            int resourceId,
            bool ownsResourceId = false)
        {
            m_ResourceId = resourceId;
            m_OwnsResourceId = ownsResourceId;
        }

        ~PixelSheet2D()
        {
            ReleaseResource();
        }

        public override void ReleaseResource()
        {
            if (m_OwnsResourceId && m_ResourceId != -1)
            {
                InternalCalls.DestroyPixelSheet2D(m_ResourceId);
                m_ResourceId = -1;
                m_OwnsResourceId = false;
            }
        }

        public void Fill(ColorU32 color)
        {
            InternalCalls.PixelSheet2DFillU32(m_ResourceId, color.ToUInt());
        }

        public void BlendWith(PixelSheet2D otherSheet)
        {
            InternalCalls.PixelSheet2DBlend(m_ResourceId, otherSheet.ResourceId);
        }

        public void SaveToFileAsTGA(string filename, bool resetAlpha = false)
        {
            InternalCalls.PixelSheet2DSaveToFileAsTGA(m_ResourceId, filename, resetAlpha);
        }

        public uint GetPixelU32(int x, int y) => InternalCalls.PixelSheet2DGetPixelU32(m_ResourceId, x, y);
        public void SetPixelU32(int x, int y, uint pixel) => InternalCalls.PixelSheet2DSetPixelU32(m_ResourceId, x, y, pixel);

        public ColorU32 GetPixelColorU32(int x, int y) => new ColorU32(GetPixelU32(x, y));
        public void SetPixelColorU32(int x, int y, ColorU32 color) => SetPixelU32(x, y, color.ToUInt());
    }
}
