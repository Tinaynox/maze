using Maze.Core;

namespace Maze.Graphics
{
    // Mirrors Maze::TextureFilter (MazeTexture.hpp)
    public enum TextureFilter
    {
        None = 0,
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
    }

    // Mirrors Maze::TextureWrap (MazeTexture.hpp)
    public enum TextureWrap
    {
        None = 0,
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    }

    public class Texture2D : IndexedResource
    {
        public Vec2S Size
        {
            get { InternalCalls.Texture2DGetSize(m_ResourceId, out Vec2S size); return size; }
        }
        public int Width => Size.X;
        public int Height => Size.Y;

        // Asset textures are named after their asset file
        public string Name => InternalCalls.Texture2DGetName(m_ResourceId);

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

        public bool LoadEmpty(int width, int height, PixelFormat pixelFormat)
        {
            return InternalCalls.Texture2DLoadEmpty(m_ResourceId, width, height, (int)pixelFormat);
        }

        public void Reload()
        {
            InternalCalls.Texture2DReload(m_ResourceId);
        }

        public void SetFilters(TextureFilter minFilter, TextureFilter magFilter)
        {
            InternalCalls.Texture2DSetFilters(m_ResourceId, (int)minFilter, (int)magFilter);
        }

        public void SetWrap(TextureWrap wrapS, TextureWrap wrapT)
        {
            InternalCalls.Texture2DSetWrap(m_ResourceId, (int)wrapS, (int)wrapT);
        }

        // Uploads a tightly packed width x height block of pixels into the texture at (x, y).
        // pixels may be larger than required (only the first width * height * bpp bytes are read)
        public void CopyImageFrom(byte[] pixels, PixelFormat pixelFormat, int width, int height, int x, int y)
        {
            InternalCalls.Texture2DCopyImageFrom(m_ResourceId, pixels, (int)pixelFormat, width, height, x, y);
        }
    }
}
