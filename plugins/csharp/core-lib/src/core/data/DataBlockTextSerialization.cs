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
            ByteBufferWriteStream stream,
            DataBlock dataBlock,
            int level,
            bool compact)
        {
            void WriteEndOfLine() => stream.WriteASCII('\n');
            void WriteIndent(int size)
            {
                for (int i = 0; i < size; ++i)
                    stream.WriteASCII(' ');
            }
            

            bool IsNameIsSimple(string name)
            {
                var result = true;
                if (name.Length == 0)
                    result = false;

                for (int i = 0; i < name.Length && result; ++i)
                    result = DataBlockSerializationUtils.IsDataBlockIdentifierChar((byte)name[i]);

                return result;
            }

            bool skipNextIndent = false;
            
            for (int i = 0; i < dataBlock.ParamsCount; ++i)
            {
                DataBlockParam param = dataBlock.GetParam(i);

                string name = dataBlock.Shared.GetString(param.NameId);
                bool nameIsSimple = IsNameIsSimple(name);

                // Indent
                if (level > 0)
                {
                    if (!skipNextIndent)
                        WriteIndent(level * 2);
                    else
                        skipNextIndent = false;
                }

                if (name.Length >= 3 && name.StartsWith(DataBlockSerializationUtils.CommentPrefixString))
                {
                    // C-style comment
                    if (name[2] == DataBlockSerializationUtils.CommentEndlineSuffixC || name[2] == DataBlockSerializationUtils.CommentSuffixC)
                    {
                        stream.WriteASCII('/');
                        stream.WriteASCII('*');
                        stream.WriteASCII(dataBlock.GetString(i));
                        stream.WriteASCII('*');
                        stream.WriteASCII('/');
                        WriteEndOfLine();
                    }
                    // CPP-style comment
                    else if (name[2] == DataBlockSerializationUtils.CommentEndlineSuffixCpp || name[2] == DataBlockSerializationUtils.CommentSuffixCpp)
                    {
                        stream.WriteASCII('/');
                        stream.WriteASCII('/');
                        stream.WriteASCII(dataBlock.GetString(i));
                        WriteEndOfLine();
                    }
                    continue;
                }

                if (nameIsSimple)
                    WriteSimpleString(stream, Encoding.ASCII.GetBytes(name));
                else
                    WriteComplexString(stream, Encoding.ASCII.GetBytes(name));

                stream.WriteASCII(':');
                stream.WriteASCII(GetDataBlockParamTypeString(param.Type));

                if (compact)
                    stream.WriteASCII('=');
                else
                {
                    stream.WriteASCII(' ');
                    stream.WriteASCII('=');
                    stream.WriteASCII(' ');
                }

                if (param.Type == DataBlockParamType.ParamString)
                {
                    WriteComplexString(stream, Encoding.ASCII.GetBytes(dataBlock.GetString(i)));
                }
                else
                {
                    switch (param.Type)
                    {
                        case DataBlockParamType.ParamS8: stream.Write(dataBlock.GetParamValue(i, (sbyte)0)); break;
                        case DataBlockParamType.ParamS16: stream.Write(dataBlock.GetParamValue(i, (short)0)); break;
                        case DataBlockParamType.ParamS32: stream.Write(dataBlock.GetParamValue(i, (int)0)); break;
                        case DataBlockParamType.ParamS64: stream.Write(dataBlock.GetParamValue(i, (long)0)); break;
                        case DataBlockParamType.ParamU8: stream.Write(dataBlock.GetParamValue(i, (byte)0)); break;
                        case DataBlockParamType.ParamU16: stream.Write(dataBlock.GetParamValue(i, (ushort)0)); break;
                        case DataBlockParamType.ParamU32: stream.Write(dataBlock.GetParamValue(i, (uint)0)); break;
                        case DataBlockParamType.ParamU64: stream.Write(dataBlock.GetParamValue(i, (ulong)0)); break;
                        case DataBlockParamType.ParamF32: stream.Write(dataBlock.GetParamValue(i, 0.0f)); break;
                        case DataBlockParamType.ParamF64: stream.Write(dataBlock.GetParamValue(i, 0.0)); break;
                        case DataBlockParamType.ParamBool: StringHelper.BoolToStringPretty(dataBlock.GetParamValue(i, false)); break;
                        case DataBlockParamType.ParamVec4S8: stream.WriteASCII(dataBlock.GetParamValue(i, Vec4S8.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec4U8: stream.WriteASCII(dataBlock.GetParamValue(i, Vec4U8.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec2S32: stream.WriteASCII(dataBlock.GetParamValue(i, Vec2S.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec3S32: stream.WriteASCII(dataBlock.GetParamValue(i, Vec3S.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec4S32: stream.WriteASCII(dataBlock.GetParamValue(i, Vec4S.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec2U32: stream.WriteASCII(dataBlock.GetParamValue(i, Vec2U.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec3U32: stream.WriteASCII(dataBlock.GetParamValue(i, Vec3U.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec4U32: stream.WriteASCII(dataBlock.GetParamValue(i, Vec4U.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec2F32: stream.WriteASCII(dataBlock.GetParamValue(i, Vec2F.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec3F32: stream.WriteASCII(dataBlock.GetParamValue(i, Vec3F.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec4F32: stream.WriteASCII(dataBlock.GetParamValue(i, Vec4F.Zero).ToString()); break;
                        case DataBlockParamType.ParamVec2B: stream.WriteASCII(dataBlock.GetParamValue(i, Vec2B.False).ToStringPretty()); break;
                        case DataBlockParamType.ParamVec3B: stream.WriteASCII(dataBlock.GetParamValue(i, Vec3B.False).ToStringPretty()); break;
                        case DataBlockParamType.ParamVec4B: stream.WriteASCII(dataBlock.GetParamValue(i, Vec4B.False).ToStringPretty()); break;
                        case DataBlockParamType.ParamMat3F32: stream.WriteASCII(dataBlock.GetParamValue(i, Mat3F.Identity).ToStringPretty()); break;
                        case DataBlockParamType.ParamMat4F32: stream.WriteASCII(dataBlock.GetParamValue(i, Mat4F.Identity).ToStringPretty()); break;
                        case DataBlockParamType.ParamTMat: stream.WriteASCII(dataBlock.GetParamValue(i, TMat.Identity).ToStringPretty()); break;
                        default:
                            Debug.LogError($"Undefined data block param type: {param.Type}");
                            break;
                    }

                }

                if (compact && dataBlock.ParamsCount == 1 && dataBlock.DataBlocksCount == 0)
                    stream.WriteASCII(';');
                else
                {
                    if (i + 1 < dataBlock.ParamsCount)
                    {
                        string nextParamName = dataBlock.Shared.GetString(dataBlock.GetParam(i + 1).NameId);

                        if (nextParamName.Length >= 3 &&
                            nextParamName[0] == DataBlockSerializationUtils.CommentPrefix[0] &&
                            nextParamName[1] == DataBlockSerializationUtils.CommentPrefix[1] &&
                            (nextParamName[2] == DataBlockSerializationUtils.CommentEndlineSuffixC ||
                             nextParamName[2] == DataBlockSerializationUtils.CommentEndlineSuffixCpp))
                        {
                            stream.WriteASCII(' ');
                            skipNextIndent = true;
                            continue;
                        }
                    }

                    WriteEndOfLine();
                }
            }

            if (!compact && dataBlock.ParamsCount > 0 && dataBlock.DataBlocksCount > 0)
                WriteEndOfLine();

            // Data Blocks
            for (int i = 0; i < dataBlock.DataBlocksCount; ++i)
            {
                DataBlock childBlock = dataBlock.GetDataBlock(i);

                string name = dataBlock.Shared.GetString(childBlock.NameId);
                bool nameIsSimple = IsNameIsSimple(name);

                if (name.Length >= 3 &&
                    name[0] == DataBlockSerializationUtils.CommentPrefix[0] &&
                    name[1] == DataBlockSerializationUtils.CommentPrefix[1])
                {
                    if (!compact && level > 0)
                        WriteIndent(level * 2);

                    // C-style comment
                    if (name[2] == DataBlockSerializationUtils.CommentSuffixC)
                    {
                        stream.WriteASCII('/');
                        stream.WriteASCII('*');
                        stream.WriteASCII(childBlock.GetString(0));
                        stream.WriteASCII('*');
                        stream.WriteASCII('/');
                        WriteEndOfLine();
                    }
                    // CPP-style comment
                    else if (name[2] == DataBlockSerializationUtils.CommentSuffixCpp)
                    {
                        stream.WriteASCII('/');
                        stream.WriteASCII('/');
                        stream.WriteASCII(childBlock.GetString(0));
                        WriteEndOfLine();
                    }

                    continue;
                }

                if (!compact && level > 0)
                    WriteIndent(level * 2);

                if (nameIsSimple)
                    WriteSimpleString(stream, Encoding.ASCII.GetBytes(name));
                else
                    WriteComplexString(stream, Encoding.ASCII.GetBytes(name));

                if (childBlock.IsEmpty())
                {
                    stream.WriteASCII('{');
                    stream.WriteASCII('}');
                    WriteEndOfLine();
                    continue;
                }

                // Opening bracket
                WriteEndOfLine();
                if (!compact && level > 0)
                    WriteIndent(level * 2);
                stream.WriteASCII('{');

                if (!(compact && childBlock.ParamsCount == 1 && dataBlock.DataBlocksCount == 0))
                    WriteEndOfLine();

                if (!WriteDataBlockText(stream, childBlock, level + 1, compact))
                    return false;

                // Closing bracket
                if (!compact && level > 0)
                    WriteIndent(level * 2);
                stream.WriteASCII('}');

                WriteEndOfLine();

                if (i != dataBlock.DataBlocksCount - 1 && !compact)
                    WriteEndOfLine();
            }

            return true;
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
            DataBlockTextParser parser = new DataBlockTextParser(buffer);
            parser.Parse(dataBlock);

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

        public static string GetDataBlockParamTypeString(DataBlockParamType type)
        {
            switch (type)
            {
                case DataBlockParamType.ParamS8: return "S8";
                case DataBlockParamType.ParamS16: return "S16";
                case DataBlockParamType.ParamS32: return "S32";
                case DataBlockParamType.ParamS64: return "S64";
                case DataBlockParamType.ParamU8: return "U8";
                case DataBlockParamType.ParamU16: return "U16";
                case DataBlockParamType.ParamU32: return "U32";
                case DataBlockParamType.ParamU64: return "U64";
                case DataBlockParamType.ParamF32: return "F32";
                case DataBlockParamType.ParamF64: return "F64";
                case DataBlockParamType.ParamBool: return "Bool";
                case DataBlockParamType.ParamVec4S8: return "Vec4S8";
                case DataBlockParamType.ParamVec4U8: return "Vec4U8";
                case DataBlockParamType.ParamVec2S32: return "Vec2S";
                case DataBlockParamType.ParamVec3S32: return "Vec3S";
                case DataBlockParamType.ParamVec4S32: return "Vec4S";
                case DataBlockParamType.ParamVec2U32: return "Vec2U";
                case DataBlockParamType.ParamVec3U32: return "Vec3U";
                case DataBlockParamType.ParamVec4U32: return "Vec4U";
                case DataBlockParamType.ParamVec2F32: return "Vec2F";
                case DataBlockParamType.ParamVec3F32: return "Vec3F";
                case DataBlockParamType.ParamVec4F32: return "Vec4F";
                case DataBlockParamType.ParamVec2B: return "Vec2B";
                case DataBlockParamType.ParamVec3B: return "Vec3B";
                case DataBlockParamType.ParamVec4B: return "Vec4B";
                case DataBlockParamType.ParamMat3F32: return "Mat3F";
                case DataBlockParamType.ParamMat4F32: return "Mat4F";
                case DataBlockParamType.ParamTMat: return "TMat";
                case DataBlockParamType.ParamString: return "String";
            }
            Debug.LogError($"Not implemented DataBlockParamType: {type}");

            return "";
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

        bool ParseIdentifier(List<byte> name)
        {
            while (true)
            {
                if (!SkipWhite())
                    return false;

                if (IsEndOfBuffer())
                    break;

                byte ch = ReadCharNoRewind();
                if (DataBlockSerializationUtils.IsDataBlockIdentifierChar(ch))
                {
                    int identifierOffset = m_ReadStream.Offset;
                    m_ReadStream.Rewind(1);
                    while (!IsEndOfBuffer())
                    {
                        if (!DataBlockSerializationUtils.IsDataBlockIdentifierChar(ReadCharNoRewind()))
                            break;
                        m_ReadStream.Rewind(1);
                    }

                    int identifierLength = m_ReadStream.Offset - identifierOffset;

                    name.Resize(identifierLength + 1);
                    for (int i = 0; i < identifierLength; ++i)
                        name[i] = m_ReadStream.ByteBufferData[m_ReadStream.Offset + i];
                    name[identifierLength] = 0;
                    
                    return true;
                }
                else if (ch == '"' || ch == '\'')
                    return ParseValue(name);
                else
                    break;
            }
            
            return false;
        }

        bool ParseValue(List<byte> value)
        {
            value.Clear();

            bool multiLineString = false;

            byte quotCh = 0;
            if (ReadCharNoRewind() == '"' || ReadCharNoRewind() == '\'')
            {
                quotCh = ReadChar();

                byte[] twoCh = new byte[2];
                m_ReadStream.ReadNoRewind(twoCh, 2);

                // Triple quot (MultiLine value prefix like '''\n )
                if (twoCh[0] == quotCh && twoCh[1] == quotCh)
                {
                    multiLineString = true;
                    m_ReadStream.Rewind(2);

                    // Skip first \n (start) of multiline
                    int chIdx = m_ReadStream.Offset;
                    for (; chIdx < m_ReadStream.ByteBuffereSize; ++chIdx)
                    {
                        byte c = m_ReadStream.ByteBufferData[chIdx];

                        if (c == '\n')
                        {
                            m_ReadStream.SetOffset(chIdx + 1);
                            break;
                        }
                        else if (c == ' ' || c == '\r' || c == '\t')
                        {
                            break;
                        }
                    }
                }
            }

            int multiLineCommentOffset = -1;
            int rewindToOffset = -1;

            while (true)
            {
                if (IsEndOfBuffer())
                    break;

                m_ReadStream.ReadNoRewind(out byte ch);

                // MultiLine comment
                if (multiLineCommentOffset != -1)
                {
                    byte ch2 = ReadCharNoRewind(1);

                    // CRLF
                    if (ch == '\r')
                    {
                        if (ch2 == '\n')
                        {
                            m_ReadStream.Rewind(2);
                            IncCurrentLine();
                        }
                        rewindToOffset = multiLineCommentOffset;
                        break;
                    }
                    else if (ch == '\n')
                    {
                        m_ReadStream.Rewind(1);
                        IncCurrentLine();
                        rewindToOffset = multiLineCommentOffset;
                        break;
                    }
                    else if (ch == 0)
                    {
                        ProcessSyntaxError("Unclosed comment");
                        return false;
                    }
                    else if (ch == '*' && ch2 == '/')
                    {
                        m_ReadStream.Rewind(2);
                        ch = ReadCharNoRewind();
                        if (ch == '\r' || ch == '\n')
                        {
                            rewindToOffset = multiLineCommentOffset;
                        }
                        multiLineCommentOffset = -1;
                    }
                    else
                    {
                        m_ReadStream.Rewind(1);
                        continue;
                    }
                }

                // String value
                if (quotCh != 0)
                {
                    if (ch == quotCh && !multiLineString)
                    {
                        m_ReadStream.Rewind(1);
                        if (!SkipWhite())
                            return false;

                        if (ReadCharNoRewind() == ';')
                            m_ReadStream.Rewind(1);

                        break;
                    }
                    else if (ch  == quotCh && multiLineString && ReadCharNoRewind(1) == quotCh && ReadCharNoRewind(2) == quotCh)
                    {
                        // Crop last multiline \n (end)
                        if (value.Count > 1 && value[value.Count - 1] == '\n')
                            value.RemoveAt(value.Count - 1);

                        m_ReadStream.Rewind(3);

                        if (!SkipWhite())
                            return false;

                        if (ReadCharNoRewind() == ';')
                            m_ReadStream.Rewind(1);

                        break;
                    }
                    else if (((ch == '\r' || ch == '\n') && !multiLineString) || ch == 0)
                    {
                        ProcessSyntaxError("Unclosed string");
                        return false;
                    }
                    else if (ch == '~')
                    {
                        m_ReadStream.Rewind(1);

                        if (IsEndOfBuffer())
                        {
                            ProcessSyntaxError("Unclosed string");
                            return false;
                        }

                        ch = ReadCharNoRewind();
                        if (ch == 'r')
                            ch = (byte)'\r';
                        else if (ch == 'n')
                            ch = (byte)'\n';
                        else if (ch == 't')
                            ch = (byte)'\t';
                    }
                    else if (ch == '\r')
                    {
                        m_ReadStream.Rewind(1);
                        continue;
                    }
                }
                else
                {
                    if (ch == ';' || ch == '\r' || ch == '\n' || ch == 0 || ch == '}')
                    {
                        if (ch == ';')
                            m_ReadStream.Rewind(1);
                        break;
                    }
                    else if (ch == '/')
                    {
                        byte nextChar = ReadCharNoRewind(1);

                        if (nextChar == '/')
                            break;
                        else if (nextChar == '*')
                        {
                            multiLineCommentOffset = m_ReadStream.Offset - 1;
                            m_ReadStream.Rewind(2);
                            continue;
                        }
                    }
                }

                value.Add(ch);
                m_ReadStream.Rewind(1);
            }

            // MultiLine comment end
            if (multiLineCommentOffset != -1)
            {
                while (true)
                {
                    byte ch = ReadCharNoRewind();
                    if (ch == 0)
                    {
                        ProcessSyntaxError("Unclosed string");
                        return false;
                    }
                    else if (ch == '\r' && ReadCharNoRewind(1) == '\n')
                    {
                        m_ReadStream.Rewind(1);
                        IncCurrentLine();
                    }
                    else if (ch == '\n')
                    {
                        IncCurrentLine();
                    }
                    else if (ch == '*' && ReadCharNoRewind(1) == '/')
                    {
                        m_ReadStream.Rewind(2);
                        multiLineCommentOffset = -1;
                    }

                    m_ReadStream.Rewind(1);
                }
            }

            if (quotCh == 0)
            {
                int i = 0;
                for (i = value.Count - 1; i >= 0; --i)
                {
                    if (value[i] != ' ' && value[i] != '\t')
                        break;
                }
                ++i;
                if (i < value.Count)
                    value.RemoveRange(i, value.Count - 1);
            }

            value.Add(0);

            if (rewindToOffset != -1)
                m_ReadStream.SetOffset(rewindToOffset);

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
                    case DataBlockParamType.ParamVec4U8:
                    {
                        if (Vec4U8.ParseString(value, 0, value.Count, out Vec4U8 paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec4U8(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec2S32:
                    {
                        if (Vec2S.ParseString(value, 0, value.Count, out Vec2S paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec2S(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec3S32:
                    {
                        if (Vec3S.ParseString(value, 0, value.Count, out Vec3S paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec3S(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec4S32:
                    {
                        if (Vec4S.ParseString(value, 0, value.Count, out Vec4S paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec4S(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec2U32:
                    {
                        if (Vec2U.ParseString(value, 0, value.Count, out Vec2U paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec2U(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec3U32:
                    {
                        if (Vec3U.ParseString(value, 0, value.Count, out Vec3U paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec3U(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec4U32:
                    {
                        if (Vec4U.ParseString(value, 0, value.Count, out Vec4U paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec4U(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec2F32:
                    {
                        if (Vec2F.ParseString(value, 0, value.Count, out Vec2F paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec2F(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec3F32:
                    {
                        if (Vec3F.ParseString(value, 0, value.Count, out Vec3F paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec3F(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec4F32:
                    {
                        if (Vec4F.ParseString(value, 0, value.Count, out Vec4F paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec4F(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec2B:
                    {
                        if (Vec2B.ParseString(value, 0, value.Count, out Vec2B paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec2B(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec3B:
                    {
                        if (Vec3B.ParseString(value, 0, value.Count, out Vec3B paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec3B(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamVec4B:
                    {
                        if (Vec4B.ParseString(value, 0, value.Count, out Vec4B paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddVec4B(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamMat3F32:
                    {
                        if (Mat3F.ParseStringPretty(value, 0, value.Count, out Mat3F paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddMat3F(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamMat4F32:
                    {
                        if (Mat4F.ParseStringPretty(value, 0, value.Count, out Mat4F paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddMat4F(name, paramValue);
                        break;
                    }
                    case DataBlockParamType.ParamTMat:
                    {
                        if (TMat.ParseStringPretty(value, 0, value.Count, out TMat paramValue, (byte)',') < 0)
                        {
                            ProcessSyntaxError("Syntax error");
                            return false;
                        }
                        dataBlock.AddTMat(name, paramValue);
                        break;
                    }
                    default:
                        break;
                }
            }

            return true;
        }
    }
}
