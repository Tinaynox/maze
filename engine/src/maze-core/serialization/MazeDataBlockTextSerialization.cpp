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
        static inline bool IsDataBlockStringSimpleChar(Char _char)
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

            //_stream.write((U8 const*)_value, _size);
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
                        result = IsDataBlockStringSimpleChar(_name[i]);

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
            _dataBlock.clear();

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


} // namespace Maze
//////////////////////////////////////////
