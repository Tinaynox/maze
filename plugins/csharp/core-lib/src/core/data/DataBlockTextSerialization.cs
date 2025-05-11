using System;
using System.Collections.Generic;
using System.Text;

namespace Maze.Core
{
    public static class DataBlockTextSerialization
    {
        public enum DataBlockTextFlags : uint
        {
            Compact = 0x1
        }

        static void WriteSimpleString(ByteBufferWriteStream stream, byte[] value)
        {
            stream.Write(value);
        }

        static void WriteComplexString(ByteBufferWriteStream stream, byte[] value)
        {
            byte[] quote = new byte[] { (byte)'"' };
            int quoteLen = 1;

            bool hasLineBreak = false;
            bool hasQuote = false;
            bool hasTick = false;

            foreach (byte c in value)
            {
                if (c == '\n' || c == '\r')
                    hasLineBreak = true;
                else if (c == '\"')
                    hasQuote = true;
                else if (c == '\'')
                    hasTick = true;
                else
                    continue;

                if (hasLineBreak && hasQuote && hasTick)
                    break;
            }

            if (hasLineBreak && !hasQuote)
            {
                quote = new byte[] { (byte)'"', (byte)'"', (byte)'"', (byte)'\n', (byte)'"', (byte)'"', (byte)'"' };
                quoteLen = 4;
            }
            else if (hasLineBreak && !hasTick)
            {
                quote = new byte[] { (byte)'\'', (byte)'\'', (byte)'\'', (byte)'\n', (byte)'\'', (byte)'\'', (byte)'\'' };
                quoteLen = 4;
            }
            else if (hasLineBreak)
            {
                quote = new byte[] { (byte)'"', (byte)'"', (byte)'"', (byte)'\n', (byte)'"', (byte)'"', (byte)'"' };
                quoteLen = 4;
            }
            else if (hasQuote && !hasTick)
            {
                quote = new byte[(byte)'\''];
            }

            // Opening quote
            stream.Write(quote, quoteLen);

            for (int i = 0; i < value.Length; ++i)
            {
                byte c = value[i];

                if (c == '~')
                { 
                    stream.Write((byte)'~');
                    stream.Write((byte)'~');
                }
                else if (c == quote[0] && (quoteLen == 1 || value[i + 1] == c))
                {
                    if (c == '\"')
                    {
                        stream.Write((byte)'~');
                        stream.Write((byte)'\"');
                    }
                    else
                    {
                        stream.Write((byte)'~');
                        stream.Write((byte)'\'');
                    }
                }
                else if (c == '\r' && quoteLen == 1)
                { 
                    stream.Write('~');
                    stream.Write('r');
                }
                else if (c == '\n' && quoteLen == 1)
                {
                    stream.Write('~');
                    stream.Write('n');
                }
                else if (c == '\t')
                {
                    stream.Write('~');
                    stream.Write('t');
                }
                else
                    stream.Write(c);
            }

            // Closing quote
            stream.Write(quote, quoteLen - 1, quoteLen);
        }

        static bool WriteDataBlockText(
            ByteBufferWriteStream stram,
            DataBlock dataBlock,
            int level,
            bool compact)
        {
            Debug.LogError("NOT IMPLEMENTED");
            return false;
        }

        static bool SaveText(DataBlock dataBlock, ByteBuffer buffer, uint flags)
        {
            buffer.Clear();

            ByteBufferWriteStream writeStream = new ByteBufferWriteStream(buffer);
            var isCompact = (flags & (uint)DataBlockTextFlags.Compact) != 0u;
            WriteDataBlockText(writeStream, dataBlock, 0, isCompact);

            return true;
        }

        static bool LoadText(DataBlock dataBlock, ByteBuffer buffer)
        {
            Debug.LogError("NOT IMPLEMENTED");
            return true;
        }

        static bool SaveTextFile(DataBlock dataBlock, string path, uint flags)
        {
            ByteBuffer byteBuffer = new ByteBuffer();
            if (!SaveText(dataBlock, byteBuffer, flags))
                return false;

            return DataBlockBinarySerialization.SaveBinaryFile(byteBuffer, path);
        }

        static bool LoadTextFile(DataBlock dataBlock, string path)
        {
            ByteBuffer buffer = ByteBuffer.LoadFile(path);
            if (buffer == null)
                return false;

            return LoadText(dataBlock, buffer);
        }


