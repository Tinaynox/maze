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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditorCamera3D.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDRadians.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDSliderRadians.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDFOV.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ComponentEditorCamera3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ComponentEditorCamera3D);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ComponentEditorCamera3D);

    //////////////////////////////////////////
    ComponentEditorCamera3D::ComponentEditorCamera3D()
    {
        
    }

    //////////////////////////////////////////
    ComponentEditorCamera3D::~ComponentEditorCamera3D()
    {
    }

    //////////////////////////////////////////
    ComponentEditorCamera3DPtr ComponentEditorCamera3D::Create(ComponentId _componentId, MetaClass* _metaClass)
    {
        ComponentEditorCamera3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ComponentEditorCamera3D, object, init(_componentId, _metaClass));
        return object;
    }

    //////////////////////////////////////////
    bool ComponentEditorCamera3D::init(ComponentId _componentId, MetaClass* _metaClass)
    {
        if (!ComponentEditor::init(_componentId, _metaClass))
            return false;

        registerCustomPropertyDrawer<MetaPropertyDrawerFOV>(getComponentMetaClass()->getProperty("fov"));

        return true;
    }

    //////////////////////////////////////////
    void ComponentEditorCamera3D::buildUI()
    {
        ComponentEditor::buildUI();
    }

} // namespace Maze
//////////////////////////////////////////
