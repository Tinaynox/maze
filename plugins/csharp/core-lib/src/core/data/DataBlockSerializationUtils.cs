using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Maze.Core
{
    public static class DataBlockSerializationUtils
    {
        static public byte[] CommentPrefix = new byte[] { (byte)'@', 0xC };
        static public byte CommentSuffixCpp = 0x1;
        static public byte CommentSuffixC = 0x2;
        static public byte CommentEndlineSuffixCpp = 0x11;
        static public byte CommentEndlineSuffixC = 0x12;
        static public byte[] CommentCpp = new byte[] { CommentPrefix[0], CommentPrefix[1], CommentSuffixCpp };
        static public byte[] CommentC = new byte[] { CommentPrefix[0], CommentPrefix[1], CommentSuffixC };
        static public byte[] CommentEndlineCpp = new byte[] { CommentPrefix[0], CommentPrefix[1], CommentEndlineSuffixCpp };
        static public byte[] CommentEndlineC = new byte[] { CommentPrefix[0], CommentPrefix[1], CommentEndlineSuffixC };

        static public string CommentPrefixString = Encoding.ASCII.GetString(CommentPrefix);
        static public string CommentCppString = Encoding.ASCII.GetString(CommentCpp);
        static public string CommentCString = Encoding.ASCII.GetString(CommentC);
        static public string CommentEndlineCppString = Encoding.ASCII.GetString(CommentCpp);
        static public string CommentEndlineCString = Encoding.ASCII.GetString(CommentC);

        static public bool IsDataBlockIdentifierChar(byte c)
        {
            return (
                (c >= 'a' && c <= 'z') ||
                (c >= 'A' && c <= 'Z') ||
                (c >= '0' && c <= '9') ||
                c == '_' ||
                c == '-' ||
                c == '.' ||
                c == '~');
        }
    }
}
