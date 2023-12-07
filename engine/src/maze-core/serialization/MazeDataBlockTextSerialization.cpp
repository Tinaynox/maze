//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeCoreHeader.hpp"
#include "maze-core/serialization/MazeDataBlockTextSerialization.hpp"
#include "maze-core/serialization/MazeDataBlockSerializationUtils.hpp"
#include "maze-core/data/MazeDataBlockShared.hpp"
#include "maze-core/data/MazeByteBufferWriteStream.hpp"
#include "maze-core/data/MazeByteBufferReadStream.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/helpers/MazeByteBufferHelper.hpp"
#include "maze-core/hash/MazeHashFNV1.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace DataBlockTextSerialization
    {
        //////////////////////////////////////////
        static inline bool IsDataBlockIdentifierChar(Char _char)
        {
            return (
                (_char >= 'a' && _char <= 'z') || 
                (_char >= 'A' && _char <= 'Z') ||
                (_char >= '0' && _char <= '9') || 
                _char == '_' || 
                _char == '-' ||
                _char == '.' ||
                _char == '~');
        }

        //////////////////////////////////////////
        static inline void WriteSimpleString(ByteBufferWriteStream& _stream, CString _value, Size _size)
        {
            if (_size == 0 || !*_value)
                return;

            _stream.write((U8 const*)_value, _size);
        };

        //////////////////////////////////////////
        static inline void WriteSimpleString(ByteBufferWriteStream& _stream, CString _value)
        {
            WriteSimpleString(_stream, _value, strlen(_value));
        }

        //////////////////////////////////////////
        static inline void WriteComplexString(ByteBufferWriteStream& _stream, CString _value, Size _size)
        {
            if (_size == 0)
                _value = "";

            // Quote type
            CString quote = "\"";
            Size quoteLen = 1;

            bool hasLineBreak = false;
            bool hasQuote = false;
            bool hasTick = false;
            for (CString p = _value; *p; ++p)
            {
                Char c = *p;

                if (c == '\n' || c == '\r')
                    hasLineBreak = true;
                else
                if (c == '\"')
                    hasQuote = true;
                else
                if (c == '\'')
                    hasTick = true;
                else
                    continue;

                if (hasLineBreak && hasQuote && hasTick)
                    break;
            }

            if (hasLineBreak && !hasQuote)
            {
                quote = "\"\"\"\n\"\"\"";
                quoteLen = 4;
            }
            else
            if (hasLineBreak && !hasTick)
            {
                quote = "'''\n'''";
                quoteLen = 4;
            }
            else
            if (hasLineBreak)
            {
                quote = "\"\"\"\n\"\"\"";
                quoteLen = 4;
            }
            else
            if (hasQuote && !hasTick)
            {
                quote = "'";
            }

            // Opening quote
            _stream.write((U8 const*)quote, quoteLen);

            for (CString p = _value; *p; ++p)
            {
                Char c = *p;

                if (c == '~')
                    _stream.write("~~", 2);
                else
                if (c == quote[0] && (quoteLen == 1 || p[1] == c))
                    _stream.write(c == '\"' ? "~\"" : "~\'", 2);
                else
                if (c == '\r' && quoteLen == 1)
                    _stream.write("~r", 2);
                else
                if (c == '\n' && quoteLen == 1)
                    _stream.write("~n", 2);
                else
                if (c == '\t')
                    _stream.write("~t", 2);
                else
                    _stream.write(&c, 1);
            }

            // Closing quote
            _stream.write((U8 const*)(quote + quoteLen - 1), quoteLen);
        };

        //////////////////////////////////////////
        static inline void WriteComplexString(ByteBufferWriteStream& _stream, CString _value)
        {
            WriteComplexString(_stream, _value, strlen(_value));
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool WriteDataBlockText(
            ByteBufferWriteStream& _stream,
            DataBlock const& _dataBlock,
            S32 _level,
            bool _compact)
        {
            auto writeEndOfLine = [&]() { _stream << "\n"; };
            auto writeIndent =
                [&](Size _size)
                {
                    static CString c_space8 = "        ";
                    for (; _size >= 8; _size -= 8)
                        _stream.write((U8 const*)c_space8, 8);
                    if (_size > 0)
                        _stream.write((U8 const*)c_space8, _size);
                };

            auto isNameIsSimple =
                [](String const& _name)
                {
                    bool result = true;
                    if (_name.empty())
                        result = false;

                    for (Size i = 0, e = _name.size(); i < e && result; ++i)
                        result = IsDataBlockIdentifierChar(_name[i]);

                    return result;
                };
            

            bool skipNextIndent = false;

            // Params
            for (DataBlock::ParamIndex i = 0; i < (DataBlock::ParamIndex)_dataBlock.getParamsCount(); ++i)
            {
                DataBlock::Param const& param = _dataBlock.getParam(i);

                String const& name = _dataBlock.getShared()->getString(param.nameId);
                bool nameIsSimple = isNameIsSimple(name);

                // Indent
                if (_level > 0)
                {
                    if (!skipNextIndent)
                        writeIndent(_level * 2);
                    else
                        skipNextIndent = false;
                }

                // TODO: Comments here


                if (nameIsSimple)
                    WriteSimpleString(_stream, name.c_str(), name.size());
                else
                    WriteComplexString(_stream, name.c_str(), name.size());
                
                _stream << ":";
                _stream << c_dataBlockParamTypeInfo[param.type].name;
                _stream << "=";
                if (param.type == U32(DataBlockParamType::ParamString))
                {
                    WriteComplexString(_stream, _dataBlock.getCString(i));
                }
                else
                {
                    U8 const* paramData = _dataBlock.getParamData(i);
                    Bool const* paramDataBool = (Bool const*)paramData;
                    S32 const* paramDataS32 = (S32 const*)paramData;
                    S64 const* paramDataS64 = (S64 const*)paramData;
                    U32 const* paramDataU32 = (U32 const*)paramData;
                    U64 const* paramDataU64 = (U64 const*)paramData;
                    F32 const* paramDataF32 = (F32 const*)paramData;
                    F64 const* paramDataF64 = (F64 const*)paramData;

                    auto paramBool = [](Bool _data) { return _data ? "true" : "false"; };

                    Char buff[256];
                    switch (DataBlockParamType(param.type))
                    {
                        case DataBlockParamType::ParamS32: snprintf(buff, sizeof(buff), "%d", paramDataS32[0]); break;
                        case DataBlockParamType::ParamS64: snprintf(buff, sizeof(buff), "%I64d", paramDataS64[0]); break;
                        case DataBlockParamType::ParamU32: snprintf(buff, sizeof(buff), "%u", paramDataU32[0]); break;
                        case DataBlockParamType::ParamU64: snprintf(buff, sizeof(buff), "%llu", paramDataU64[0]); break;
                        case DataBlockParamType::ParamF32: snprintf(buff, sizeof(buff), "%g", paramDataF32[0]); break;
                        case DataBlockParamType::ParamF64: snprintf(buff, sizeof(buff), "%lg", paramDataF64[0]); break;
                        case DataBlockParamType::ParamBool: snprintf(buff, sizeof(buff), paramBool(paramDataBool[0])); break;
                        case DataBlockParamType::ParamVec2DS: snprintf(buff, sizeof(buff), "%d, %d", paramDataS32[0], paramDataS32[1]); break;
                        case DataBlockParamType::ParamVec3DS: snprintf(buff, sizeof(buff), "%d, %d, %d", paramDataS32[0], paramDataS32[1], paramDataS32[2]); break;
                        case DataBlockParamType::ParamVec4DS: snprintf(buff, sizeof(buff), "%d, %d, %d, %d", paramDataS32[0], paramDataS32[1], paramDataS32[2], paramDataS32[3]); break;
                        case DataBlockParamType::ParamVec2DU: snprintf(buff, sizeof(buff), "%u, %u", paramDataU32[0], paramDataU32[1]); break;
                        case DataBlockParamType::ParamVec3DU: snprintf(buff, sizeof(buff), "%u, %u, %u", paramDataU32[0], paramDataU32[1], paramDataU32[2]); break;
                        case DataBlockParamType::ParamVec4DU: snprintf(buff, sizeof(buff), "%u, %u, %u, %u", paramDataU32[0], paramDataU32[1], paramDataU32[2], paramDataU32[3]); break;
                        case DataBlockParamType::ParamVec2DF: snprintf(buff, sizeof(buff), "%g, %g", paramDataF32[0], paramDataF32[1]); break;
                        case DataBlockParamType::ParamVec3DF: snprintf(buff, sizeof(buff), "%g, %g, %g", paramDataF32[0], paramDataF32[1], paramDataF32[2]); break;
                        case DataBlockParamType::ParamVec4DF: snprintf(buff, sizeof(buff), "%g, %g, %g, %g", paramDataF32[0], paramDataF32[1], paramDataF32[2], paramDataF32[3]); break;
                        case DataBlockParamType::ParamVec2DB: snprintf(buff, sizeof(buff), "%s, %s", paramBool(paramDataBool[0]), paramBool(paramDataBool[1])); break;
                        case DataBlockParamType::ParamVec3DB: snprintf(buff, sizeof(buff), "%s, %s, %s", paramBool(paramDataBool[0]), paramBool(paramDataBool[1]), paramBool(paramDataBool[2])); break;
                        case DataBlockParamType::ParamVec4DB: snprintf(buff, sizeof(buff), "%s, %s, %s, %s", paramBool(paramDataBool[0]), paramBool(paramDataBool[1]), paramBool(paramDataBool[2]), paramBool(paramDataBool[3])); break;
                        case DataBlockParamType::ParamMat3DF: 
                            snprintf(
                                buff,
                                sizeof(buff),
                                "[[%g, %g, %g] [%g, %g, %g] [%g, %g, %g]]",
                                paramDataF32[0], paramDataF32[1], paramDataF32[2],
                                paramDataF32[3], paramDataF32[4], paramDataF32[5],
                                paramDataF32[6], paramDataF32[7], paramDataF32[8]);
                            break;
                        case DataBlockParamType::ParamMat4DF:
                            snprintf(
                                buff,
                                sizeof(buff),
                                "[[%g, %g, %g, %g] [%g, %g, %g, %g] [%g, %g, %g, %g] [%g, %g, %g, %g]]",
                                paramDataF32[0], paramDataF32[1], paramDataF32[2], paramDataF32[3],
                                paramDataF32[4], paramDataF32[5], paramDataF32[6], paramDataF32[7],
                                paramDataF32[8], paramDataF32[9], paramDataF32[10], paramDataF32[11],
                                paramDataF32[12], paramDataF32[13], paramDataF32[14], paramDataF32[15]);
                            break;
                        default:
                            MAZE_WARNING("Undefined data block param type: %d", (S32)param.type);
                            break;
                    }

                    _stream << buff;
                }
                if (_compact && _dataBlock.getParamsCount() == 1 && _dataBlock.getDataBlocksCount() == 0)
                    _stream << ";";
                else
                {
                    // TODO: Side comments

                    writeEndOfLine();
                }
            }

            // Data Blocks
            bool paramsBlocksSeparated = false;
            for (DataBlock::DataBlockIndex i = 0, e = _dataBlock.getDataBlocksCount(); i < e; ++i)
            {
                DataBlock const* dataBlock = _dataBlock.getDataBlock(i);

                String const& name = _dataBlock.getShared()->getString(dataBlock->getNameId());
                bool nameIsSimple = isNameIsSimple(name);

                // TODO: Comments

                if (!_compact && !paramsBlocksSeparated)
                {
                    paramsBlocksSeparated = true;
                    writeEndOfLine();
                }

                if (!_compact && _level > 0)
                    writeIndent(_level * 2);

                if (nameIsSimple)
                    WriteSimpleString(_stream, name.c_str(), name.size());
                else
                    WriteComplexString(_stream, name.c_str(), name.size());

                if (dataBlock->isEmpty())
                {
                    _stream.write("{}", 2);
                    writeEndOfLine();
                    continue;
                }

                // Opening bracket
                _stream.write("\n", 1);
                if (!_compact && _level > 0)
                    writeIndent(_level * 2);
                _stream.write("{", 1);

                if (!(_compact && dataBlock->getParamsCount() == 1 && dataBlock->getDataBlocksCount() == 0))                
                    writeEndOfLine();

                if (!WriteDataBlockText(_stream, *dataBlock, _level + 1, _compact))
                    return false;

                // Closing bracket
                if (!_compact && _level > 0)
                    writeIndent(_level * 2);
                _stream.write("}", 1);

                writeEndOfLine();

                if (i != e - 1 && !_compact)
                    writeEndOfLine();
            }

            return true;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API bool SaveText(DataBlock const& _dataBlock, ByteBuffer& _buffer, U32 _flags)
        {
            _buffer.clear();

            StringKeyMap<DataBlock::SharedStringId> const& stringsMap = _dataBlock.getShared()->getStrings();

            ByteBufferWriteStream writeStream(_buffer);
            writeStream.reserve(stringsMap.size() * 64);


            WriteDataBlockText(writeStream, _dataBlock, 0, _flags & U32(DataBlockTextFlags::Compact));

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool LoadText(DataBlock& _dataBlock, ByteBuffer const& _buffer)
        {
            DataBlockTextParser parser(_buffer);
            parser.parse(_dataBlock);

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool SaveTextFile(DataBlock const& _dataBlock, Path const& _path, U32 _flags)
        {
            ByteBuffer byteBuffer;
            if (!SaveText(_dataBlock, byteBuffer, _flags))
                return false;

            return ByteBufferHelper::SaveBinaryFile(byteBuffer, _path);
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool LoadTextFile(DataBlock& _dataBlock, Path const& _path)
        {
            ByteBuffer byteBuffer;
            if (!ByteBufferHelper::LoadBinaryFile(byteBuffer, _path))
                return false;

            return LoadText(_dataBlock, byteBuffer);
        }


    } // namespace DataBlockTextSerialization
    //////////////////////////////////////////


    //////////////////////////////////////////
    MAZE_CORE_API static inline DataBlock::ParamIndex AddParam(
        DataBlock& _dataBlock,
        HashedCString _name,
        DataBlockParamType _type,
        CString _value,
        Size _size)
    {
        DataBlock::ParamIndex at = -1;

        DataBlock::ParamIndex itemId = _dataBlock.findParamIndex(_dataBlock.getShared()->getStringId(_name));
        if ((itemId >= 0) && (_dataBlock.getParamType(itemId) != _type))
        {
            return -1;
        }

        if (_type == DataBlockParamType::ParamString)
        {
            at = _dataBlock.addString(_name, String(_value, _value + _size));
        }
        else
        {
            switch (_type)
            {
                case DataBlockParamType::ParamS32:
                {
                    S32 value = StringHelper::StringToS32(_value);
                    at = _dataBlock.addS32(_name, value);
                    break;
                }
                case DataBlockParamType::ParamS64:
                {
                    S64 value = StringHelper::StringToS64(_value);
                    at = _dataBlock.addS64(_name, value);
                    break;
                }
                case DataBlockParamType::ParamU32:
                {
                    U32 value = StringHelper::StringToU32(_value);
                    at = _dataBlock.addU32(_name, value);
                    break;
                }
                case DataBlockParamType::ParamU64:
                {
                    U64 value = StringHelper::StringToU64(_value);
                    at = _dataBlock.addU64(_name, value);
                    break;
                }
                case DataBlockParamType::ParamF32:
                {
                    F32 value = StringHelper::StringToF32Safe(_value);
                    at = _dataBlock.addF32(_name, value);
                    break;
                }
                case DataBlockParamType::ParamF64:
                {
                    F64 value = StringHelper::StringToF64Safe(_value);
                    at = _dataBlock.addF64(_name, value);
                    break;
                }
                case DataBlockParamType::ParamBool:
                {
                    Bool value = false;
                    MAZE_TODO;
                    at = _dataBlock.addBool(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec2DS:
                {
                    Vec2DS value = Vec2DS::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addVec2DS(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec3DS:
                {
                    Vec3DS value = Vec3DS::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addVec3DS(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec4DS:
                {
                    Vec4DS value = Vec4DS::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addVec4DS(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec2DU:
                {
                    Vec2DU value = Vec2DU::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addVec2DU(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec3DU:
                {
                    Vec3DU value = Vec3DU::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addVec3DU(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec4DU:
                {
                    Vec4DU value = Vec4DU::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addVec4DU(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec2DF:
                {
                    Vec2DF value = Vec2DF::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addVec2DF(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec3DF:
                {
                    Vec3DF value = Vec3DF::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addVec3DF(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec4DF:
                {
                    Vec4DF value = Vec4DF::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addVec4DF(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec2DB:
                {
                    Vec2DB value = Vec2DB(false);
                    MAZE_TODO;
                    at = _dataBlock.addVec2DB(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec3DB:
                {
                    Vec3DB value = Vec3DB(false);
                    MAZE_TODO;
                    at = _dataBlock.addVec3DB(_name, value);
                    break;
                }
                case DataBlockParamType::ParamVec4DB:
                {
                    Vec4DB value = Vec4DB(false);
                    MAZE_TODO;
                    at = _dataBlock.addVec4DB(_name, value);
                    break;
                }
                case DataBlockParamType::ParamMat3DF:
                {
                    Mat3DF value = Mat3DF::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addMat3DF(_name, value);
                    break;
                }
                case DataBlockParamType::ParamMat4DF:
                {
                    Mat4DF value = Mat4DF::c_zero;
                    MAZE_TODO;
                    at = _dataBlock.addMat4DF(_name, value);
                    break;
                }
                default:
                    break;
            }
        }

        return at;
    }


    //////////////////////////////////////////
    // Class DataBlockTextParser
    //
    //////////////////////////////////////////
    DataBlockTextParser::DataBlockTextParser(ByteBuffer const& _buffer)
        : m_readStream(_buffer)
    {

    }

    //////////////////////////////////////////
    DataBlockTextParser::~DataBlockTextParser()
    {

    }

    //////////////////////////////////////////
    bool DataBlockTextParser::parse(DataBlock& _dataBlock)
    {
        m_dataBlock = &_dataBlock;
        m_dataBlock->clear();
        return parseDataBlock(*m_dataBlock, true);
    }

    //////////////////////////////////////////
    bool DataBlockTextParser::parseDataBlock(DataBlock& _dataBlock, Bool _isTopmost)
    {
        String nameText;
        String typeNameText;
        String valueText;

        for (;;)
        {
            if (!skipWhite(true))
                return false;

            if (isEndOfBuffer())
            {
                if (!_isTopmost)
                {
                    processSyntaxError("Unexpected EOF");
                    return false;
                }

                break;
            }

            if (readCharNoRewind() == '}')
            {
                if (_isTopmost)
                {
                    processSyntaxError("Unexpected '}' in topmost block");
                    return false;
                }
                m_readStream.rewind(1);

                flushPendingComments(_dataBlock, false);
                break;
            }

            Size startOffset = m_readStream.getOffset();
            nameText.clear();
            if (!parseIdentifier(nameText))
            {
                processSyntaxError("Expected identifier");
                return false;
            }

            if (!skipWhite())
                return false;

            if (isEndOfBuffer())
            {
                processSyntaxError("Unexpected EOF");
                return false;
            }

            if (readCharNoRewind() == '{')
            {
                m_readStream.rewind(1);
                DataBlock* newBlock = nullptr;

                m_wasNewLineAfterStatement = false;
                m_lastStatement = Statement::None;
                flushPendingComments(_dataBlock, false);

                newBlock = _dataBlock.addNewDataBlock(HashedString(nameText));

                if (newBlock)
                {
                    if (!parseDataBlock(*newBlock, false))
                        return false;
                }
                
                m_lastStatement = Statement::None;
            }
            else
            if (readCharNoRewind() == ':')
            {
                flushPendingComments(_dataBlock, true);

                m_readStream.rewind(1);
                typeNameText.clear();
                if (!parseIdentifier(typeNameText))
                {
                    processSyntaxError("Expected type identifier");
                    return false;
                }

                DataBlockParamType paramType = GetDataBlockParamType(MAZE_HASHED_CSTRING(typeNameText.c_str()));
                if (paramType == DataBlockParamType::None)
                {
                    processSyntaxError("Unknown type");
                    return false;
                }

                if (!skipWhite())
                    return false;

                if (isEndOfBuffer())
                {
                    processSyntaxError("Unexpected EOF");
                    return false;
                }

                if (readChar() != '=')
                {
                    processSyntaxError("Expected '-'");
                    return false;
                }

                if (!skipWhite())
                    return false;

                if (strchr("\r\n", readCharNoRewind()))
                {
                    processSyntaxError("unexpected CR/LF");
                    return false;
                }

                if (isEndOfBuffer())
                {
                    processSyntaxError("Unexpected EOF");
                    return false;
                }

                valueText.clear();
                if (!parseValue(valueText))
                    return false;

                if (AddParam(_dataBlock, HashedString(nameText), paramType, valueText.c_str(), valueText.size()) < 0)
                {
                    processSyntaxError("Invalid value");
                    return false;
                }

                m_wasNewLineAfterStatement = false;
                m_lastStatement = Statement::Param;
            }
            else
            if (readCharNoRewind() == '=')
            {
                MAZE_TODO;
            }
            else
            {
                processSyntaxError("Syntax error!");
                return false;
            }
        }

        return true;
    }

    //////////////////////////////////////////
    bool DataBlockTextParser::skipWhite(
        Bool _allowCRLF,
        Bool _trackNewLineAfterParam)
    {
        for (;;)
        {
            if (isEndOfBuffer())
                break;

            Char ch = 0;
            m_readStream >> ch;
            if (ch == ' ' || ch == '\t' || ch == '\x1A')
                continue;

            if (ch == 0)
            {
                continue;
            }

            if (_allowCRLF)
            {
                // CR
                if (ch == '\r')
                {
                    Char nextCh = 0;
                    m_readStream.readNoRewind(nextCh);

                    if (!isEndOfBuffer() && nextCh == '\n')
                    {
                        m_readStream.rewind(1);
                        incCurrentLine();
                        if (_trackNewLineAfterParam)
                            m_wasNewLineAfterStatement = true;
                    }
                    continue;
                }
                else
                // LF
                if (ch == '\n')
                {
                    incCurrentLine();
                    if (_trackNewLineAfterParam)
                        m_wasNewLineAfterStatement = true;
                    continue;
                }
            }

            if (ch == '/')
            {
                if (!isEndOfBuffer())
                {
                    Char nextCh = 0;
                    m_readStream >> nextCh;
                    if (nextCh == '/')
                    {
                        Size cppCommentStartOffset = m_readStream.getOffset();

                        while (!isEndOfBuffer())
                        {
                            Char nextNextCh = 0;
                            m_readStream >> nextNextCh;
                            if (nextNextCh == '\r' || nextNextCh == '\n')
                                break;
                        }

                        if (m_wasNewLineAfterStatement || m_lastStatement == Statement::None)
                            m_pendingComments.emplace_back(cppCommentStartOffset, m_readStream.getOffset() - 1, true);
                        else
                        if (m_lastStatement == Statement::Param)
                        {
                            m_dataBlock->addString(
                                MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_CPP),
                                String(m_readStream.getData() + cppCommentStartOffset, m_readStream.getData() + m_readStream.getOffset() - 1));
                        }
                        else
                        {
                            m_dataBlock->addNewDataBlock(
                                MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_CPP))->addString(
                                    MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_CPP),
                                    String(m_readStream.getData() + cppCommentStartOffset, m_readStream.getData() + m_readStream.getOffset() - 1));
                        }

                        continue;
                    }
                    else
                    if (nextCh == '*')
                    {
                        Size cCommentStartOffset = m_readStream.getOffset();
                        S32 count = 1;
                        while (m_readStream.canRead(2))
                        {
                            Char nextNextChars[2];
                            m_readStream >> nextNextChars;

                            if (nextNextChars[0] == '/' && nextNextChars[1] == '*')
                            {
                                m_readStream.rewind(2);
                                ++count;
                            }
                            else
                            if (nextNextChars[0] == '*' && nextNextChars[1] == '/')
                            {
                                m_readStream.rewind(2);
                                if (--count <= 0)
                                    break;
                            }
                            else
                            {
                                m_readStream.rewind(1);
                            }
                        }

                        if (count > 0 && !m_readStream.canRead(2))
                        {
                            processSyntaxError("Unexpected EOF inside comment");
                            return false;
                        }

                        if (m_wasNewLineAfterStatement || m_lastStatement == Statement::None)
                            m_pendingComments.emplace_back(cCommentStartOffset, m_readStream.getOffset() - 2, false);
                        else
                        if (m_lastStatement == Statement::Param)
                        {
                            m_dataBlock->addString(
                                MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_C),
                                String(m_readStream.getData() + cCommentStartOffset, m_readStream.getData() + m_readStream.getOffset() - 2));
                        }
                        else
                        {
                            m_dataBlock->addNewDataBlock(
                                MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_C))->addString(
                                    MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_C),
                                    String(m_readStream.getData() + cCommentStartOffset, m_readStream.getData() + m_readStream.getOffset() - 2));
                        }

                        continue;
                    }
                    else
                    {
                        m_readStream.rewind(-1);
                    }
                }

                m_readStream.rewind(-1);
                break;
            }
            else
            {
                m_readStream.rewind(-1);
                break;
            }
        }

        return true;
    }

    //////////////////////////////////////////
    bool DataBlockTextParser::parseIdentifier(String& _name)
    {
        for (;;)
        {
            if (!skipWhite())
                return false;

            if (isEndOfBuffer())
                break;

            Char ch = readCharNoRewind();
            if (IsDataBlockIdentifierChar(ch))
            {
                Size identifierOffset = m_readStream.getOffset();
                m_readStream.rewind(1);
                while (!isEndOfBuffer())
                {
                    if (!IsDataBlockIdentifierChar(readCharNoRewind()))
                        break;
                    m_readStream.rewind(1);
                }

                Size identifierLength = m_readStream.getOffset() - identifierOffset;

                _name.resize(identifierLength + 1);
                memcpy(&_name[0], m_readStream.getData() + identifierOffset, identifierLength);
                _name[identifierLength] = 0;

                return true;
            }
            else
            if (ch == '"' || ch == '\'')
                return parseValue(_name);
            else
                break;
        }
        return false;
    }

    //////////////////////////////////////////
    bool DataBlockTextParser::parseValue(String& _value)
    {
        _value.clear();

        Bool multiLineString = false;

        Char quotCh = 0;
        if (readCharNoRewind() == '"' || readCharNoRewind() == '\'')
        {
            m_readStream.read(quotCh);

            Char twoCh[2];
            m_readStream.readNoRewind((U8*)twoCh, 2);

            // Triple quot (MultiLine value prefix like '''\n )
            if (twoCh[0] == quotCh && twoCh[1] == quotCh)
            {
                multiLineString = true;
                m_readStream.rewind(2);

                // Skip first \n (start) of multiline
                CString ch = (CString)(m_readStream.getData() + m_readStream.getOffset());
                for (CString p = ch; *p; ++p)
                {
                    if (*p == '\n')
                    {
                        m_readStream.setOffset((U8*)p - m_readStream.getData() + 1);
                        break;
                    }
                    else
                    if (!strchr(" \r\t", *p))
                        break;
                }

            }
        }

        Size multiLineCommentOffset = Size(-1);
        Size rewindToOffset = Size(-1);

        for (;;)
        {
            if (isEndOfBuffer())
            {
                processSyntaxError("Unexpected EOF");
                return false;
            }

            Char ch = 0;
            m_readStream.readNoRewind(ch);

            // MultiLine comment
            if (multiLineCommentOffset != Size(-1))
            {
                Char ch2 = 0;
                m_readStream.readNoRewind(ch2);

                // CRLF
                if (ch == '\r')
                {
                    if (ch2 == '\n')
                    {
                        m_readStream.rewind(2);
                        incCurrentLine();
                    }
                    rewindToOffset = multiLineCommentOffset;
                    break;
                }
                else
                if (ch == '\n')
                {
                    m_readStream.rewind(1);
                    incCurrentLine();
                    rewindToOffset = multiLineCommentOffset;
                    break;
                }
                else
                if (ch == 0)
                {
                    processSyntaxError("Unclosed comment");
                    return false;
                }
                else
                if ((ch == '*') && (ch2 == '/'))
                {
                    m_readStream.rewind(2);
                    m_readStream.readNoRewind(ch);
                    if (ch == '\r' || ch == '\n')
                    {
                        rewindToOffset = multiLineCommentOffset;
                    }
                    multiLineCommentOffset = Size(-1);
                }
                else
                {
                    m_readStream.rewind(1);
                    continue;
                }
            }

            // String value
            if (quotCh)
            {
                if (ch == quotCh && !multiLineString)
                {
                    m_readStream.rewind(1);
                    if (!skipWhite())
                        return false;

                    if (readCharNoRewind() == ';')
                        m_readStream.rewind(1);
                    break;
                }
                else
                if (ch == quotCh && multiLineString && readCharNoRewind(1) == quotCh && readCharNoRewind(2) == quotCh)
                {
                    // Crop last multiline \n (end)
                    if (_value.size() > 1 && _value[_value.size() - 1] == '\n')
                        _value.erase(_value.size() - 1, 1);
                    m_readStream.rewind(3);

                    if (!skipWhite())
                        return false;
                    if (readCharNoRewind() == ';')
                        m_readStream.rewind(1);
                    break;
                }
                else
                if (((ch == '\r' || ch == '\n') && !multiLineString) || ch == 0)
                {
                    processSyntaxError("Unclosed string");
                    return false;
                }
                else
                if (ch == '~')
                {
                    m_readStream.rewind(1);

                    if (isEndOfBuffer())
                    {
                        processSyntaxError("Unclosed string");
                        return false;
                    }

                    ch = readCharNoRewind();
                    if (ch == 'r')
                        ch = '\r';
                    else
                    if (ch == 'n')
                        ch = '\n';
                    else
                    if (ch == 't')
                        ch = '\t';
                }
                else
                if (ch == '\r')
                {
                    m_readStream.rewind(1);
                    continue;
                }
            }
            else
            {
                if (ch == ';' || ch == '\r' || ch == '\n' || ch == 0 || ch == '}')
                {
                    if (ch == ';')
                        m_readStream.rewind(1);
                    break;
                }
                else
                if (ch == '/')
                {
                    Char nextChar = readCharNoRewind(1);

                    if (nextChar == '/')
                        break;
                    else
                    if (nextChar == '*')
                    {
                        multiLineCommentOffset = m_readStream.getOffset() - 1;
                        m_readStream.rewind(2);
                        continue;
                    }
                }
            }

            _value.push_back(ch);
            m_readStream.rewind(1);
        }

        // MultiLine comment end
        if (multiLineCommentOffset != Size(-1))
        {
            for (;;)
            {
                Char ch = readCharNoRewind();
                if (ch == 0)
                {
                    processSyntaxError("Unclosed string");
                    return false;
                }
                else
                if ((ch == '\r') && (readCharNoRewind(1) == '\n'))
                {
                    m_readStream.rewind(1);
                    incCurrentLine();
                }
                else
                if (ch == '\n')
                {
                    incCurrentLine();
                }
                else
                if ((ch == '*') && (readCharNoRewind(1) == '/'))
                {
                    m_readStream.rewind(2);
                    multiLineCommentOffset = Size(-1);
                }

                m_readStream.rewind(1);
            }
        }

        if (quotCh == 0)
        {
            S32 i = 0;
            for (i = (S32)_value.size() - 1; i >= 0; --i)
            {
                if (_value[i] != ' ' && _value[i] != '\t')
                    break;
            }
            ++i;
            if (i < (S32)_value.size())
                _value.erase(i, _value.size() - 1);
        }

        _value.push_back(0);

        if (rewindToOffset != Size(-1))
            m_readStream.setOffset(rewindToOffset);
        return true;
    }

    //////////////////////////////////////////
    void DataBlockTextParser::incCurrentLine()
    {
        ++m_currentLine;
        m_currentLineOffset = m_readStream.getOffset();
    }

    //////////////////////////////////////////
    void DataBlockTextParser::processSyntaxError(CString _message)
    {
        MAZE_ERROR("%s", _message);
    }

    //////////////////////////////////////////
    Char DataBlockTextParser::readCharNoRewind(Size _index)
    {
        MAZE_ASSERT(_index < 3 && "Index is out of bounds");
        Char ch[3] = {0, 0, 0};
        m_readStream.readNoRewind((U8*)ch, 3);
        return ch[_index];
    }

    //////////////////////////////////////////
    Char DataBlockTextParser::readChar()
    {
        Char ch = 0;
        m_readStream >> ch;
        return ch;
    }

    //////////////////////////////////////////
    void DataBlockTextParser::flushPendingComments(DataBlock& _dataBlock, Bool _toParams)
    {
        for (auto& pendingComment : m_pendingComments)
        {
            HashedCString commentKey = pendingComment.cppStyle ? MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_SUFFIX_CPP)
                                                               : MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_SUFFIX_C);
            if (_toParams)
                _dataBlock.addString(
                    commentKey,
                    String(m_readStream.getData() + pendingComment.startOffset, m_readStream.getData() + pendingComment.endOffset));
            else
                _dataBlock.addNewDataBlock(commentKey)->addString(
                    commentKey,
                    String(m_readStream.getData() + pendingComment.startOffset, m_readStream.getData() + pendingComment.endOffset));
        }
        m_pendingComments.clear();
    }


} // namespace Maze
//////////////////////////////////////////
