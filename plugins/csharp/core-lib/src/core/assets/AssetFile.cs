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

        // Enumerates files inside a virtual asset folder (e.g. "Assets/Localization.mzap"), filtered by extension if given.
        // Works for both loose folders and obfuscated/archived .mzap packs.
        public static AssetFile[] GetFilesInFolder(string folderFullPath, string extension = null)
        {
            uint[] ids = InternalCalls.AssetManagerGetAssetFilesInFolder(folderFullPath, extension);
            if (ids == null)
                return default(AssetFile[]);

            AssetFile[] result = new AssetFile[ids.Length];
            for (int i = 0; i < ids.Length; ++i)
                result[i] = new AssetFile(new AssetFileId(ids[i]));

            return result;
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

        public string GetFileName()
        {
            return InternalCalls.AssetFileGetFileName(m_Id.Id);
        }

        public string ReadAsString()
        {
            return InternalCalls.AssetFileReadAsString(m_Id.Id);
        }
    }
}
