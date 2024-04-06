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
#include "maze-core/helpers/MazeByteBufferHelper.hpp"
#include "maze-core/data/MazeDataBlockShared.hpp"
#include "maze-core/data/MazeByteBufferWriteStream.hpp"
#include "maze-core/data/MazeByteBufferReadStream.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    namespace ByteBufferHelper
    {

        //////////////////////////////////////////
        MAZE_CORE_API bool SaveBinaryFile(ByteBuffer const& _byteBuffer, Path const& _path)
        {
            FILE* file = StdHelper::OpenFile(_path, "wb");
            if (!file)
                return false;

            fwrite(_byteBuffer.getDataRO(), _byteBuffer.getSize(), 1, file);
            fclose(file);

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool LoadBinaryFile(ByteBuffer& _byteBuffer, Path const& _path)
        {
            FILE* file = StdHelper::OpenFile(_path, "rb");
            if (!file)
                return false;

            U8 buff[BUFSIZ];
            Size readSize = fread(buff, 1, BUFSIZ, file);
            while (readSize)
            {
                Size prevSize = _byteBuffer.getSize();
                _byteBuffer.resize(_byteBuffer.getSize() + readSize);
                memcpy(_byteBuffer.getDataRW() + prevSize, buff, readSize);

                readSize = fread(buff, 1, BUFSIZ, file);
            }

            Size prevSize = _byteBuffer.getSize();
            _byteBuffer.resize(_byteBuffer.getSize() + readSize);
            memcpy(_byteBuffer.getDataRW() + prevSize, buff, readSize);

            fclose(file);

            return true;
        }


    } // namespace ByteBufferHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
