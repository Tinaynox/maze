using Maze.Core;

namespace Maze.Graphics
{
    public class Texture2D : IndexedResource
    {
        public Texture2D()
        {
            m_ResourceId = InternalCalls.CreateTexture2D();
            m_OwnsResourceId = true;
        }

        public Texture2D(int resourceId)
        {
            m_ResourceId = resourceId;
        }

        ~Texture2D()
        {
            ReleaseResource();
        }

        public override void ReleaseResource()
        {
            if (m_OwnsResourceId && m_ResourceId != -1)
            {
                InternalCalls.DestroyTexture2D(m_ResourceId);
                m_ResourceId = -1;
                m_OwnsResourceId = false;
            }
        }

        public PixelSheet2D ReadAsPixelSheet()
        {
            int pixelSheet2DId = InternalCalls.Texture2DReadAsPixelSheet(m_ResourceId);
            if (pixelSheet2DId != -1)
                return new PixelSheet2D(pixelSheet2DId, true);

            return null;
        }

        public void SaveToFileAsTGA(string filename, bool resetAlpha = false)
        {
            PixelSheet2D pixelSheet = ReadAsPixelSheet();
            if (pixelSheet != null)
                pixelSheet.SaveToFileAsTGA(filename, resetAlpha);
        }

        public void LoadTexture(PixelSheet2D pixelSheet)
        {
            InternalCalls.Texture2DLoad(m_ResourceId, pixelSheet.ResourceId);
        }
    }
}
