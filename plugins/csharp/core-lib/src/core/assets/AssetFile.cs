using System;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    public class AssetFile
    {
        AssetFileId m_Id;
        public AssetFileId Id => m_Id;

        public AssetFile(AssetFileId id)
        {
            m_Id = id;
        }

        public static AssetFile Get(AssetFileId id)
        {
            if (InternalCalls.AssetFileIdIsValid(id.Id))
                return new AssetFile(id);

            return null;
        }

        public unsafe ByteBuffer ReadAsByteBuffer()
        {
            if (!InternalCalls.AssetFileReadAsDataBlock(
                m_Id.Id,
                out uint size,
                out byte* bytes))
                return null;

            return ByteBuffer.LoadBytes(bytes, size);
        }
    }
}
