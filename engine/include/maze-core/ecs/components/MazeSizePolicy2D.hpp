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
#if (!defined(_MazeSizePolicy2D_hpp_))
#define _MazeSizePolicy2D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SizePolicy2D);
    MAZE_USING_SHARED_PTR(Transform2D);


    //////////////////////////////////////////
    // Class SizePolicy2D
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SizePolicy2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SizePolicy2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SizePolicy2D);
    
    public:

        //////////////////////////////////////////
        enum Flags
        {
            Width        = MAZE_BIT(0),
            Height       = MAZE_BIT(1),
        };

    public:

        //////////////////////////////////////////
        virtual ~SizePolicy2D();

        //////////////////////////////////////////
        static SizePolicy2DPtr Create();

        //////////////////////////////////////////
        void processInit(Transform2D* _transform);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        Vec2F const& getSizeDelta() const { return m_sizeDelta; }

        //////////////////////////////////////////
        void setSizeDelta(Vec2F const& _sizeDelta);

        //////////////////////////////////////////
        inline void setSizeDelta(F32 _x, F32 _y)
        {
            setSizeDelta(Vec2F(_x, _y));
        }


        //////////////////////////////////////////
        inline void setFlag(Flags _flag, bool _value)
        {
            if (_value)
                m_flags |= _flag;
            else
                m_flags &= ~_flag;
        }

        //////////////////////////////////////////
        inline bool getFlag(Flags _flag) const { return m_flags & _flag; }

        //////////////////////////////////////////
        inline S32 getFlags() const { return m_flags; }

        //////////////////////////////////////////
        inline void setFlags(S32 _flags) { m_flags = _flags; }


        //////////////////////////////////////////
        void updateSize();

    protected:

        //////////////////////////////////////////
        SizePolicy2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

    protected:
        Transform2DPtr m_transform;

        S32 m_flags;
        Vec2F m_sizeDelta;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSizePolicy2D_hpp_
//////////////////////////////////////////
