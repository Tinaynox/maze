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
#if (!defined(_MazeInstanceStreamColor_hpp_))
#define _MazeInstanceStreamColor_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStream.hpp"
#include "maze-graphics/MazeColorU32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(InstanceStreamColor);


    //////////////////////////////////////////
    // Class InstanceStreamColor
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API InstanceStreamColor
        : public InstanceStream
    {
    public:

        //////////////////////////////////////////
        virtual ~InstanceStreamColor();


        //////////////////////////////////////////
        inline void setOffset(S32 _value) { m_dataOffset = _value; }

        //////////////////////////////////////////
        inline S32 getOffset() const { return m_dataOffset; }

        //////////////////////////////////////////
        inline Size getDataSize() const { return m_data.size(); }

        //////////////////////////////////////////
        inline void setData(Size _index, Vec4F const& _value) { m_data[_index] = _value; }

        //////////////////////////////////////////
        inline void setData(
            Size _index,
            Vec4F const* _value,
            S32 _count)
        {
            memcpy(
                m_data.begin() + _index,
                _value,
                sizeof(Vec4F) * _count);
        }

    protected:

        //////////////////////////////////////////
        InstanceStreamColor();

        //////////////////////////////////////////
        bool init();

    protected:
        FastVector<Vec4F> m_data;
        S32 m_dataOffset = 0;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeInstanceStreamColor_hpp_
//////////////////////////////////////////
