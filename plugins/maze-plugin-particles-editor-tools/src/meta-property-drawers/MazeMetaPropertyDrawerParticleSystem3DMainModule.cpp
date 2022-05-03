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
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DMainModule.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazePropertyDrawerParticleSystemParameterF32.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DEmissionModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DSizeOverLifetimeModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DColorOverLifetimeModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DVelocityLimitOverLifetimeModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MetaPropertyDrawerParticleSystem3DMainModule
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MetaPropertyDrawerParticleSystem3DMainModule, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerParticleSystem3DMainModule);

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DMainModule::MetaPropertyDrawerParticleSystem3DMainModule()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DMainModule::~MetaPropertyDrawerParticleSystem3DMainModule()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DMainModulePtr MetaPropertyDrawerParticleSystem3DMainModule::Create(MetaProperty* _metaProperty)
    {
        MetaPropertyDrawerParticleSystem3DMainModulePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerParticleSystem3DMainModule, object, init(_metaProperty));
        return object;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerParticleSystem3DMainModule::init(MetaProperty* _metaProperty)
    {
        if (!MetaPropertyDrawer::init(_metaProperty))
            return false;

        registerCustomPropertyDrawer<
            MetaPropertyDrawerDefault<
            ParticleSystemParameterF32,
            PropertyDrawerParticleSystemParameterF32Positive>>("size");

        registerCustomPropertyDrawer<
            MetaPropertyDrawerDefault<
            ParticleSystemParameterF32,
            PropertyDrawerParticleSystemParameterF32Positive>>("lifetime");

        registerCustomPropertyDrawer<
            MetaPropertyDrawerDefault<
            ParticleSystemParameterF32,
            PropertyDrawerParticleSystemParameterF32Degrees>>("rotation");

        registerCustomPropertyDrawer<MetaPropertyDrawerParticleSystem3DEmissionModule>("emission");
        registerCustomPropertyDrawer<MetaPropertyDrawerParticleSystem3DSizeOverLifetimeModule>("sizeOverLifetime");
        registerCustomPropertyDrawer<MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule>("velocityOverLifetime");
        registerCustomPropertyDrawer<MetaPropertyDrawerParticleSystem3DVelocityLimitOverLifetimeModule>("velocityLimitOverLifetime");
        registerCustomPropertyDrawer<MetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule>("rotationOverLifetime");
        registerCustomPropertyDrawer<MetaPropertyDrawerParticleSystem3DColorOverLifetimeModule>("colorOverLifetime");

        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DMainModule::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        Vector<MetaClass*> const& componentSuperMetaClasses = ParticleSystem3DMainModule::GetMetaClass()->getAllSuperMetaClasses();
        for (MetaClass* metaClass : componentSuperMetaClasses)
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                MetaProperty* metaProperty = metaClass->getProperty(i);
                
                MetaPropertyDrawerPtr property = buildPropertyDrawer(metaProperty);

                if (property)
                {
                    property->buildUI(_parent, _label);
                    m_propertyDrawers.push_back(property);
                }
            }
        }
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DMainModule::processDataToUI()
    {
        Set<MetaInstance> mainModulesMetaInstances;

        for (MetaInstance metaInstance : m_metaInstances)
        {
            ParticleSystem3D* obj = metaInstance.staticObjectCast<ParticleSystem3D>();

            ParticleSystem3DMainModule& value = obj->getMainModule();

            mainModulesMetaInstances.insert(value.getMetaInstance());
        }

        for (MetaPropertyDrawerPtr property : m_propertyDrawers)
        {
            property->linkMetaInstances(mainModulesMetaInstances);
            property->processDataToUI();
        }
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DMainModule::processDataFromUI()
    {
        
    }


} // namespace Maze
//////////////////////////////////////////