        public static DataBlockParamType GetDataBlockParamType(string text)
        {
            switch (text)
            {
                case "S8": return DataBlockParamType.ParamS8;
                case "S16": return DataBlockParamType.ParamS16;
                case "S32": return DataBlockParamType.ParamS32;
                case "S64": return DataBlockParamType.ParamS64;
                case "U8": return DataBlockParamType.ParamU8;
                case "U16": return DataBlockParamType.ParamU16;
                case "U32": return DataBlockParamType.ParamU32;
                case "U64": return DataBlockParamType.ParamU64;
                case "F32": return DataBlockParamType.ParamF32;
                case "F64": return DataBlockParamType.ParamF64;
                case "Bool": return DataBlockParamType.ParamBool;
                case "Vec4S8": return DataBlockParamType.ParamVec4S8;
                case "Vec4U8": return DataBlockParamType.ParamVec4U8;
                case "Vec2S": return DataBlockParamType.ParamVec2S32;
                case "Vec3S": return DataBlockParamType.ParamVec3S32;
                case "Vec4S": return DataBlockParamType.ParamVec4S32;
                case "Vec2S32": return DataBlockParamType.ParamVec2S32;
                case "Vec3S32": return DataBlockParamType.ParamVec3S32;
                case "Vec4S32": return DataBlockParamType.ParamVec4S32;
                case "Vec2U": return DataBlockParamType.ParamVec2U32;
                case "Vec3U": return DataBlockParamType.ParamVec3U32;
                case "Vec4U": return DataBlockParamType.ParamVec4U32;
                case "Vec2U32": return DataBlockParamType.ParamVec2U32;
                case "Vec3U32": return DataBlockParamType.ParamVec3U32;
                case "Vec4U32": return DataBlockParamType.ParamVec4U32;
                case "Vec2F": return DataBlockParamType.ParamVec2F32;
                case "Vec3F": return DataBlockParamType.ParamVec3F32;
                case "Vec4F": return DataBlockParamType.ParamVec4F32;
                case "Vec2F32": return DataBlockParamType.ParamVec2F32;
                case "Vec3F32": return DataBlockParamType.ParamVec3F32;
                case "Vec4F32": return DataBlockParamType.ParamVec4F32;
                case "Vec2B": return DataBlockParamType.ParamVec2B;
                case "Vec3B": return DataBlockParamType.ParamVec3B;
                case "Vec4B": return DataBlockParamType.ParamVec4B;
                case "Mat3F": return DataBlockParamType.ParamMat3F32;
                case "Mat3F32": return DataBlockParamType.ParamMat3F32;
                case "Mat4F": return DataBlockParamType.ParamMat4F32;
                case "Mat4F32": return DataBlockParamType.ParamMat4F32;
                case "TMat": return DataBlockParamType.ParamTMat;
                case "String": return DataBlockParamType.ParamString;
            }

            return DataBlockParamType.None;
        }
    }

    class DataBlockTextParser
    {
        enum Statement
        {
            None,
            Param,
            Block
        };

        struct PendingComment
        {
            public int StartOffset;
            public int EndOffset;
            public bool CppStyle;

            public PendingComment(
                int startOffset = 0,
                int endOffset = 0,
                bool cppStyle = false)
            {
                StartOffset = startOffset;
                EndOffset = endOffset;
                CppStyle = cppStyle;
            }
        };

        DataBlock m_DataBlock;
        ByteBufferReadStream m_ReadStream;
        int m_CurrentLine = 1;
        int m_CurrentLineOffset = 0;

        bool m_WasNewLineAfterStatement = false;
        Statement m_LastStatement = Statement.None;

        List<PendingComment> m_PendingComments = new List<PendingComment>();


        List<byte> m_NameText = new List<byte>();
        List<byte> m_TypeNameText = new List<byte>();
        List<byte> m_ValueText = new List<byte>();


        public DataBlockTextParser(ByteBuffer buffer)
        {
            m_ReadStream = new ByteBufferReadStream(buffer);
        }

        bool IsEndOfBuffer() => m_ReadStream.IsEndOfBuffer();

        public bool Parse(DataBlock dataBlock)
        {
            dataBlock.Clear();
            return ParseDataBlock(dataBlock, true);
        }

