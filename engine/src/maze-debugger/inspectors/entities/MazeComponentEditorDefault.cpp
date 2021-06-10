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
#include "MazeDebuggerHeader.hpp"
#include "maze-debugger/inspectors/entities/MazeComponentEditorDefault.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ComponentEditorDefault
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ComponentEditorDefault, ComponentEditor);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ComponentEditorDefault);

    //////////////////////////////////////////
    ComponentEditorDefault::ComponentEditorDefault()
        : m_metaClass(nullptr)
    {

    }

    //////////////////////////////////////////
    ComponentEditorDefault::~ComponentEditorDefault()
    {

    }

    //////////////////////////////////////////
    ComponentEditorDefaultPtr ComponentEditorDefault::Create(MetaClass* _metaClass)
    {
        ComponentEditorDefaultPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ComponentEditorDefault, object, init(_metaClass));
        return object;
    }

    //////////////////////////////////////////
    bool ComponentEditorDefault::init(MetaClass* _metaClass)
    {
        if (!ComponentEditor::init())
            return false;

        m_metaClass = _metaClass;

        return true;
    }

    //////////////////////////////////////////
    ClassUID ComponentEditorDefault::getComponentUID()
    {
        return m_metaClass->getClassUID();
    }

    //////////////////////////////////////////
    MetaClass* ComponentEditorDefault::getComponentMetaClass()
    {
        return m_metaClass;
    }

} // namespace Maze
//////////////////////////////////////////
