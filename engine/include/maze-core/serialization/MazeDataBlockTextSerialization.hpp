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
#pragma once
#if (!defined(_MazeDataBlockTextSerialization_hpp_))
#define _MazeDataBlockTextSerialization_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/data/MazeByteBufferReadStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace DataBlockTextSerialization
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool SaveText(DataBlock const& _dataBlock, ByteBuffer& _buffer, U32 _flags = 0);

        //////////////////////////////////////////
        MAZE_CORE_API bool LoadText(DataBlock& _dataBlock, ByteBuffer const& _buffer);

        //////////////////////////////////////////
        MAZE_CORE_API bool SaveTextFile(DataBlock const& _dataBlock, Path const& _path, U32 _flags = 0);

        //////////////////////////////////////////
        MAZE_CORE_API bool LoadTextFile(DataBlock& _dataBlock, Path const& _path);

    } // namespace DataBlockTextSerialization
    //////////////////////////////////////////


    //////////////////////////////////////////
    // Class DataBlockTextParser
    //
    //////////////////////////////////////////
    class MAZE_CORE_API DataBlockTextParser
    {
    public:

        //////////////////////////////////////////
        enum class Statement
        {
            None,
            Param,
            Block
        };

        //////////////////////////////////////////
        struct PendingComment
        {
            //////////////////////////////////////////
            PendingComment(
                Size _startOffset = 0,
                Size _endOffset = 0,
                bool _cppStyle = false)
                : startOffset(_startOffset)
                , endOffset(_endOffset)
                , cppStyle(_cppStyle)
            {}

            Size startOffset = 0;
            Size endOffset = 0;
            bool cppStyle = false;
        };

    public: 

        //////////////////////////////////////////
        DataBlockTextParser(ByteBuffer const& _buffer);

        //////////////////////////////////////////
        ~DataBlockTextParser();

        //////////////////////////////////////////
        bool parse(DataBlock& _dataBlock);

    protected:

        //////////////////////////////////////////
        bool parseDataBlock(DataBlock& _dataBlock, Bool _isTopmost);

        //////////////////////////////////////////
        bool skipWhite(
            Bool _allowCRLF = true,
            Bool _trackNewLineAfterParam = false);

        //////////////////////////////////////////
        bool parseIdentifier(String& _name);

        //////////////////////////////////////////
        bool parseValue(String& _value);

        //////////////////////////////////////////
        inline bool isEndOfBuffer() const { return m_readStream.isEndOfBuffer(); }

        //////////////////////////////////////////
        void incCurrentLine();

        //////////////////////////////////////////
        void processSyntaxError(CString _message);

        //////////////////////////////////////////
        Char readCharNoRewind(Size _index = 0);

        //////////////////////////////////////////
        Char readChar();

        //////////////////////////////////////////
        void flushPendingComments(DataBlock& _dataBlock, Bool _toParams);

    private:
        DataBlock* m_dataBlock;
        ByteBufferReadStream m_readStream;
        S32 m_currentLine = 1;
        Size m_currentLineOffset = 0;

        Bool m_wasNewLineAfterStatement = false;
        Statement m_lastStatement = Statement::None;

        
        FastVector<PendingComment> m_pendingComments;
    };
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDataBlockTextSerialization_hpp_
//////////////////////////////////////////
