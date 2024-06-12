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
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DZoneData.hpp"
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
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/property-drawers/MazeRadians.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeDefault.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazeParticleSystemParameterF32.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class MetaPropertyDrawerParticleSystem3DZoneData
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MetaPropertyDrawerParticleSystem3DZoneData, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerParticleSystem3DZoneData);

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DZoneData::MetaPropertyDrawerParticleSystem3DZoneData()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DZoneData::~MetaPropertyDrawerParticleSystem3DZoneData()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DZoneDataPtr MetaPropertyDrawerParticleSystem3DZoneData::Create(MetaProperty* _metaProperty)
    {
        MetaPropertyDrawerParticleSystem3DZoneDataPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerParticleSystem3DZoneData, object, init(_metaProperty));
        return object;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerParticleSystem3DZoneData::init(MetaProperty* _metaProperty)
    {
        if (!MetaPropertyDrawer::init(_metaProperty))
            return false;


        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DZoneData::buildUI(
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
        sprite->setColor(190, 190, 190);

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
        
        createZoneDrawer(
            ParticleSystem3DZoneType::Sphere,
            ParticleSystem3DZoneSphere::GetMetaClass(),
            verticalLayout->getTransform());

        createZoneDrawer(
            ParticleSystem3DZoneType::Hemisphere,
            ParticleSystem3DZoneHemisphere::GetMetaClass(),
            verticalLayout->getTransform());

        createZoneDrawer(
            ParticleSystem3DZoneType::Cone,
            ParticleSystem3DZoneCone::GetMetaClass(),
            verticalLayout->getTransform());

        createZoneDrawer(
            ParticleSystem3DZoneType::Torus,
            ParticleSystem3DZoneTorus::GetMetaClass(),
            verticalLayout->getTransform());

        createZoneDrawer(
            ParticleSystem3DZoneType::Box,
            ParticleSystem3DZoneBox::GetMetaClass(),
            verticalLayout->getTransform());

        createZoneDrawer(
            ParticleSystem3DZoneType::Circle,
            ParticleSystem3DZoneCircle::GetMetaClass(),
            verticalLayout->getTransform());

        updateZoneType();
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DZoneData::processDataToUI()
    {
        
        Set<MetaInstance> mainModulesMetaInstances;

        for (MetaInstance metaInstance : m_metaInstances)
        {
            ParticleSystem3DShapeModule* obj = metaInstance.staticObjectCast<ParticleSystem3DShapeModule>();

            ParticleSystem3DZoneData& value = obj->getZoneData();
            
            switch (m_zoneType)
            {
                case ParticleSystem3DZoneType::Sphere: 
                    mainModulesMetaInstances.insert(value.sphere.getMetaInstance());
                    break;
                case ParticleSystem3DZoneType::Hemisphere:
                    mainModulesMetaInstances.insert(value.hemisphere.getMetaInstance());
                    break;
                case ParticleSystem3DZoneType::Cone:
                    mainModulesMetaInstances.insert(value.cone.getMetaInstance());
                    break;
                case ParticleSystem3DZoneType::Torus:
                    mainModulesMetaInstances.insert(value.torus.getMetaInstance());
                    break;
                case ParticleSystem3DZoneType::Box:
                    mainModulesMetaInstances.insert(value.box.getMetaInstance());
                    break;
                case ParticleSystem3DZoneType::Circle:
                    mainModulesMetaInstances.insert(value.circle.getMetaInstance());
                    break;
                default:
                    MAZE_NOT_IMPLEMENTED;
                    break;
            }
            
        }

        for (MetaPropertyDrawerPtr property : m_zonePropertyDrawers[m_zoneType])
        {
            if (property)
            {
                property->linkMetaInstances(mainModulesMetaInstances);
                property->processDataToUI();
            }
        }
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DZoneData::processDataFromUI()
    {
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DZoneData::createZoneDrawer(
        ParticleSystem3DZoneType _type,
        MetaClass* _zoneMetaClass,
        Transform2DPtr const& _parent)
    {
        VerticalLayout2DPtr verticalLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), _parent->getHeight()),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        SizePolicy2DPtr sizePolicy = verticalLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        sizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        verticalLayout->setAutoHeight(true);
        verticalLayout->setExpand(false);

        m_zoneEntities[_type] = verticalLayout->getEntity();

        Vector<MetaClass*> const& componentSuperMetaClasses = _zoneMetaClass->getAllSuperMetaClasses();
        for (MetaClass* metaClass : componentSuperMetaClasses)
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                MetaProperty* metaProperty = metaClass->getProperty(i);

                MetaPropertyDrawerPtr property;
                
                if (strcmp(metaProperty->getName(), "angle") == 0)
                {
                    property = MetaPropertyDrawerDefault<F32, PropertyDrawerRadians>::Create(metaProperty);
                }
                else
                {
                    property = buildPropertyDrawer(metaProperty);
                }

                if (property)
                {
                    property->buildUI(verticalLayout->getTransform());

                    m_zonePropertyDrawers[_type].push_back(property);
                }
            }
        }
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DZoneData::setZoneType(ParticleSystem3DZoneType _value)
    {
        if (m_zoneType == _value)
            return;

        m_zoneType = _value;

        updateZoneType();
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DZoneData::updateZoneType()
    {
        for (ParticleSystem3DZoneType zoneType : ParticleSystem3DZoneType::All())
        {
            EntityPtr const& entity = m_zoneEntities[zoneType];
            if (entity)
                entity->setActiveSelf(zoneType == m_zoneType);
        }
    }

} // namespace Maze
//////////////////////////////////////////