        public bool ParseDataBlock(DataBlock dataBlock, bool isTopmost)
        {
            m_DataBlock = dataBlock;

            m_NameText.Clear();
            m_TypeNameText.Clear();
            m_ValueText.Clear();

            while (true)
            {
                if (!SkipWhite(true))
                    return false;

                if (IsEndOfBuffer())
                {
                    if (!isTopmost)
                    {
                        ProcessSyntaxError("Unexpected EOF");
                        return false;
                    }

                    break;
                }

                if (ReadCharNoRewind() == '}')
                {
                    if (isTopmost)
                    {
                        ProcessSyntaxError("Unexpected '}' in topmost block");
                        return false;
                    }
                    m_ReadStream.Rewind(1);

                    FlushPendingComments(dataBlock, false);
                    break;
                }

                m_NameText.Clear();
                if (!ParseIdentifier(m_NameText))
                {
                    ProcessSyntaxError("Expected identifier");
                    return false;
                }

                if (!SkipWhite())
                    return false;

                if (IsEndOfBuffer())
                {
                    ProcessSyntaxError("Unexpected EOF");
                    return false;
                }

                if (ReadCharNoRewind() == '{')
                {
                    m_ReadStream.Rewind(1);
                    m_WasNewLineAfterStatement = false;
                    m_LastStatement = Statement.None;
                    FlushPendingComments(dataBlock, false);

                    DataBlock newBlock = dataBlock.AddNewDataBlock(
                        Encoding.ASCII.GetString(m_NameText.ToArray()));

                    if (newBlock != null)
                    {
                        if (!ParseDataBlock(newBlock, false))
                            return false;
                    }

                    m_LastStatement = Statement.None;
                }
                else if (ReadCharNoRewind() == ':')
                {
                    FlushPendingComments(dataBlock, true);

                    m_ReadStream.Rewind(1);
                    m_TypeNameText.Clear();
                    if (!ParseIdentifier(m_TypeNameText))
                    {
                        ProcessSyntaxError("Expected type identifier");
                        return false;
                    }

                    DataBlockParamType paramType = DataBlockTextSerialization.GetDataBlockParamType(
                        Encoding.ASCII.GetString(m_TypeNameText.ToArray()));
                    if (paramType == DataBlockParamType.None)
                    {
                        ProcessSyntaxError("Unknown type");
                        return false;
                    }

                    if (!SkipWhite())
                        return false;

                    if (IsEndOfBuffer())
                    {
                        ProcessSyntaxError("Unexpected EOF");
                        return false;
                    }

                    if (ReadChar() != '=')
                    {
                        ProcessSyntaxError("Expected '='");
                        return false;
                    }

                    if (!SkipWhite())
                        return false;

                    byte nextCh = ReadCharNoRewind();
                    if (nextCh == '\r' || nextCh == '\n')
                    {
                        ProcessSyntaxError("Unexpected CR/LF");
                        return false;
                    }

                    if (IsEndOfBuffer())
                    {
                        ProcessSyntaxError("Unexpected EOF");
                        return false;
                    }

                    m_ValueText.Clear();
                    if (!ParseValue(m_ValueText))
                        return false;

                    if (!AddParam(dataBlock, Encoding.ASCII.GetString(m_NameText.ToArray()), paramType, m_ValueText))
                        return false;

                    m_WasNewLineAfterStatement = false;
                    m_LastStatement = Statement.Param;
                }
                else if (ReadCharNoRewind() == '=')
                {
                    Debug.LogError("Not implemented");
                }
                else
                {
                    ProcessSyntaxError("Syntax error!");
                    return false;
                }
            }

            return true;
        }

