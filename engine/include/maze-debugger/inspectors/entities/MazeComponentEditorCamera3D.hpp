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
#if (!defined(_MazeComponentEditorCamera3D_hpp_))
#define _MazeComponentEditorCamera3D_hpp_


//////////////////////////////////////////
#include "maze-debugger/MazeDebuggerHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-debugger/inspectors/entities/MazeComponentEditor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ComponentEditorCamera3D);


    //////////////////////////////////////////
    // Class ComponentEditorCamera3D
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API ComponentEditorCamera3D
        : public GenericComponentEditor<Camera3D>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ComponentEditorCamera3D, ComponentEditor);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ComponentEditorCamera3D);

    public:

        //////////////////////////////////////////
        virtual ~ComponentEditorCamera3D();

        //////////////////////////////////////////
        static ComponentEditorCamera3DPtr Create();


    protected:

        //////////////////////////////////////////
        ComponentEditorCamera3D();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void buildUI() MAZE_OVERRIDE;

    protected:
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeComponentEditorCamera3D_hpp_
//////////////////////////////////////////
