using System;

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

            ByteBuffer byteBuffer = new ByteBuffer();
            byteBuffer.Resize((int)size);

            Debug.LogError($"size = {size}");

            return byteBuffer;
        }

        public static bool operator ==(AssetFile value0, AssetFile value1)
        {
            return value0.m_Id == value1.m_Id;
        }

        public static bool operator !=(AssetFile value0, AssetFile value1)
        {
            return value0.m_Id != value1.m_Id;
        }
    }
}
