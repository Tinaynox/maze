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
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DShapeModule.hpp"
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
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazeParticleSystemParameterF32.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DSizeOverLifetimeModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DColorOverLifetimeModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DRotationOverLifetimeModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DVelocityLimitOverLifetimeModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DVelocityOverLifetimeModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DZoneData.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MetaPropertyDrawerParticleSystem3DShapeModule
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MetaPropertyDrawerParticleSystem3DShapeModule, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerParticleSystem3DShapeModule);

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DShapeModule::MetaPropertyDrawerParticleSystem3DShapeModule()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DShapeModule::~MetaPropertyDrawerParticleSystem3DShapeModule()
    {
        if (m_enabledDrawer)
        {
            m_enabledDrawer->eventUIData.unsubscribe(this);
            m_enabledDrawer.reset();
        }
    }

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DShapeModulePtr MetaPropertyDrawerParticleSystem3DShapeModule::Create(MetaProperty* _metaProperty)
    {
        MetaPropertyDrawerParticleSystem3DShapeModulePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerParticleSystem3DShapeModule, object, init(_metaProperty));
        return object;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerParticleSystem3DShapeModule::init(MetaProperty* _metaProperty)
    {
        if (!MetaPropertyDrawer::init(_metaProperty))
            return false;

        DataBlock data;
        data.setCString(MAZE_HCS("label"), "Emitter Shape");
        m_enabledDrawer = PropertyDrawerBool::Create(data);
        m_enabledDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerParticleSystem3DShapeModule::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DShapeModule::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        SpriteRenderer2DPtr sprite = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            Vec2F(_parent->getWidth(), 18),
            Vec2F::c_zero,
            renderSystem->getMaterialManager()->getSpriteMaterial(),
            _parent,
            _parent->getEntity()->getEcsScene());
        sprite->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Flags::Height, false);
        VerticalLayout2DPtr spriteLayout = sprite->getEntityRaw()->ensureComponent<VerticalLayout2D>();
        spriteLayout->setAutoWidth(false);
        spriteLayout->setAutoHeight(true);
        spriteLayout->setExpand(false);
        spriteLayout->setPadding(4.0f, 4.0f, 4.0f, 4.0f);
        sprite->setColor(180, 180, 180);

        VerticalLayout2DPtr verticalLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(sprite->getTransform()->getWidth() - 8, sprite->getTransform()->getHeight() - 8),
            Vec2F(4, 4),
            sprite->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        SizePolicy2DPtr sizePolicy = verticalLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        sizePolicy->setSizeDelta(-8, -8);
        sizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        verticalLayout->setAutoHeight(true);
        verticalLayout->setExpand(false);

        m_enabledDrawer->buildUI(verticalLayout->getTransform(), _label);

        VerticalLayout2DPtr verticalLayout2 = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(sprite->getTransform()->getWidth(), sprite->getTransform()->getHeight()),
            Vec2F(0, 0),
            verticalLayout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        sizePolicy = verticalLayout2->getEntityRaw()->ensureComponent<SizePolicy2D>();        
        sizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        verticalLayout2->setAutoHeight(true);
        verticalLayout2->setExpand(false);
        m_propertyDrawersLayoutEntity = verticalLayout2->getEntity();

        Vector<MetaClass*> const& componentSuperMetaClasses = ParticleSystem3DShapeModule::GetMetaClass()->getAllSuperMetaClasses();
        for (MetaClass* metaClass : componentSuperMetaClasses)
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                MetaProperty* metaProperty = metaClass->getProperty(i);
                
                //MetaPropertyDrawerPtr property = InspectorManager::GetInstancePtr()->createMetaPropertyDrawer(metaProperty);
                MetaPropertyDrawerPtr property = buildPropertyDrawer(metaProperty);

                if (property)
                {
                    if (metaProperty->getValueClassUID() == ClassInfo<ParticleSystem3DZoneData>::UID())
                    {
                        m_zoneMetaPropertyDrawer = std::static_pointer_cast<MetaPropertyDrawerParticleSystem3DZoneData>(property);
                    }
                    else
                    if (strcmp(metaProperty->getName(), "type") == 0)
                    {
                        m_typePropertyDrawer = std::static_pointer_cast<MetaPropertyDrawerEnumClass>(property);
                    }

                    property->buildUI(verticalLayout2->getTransform(), _label);
                    m_propertyDrawers.push_back(property);
                }
            }
        }
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DShapeModule::processDataToUI()
    {
        ParticleSystem3DShapeModule value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        m_processingDataToUI = true;
        {

            if (value.getType() != ParticleSystem3DZoneType::None)
            {
                Set<MetaInstance> mainModulesMetaInstances;

                for (MetaInstance metaInstance : m_metaInstances)
                {
                    ParticleSystem3D* obj = metaInstance.staticObjectCast<ParticleSystem3D>();

                    ParticleSystem3DShapeModule& value = obj->getShapeModule();

                    m_zoneMetaPropertyDrawer->setZoneType(value.getType());

                    mainModulesMetaInstances.insert(value.getMetaInstance());
                }

                for (MetaPropertyDrawerPtr property : m_propertyDrawers)
                {
                    property->linkMetaInstances(mainModulesMetaInstances);
                    property->processDataToUI();
                }
            }

            m_enabledDrawer->setValue(value.getType() != ParticleSystem3DZoneType::None);
            m_propertyDrawersLayoutEntity->setActiveSelf(value.getType() != ParticleSystem3DZoneType::None);
        }
        m_processingDataToUI = false;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DShapeModule::processDataFromUI()
    {
        if (m_processingDataToUI)
            return;
        
        for (MetaInstance const& metaInstance : m_metaInstances)
        {
            ParticleSystem3DShapeModule value;
            m_metaProperty->getValue(metaInstance, value);

            if (!m_enabledDrawer->getValue())
                value.setType(ParticleSystem3DZoneType::None);
            else
                value.setType(ParticleSystem3DZoneType::FromString(m_typePropertyDrawer->getDrawer()->getValue()));

            m_metaProperty->setValue(metaInstance, &value);
        }
    }


} // namespace Maze
//////////////////////////////////////////