        bool SkipWhite(bool allowCRLF = true, bool trackNewLineAfterParam = false)
        {
            while (true)
            {
                if (IsEndOfBuffer())
                    break;
                
                m_ReadStream.Read(out byte ch);
                if (ch == ' ' || ch == '\t' || ch == 0x1A)
                    continue;

                if (ch == 0)
                    continue;

                if (allowCRLF)
                {
                    // CR
                    if (ch == '\r')
                    {
                        m_ReadStream.ReadNoRewind(out byte nextCh);

                        if (!IsEndOfBuffer() && nextCh == '\n')
                        {
                            m_ReadStream.Rewind(1);
                            IncCurrentLine();
                            if (trackNewLineAfterParam)
                                m_WasNewLineAfterStatement = true;
                        }
                        continue;
                    }
                    // LF
                    else if (ch == '\n')
                    {
                        IncCurrentLine();
                        if (trackNewLineAfterParam)
                            m_WasNewLineAfterStatement = true;
                        continue;
                    }
                }

                if (ch == '/')
                {
                    if (!IsEndOfBuffer())
                    {
                        m_ReadStream.Read(out byte nextCh);
                        if (nextCh == '/')
                        {
                            int cppCommentStartOffset = m_ReadStream.Offset;

                            while (!IsEndOfBuffer())
                            {
                                m_ReadStream.Read(out byte nextNextCh);
                                if (nextNextCh == '\r' || nextNextCh == '\n')
                                    break;
                            }

                            if (m_WasNewLineAfterStatement || m_LastStatement == Statement.None)
                                m_PendingComments.Add(new PendingComment(cppCommentStartOffset, m_ReadStream.Offset - 1, true));
                            else if (m_LastStatement == Statement.Param)
                            {
                                m_ReadStream.ReadNoRewind(
                                    out byte[] commentBuffer,
                                    m_ReadStream.Offset - 1 - cppCommentStartOffset,
                                    cppCommentStartOffset);
                                m_DataBlock.AddString(
                                    DataBlockSerializationUtils.CommentEndlineCppString,
                                    Encoding.ASCII.GetString(commentBuffer));
                            }
                            else
                            {
                                m_ReadStream.ReadNoRewind(
                                    out byte[] commentBuffer,
                                    m_ReadStream.Offset - 1 - cppCommentStartOffset,
                                    cppCommentStartOffset);
                                m_DataBlock.AddNewDataBlock(DataBlockSerializationUtils.CommentEndlineCppString).AddString(
                                    DataBlockSerializationUtils.CommentEndlineCppString,
                                    Encoding.ASCII.GetString(commentBuffer));
                            }

                            continue;
                        }
                        else if (nextCh == '*')
                        {
                            int cCommentStartOffset = m_ReadStream.Offset;
                            int count = 1;

                            while (m_ReadStream.CanRead(2))
                            {
                                byte[] nextNextChars = new byte[2];
                                m_ReadStream.ReadNoRewind(nextNextChars, nextNextChars.Length);
                                
                                if (nextNextChars[0] == '/' && nextNextChars[1] == '*')
                                {
                                    m_ReadStream.Rewind(2);
                                    ++count;
                                }
                                else if (nextNextChars[0] == '*' && nextNextChars[1] == '/')
                                {
                                    m_ReadStream.Rewind(2);
                                    if (--count <= 0)
                                        break;
                                }
                                else
                                {
                                    m_ReadStream.Rewind(1);
                                }
                            }
                            
                            if (count > 0 && !m_ReadStream.CanRead(2))
                            {
                                ProcessSyntaxError("Unexpected EOF inside comment");
                                return false;
                            }

                            if (m_WasNewLineAfterStatement || m_LastStatement == Statement.None)
                                m_PendingComments.Add(new PendingComment(cCommentStartOffset, m_ReadStream.Offset - 2, false));
                            else if (m_LastStatement == Statement.Param)
                            {
                                m_ReadStream.ReadNoRewind(
                                    out byte[] commentBuffer,
                                    m_ReadStream.Offset - 2 - cCommentStartOffset,
                                    cCommentStartOffset);
                                m_DataBlock.AddString(
                                    DataBlockSerializationUtils.CommentEndlineCString,
                                    Encoding.ASCII.GetString(commentBuffer));
                            }
                            else
                            {
                                m_ReadStream.ReadNoRewind(
                                    out byte[] commentBuffer,
                                    m_ReadStream.Offset - 2 - cCommentStartOffset,
                                    cCommentStartOffset);
                                m_DataBlock.AddNewDataBlock(DataBlockSerializationUtils.CommentEndlineCString).AddString(
                                    DataBlockSerializationUtils.CommentEndlineCString,
                                    Encoding.ASCII.GetString(commentBuffer));
                            }

                            continue;
                        }
                        else
                        {
                            m_ReadStream.Rewind(-1);
                        }
                    }

                    m_ReadStream.Rewind(-1);
                    break;
                }
                else
                {
                    m_ReadStream.Rewind(-1);
                    break;
                }
            }

            return true;
        }

