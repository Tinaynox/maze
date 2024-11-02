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
#include "MazeParticlesEditorToolsHeader.hpp"
#include "maze-plugin-particles-editor-tools/inspectors/entities/MazeComponentEditorParticleSystem3D.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazeParticleSystemParameterF32.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ComponentEditorParticleSystem3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ComponentEditorParticleSystem3D);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ComponentEditorParticleSystem3D);

    //////////////////////////////////////////
    ComponentEditorParticleSystem3D::ComponentEditorParticleSystem3D()
    {
        
    }

    //////////////////////////////////////////
    ComponentEditorParticleSystem3D::~ComponentEditorParticleSystem3D()
    {
    }

    //////////////////////////////////////////
    ComponentEditorParticleSystem3DPtr ComponentEditorParticleSystem3D::Create(
        ComponentId _componentId,
        MetaClass* _componentMetaClass)
    {
        ComponentEditorParticleSystem3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ComponentEditorParticleSystem3D, object, init(_componentId, _componentMetaClass));
        return object;
    }

    //////////////////////////////////////////
    bool ComponentEditorParticleSystem3D::init(
        ComponentId _componentId,
        MetaClass* _componentMetaClass)
    {
        if (!ComponentEditor::init(_componentId, _componentMetaClass))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ComponentEditorParticleSystem3D::buildUI()
    {
        Vector<MetaClass*> const& componentSuperMetaClasses = getComponentMetaClass()->getAllSuperMetaClasses();
        for (MetaClass* metaClass : componentSuperMetaClasses)
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                MetaProperty* metaProperty = metaClass->getProperty(i);
                buildPropertyDrawer(metaProperty);
            }
        }

        m_expandButtonSprite->getEntityRaw()->setActiveSelf(!m_propertyDrawers.empty());
    }

} // namespace Maze
//////////////////////////////////////////