        bool ParseIdentifier(List<byte> value)
        {
            Debug.LogError("NOT IMPLEMENTED");
            return false;
        }

        bool ParseValue(List<byte> value)
        {
            value.Clear();

            bool multiLineString = false;

            Debug.LogError("NOT IMPLEMENTED");
            return true;
        }

        void IncCurrentLine()
        {
            ++m_CurrentLine;
            m_CurrentLineOffset = m_ReadStream.Offset;
        }

        void ProcessSyntaxError(string message)
        {
            Debug.LogError($"{message} (Line: {m_CurrentLine})");
        }

        byte ReadCharNoRewind(int index = 0)
        {
            if (index >= 3)
            { 
                Debug.LogError($"Index is out of bounds: {index}");
                return 0;
            }

            byte[] ch = new byte[3];
            m_ReadStream.ReadNoRewind(ch, 3);
            return ch[index];
        }

        byte ReadChar()
        {
            m_ReadStream.Read(out byte ch);
            return ch;
        }

        void FlushPendingComments(DataBlock dataBlock, bool toParams)
        {
            foreach (PendingComment pendingComment in m_PendingComments)
            {
                string commentKey = pendingComment.CppStyle ? DataBlockSerializationUtils.CommentCppString : DataBlockSerializationUtils.CommentCString;

                int commentSize = pendingComment.EndOffset - pendingComment.StartOffset;
                byte[] commentBuffer = new byte [commentSize];
                Buffer.BlockCopy(m_ReadStream.ByteBufferData, pendingComment.StartOffset, commentBuffer, 0, commentSize);

                if (toParams)
                    dataBlock.AddString(commentKey, commentBuffer);
                else
                    dataBlock.AddNewDataBlock(commentKey).AddString(
                        commentKey, commentBuffer);
            }

            m_PendingComments.Clear();
        }

        bool AddParam(
            DataBlock dataBlock,
            string name,
            DataBlockParamType type,
            List<byte> value)
        {
            int itemId = dataBlock.FindParamIndex(dataBlock.Shared.GetStringId(name));

            if (itemId >= 0 && dataBlock.GetParam(itemId).Type != type)
            {
                ProcessSyntaxError($"Invalid type - {dataBlock.GetParam(itemId).Type} != {type}");
                return false;
            }

            if (value.Count == 0)
            {
                ProcessSyntaxError($"Param '{name}' is empty");
                return false;
            }

            if (type == DataBlockParamType.ParamString)
            {
                dataBlock.AddString(name, Encoding.ASCII.GetString(value.ToArray()));
            }
            else
            {
                switch (type)
                {
                    case DataBlockParamType.ParamS8:
                    {
                        if (StringHelper.ParseInteger(value, 0, value.Count, out sbyte paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddS8(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamS16:
                    {
                        if (StringHelper.ParseInteger(value, 0, value.Count, out short paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddS16(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamS32:
                    {
                        if (StringHelper.ParseInteger(value, 0, value.Count, out int paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddS32(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamS64:
                    {
                        if (StringHelper.ParseInteger(value, 0, value.Count, out long paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddS64(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamU8:
                    {
                        if (StringHelper.ParseInteger(value, 0, value.Count, out byte paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddU8(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamU16:
                    {
                        if (StringHelper.ParseInteger(value, 0, value.Count, out ushort paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddU16(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamU32:
                    {
                        if (StringHelper.ParseInteger(value, 0, value.Count, out uint paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddU32(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamU64:
                    {
                        if (StringHelper.ParseInteger(value, 0, value.Count, out ulong paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddU64(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamF32:
                    {
                        if (StringHelper.ParseFloat(value, 0, value.Count, out float paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddF32(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamF64:
                    {
                        if (StringHelper.ParseFloat(value, 0, value.Count, out double paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddF64(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamBool:
                    {
                        if (StringHelper.ParseBoolPretty(value, 0, value.Count, out bool paramValue) < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddBool(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec4S8:
                    {
                        if (Vec4S8.ParseString(value, 0, value.Count, out Vec4S8 paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec4S8(name, paramValue);
                        break;
                    }
                }
            }

            return true;
        }
    }
}
