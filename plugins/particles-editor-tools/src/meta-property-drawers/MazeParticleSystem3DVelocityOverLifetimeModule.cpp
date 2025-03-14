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
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DVelocityOverLifetimeModule.hpp"
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
#include "maze-plugin-particles-editor-tools/property-drawers/MazeParticleSystemParameterF32.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule);

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::~MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule()
    {
        if (m_enabledDrawer)
        {
            m_enabledDrawer->eventUIData.unsubscribe(this);
            m_enabledDrawer.reset();
        }

        if (m_linearXParameterDrawer)
        {
            m_linearXParameterDrawer->eventUIData.unsubscribe(this);
            m_linearXParameterDrawer.reset();
        }

        if (m_linearYParameterDrawer)
        {
            m_linearYParameterDrawer->eventUIData.unsubscribe(this);
            m_linearYParameterDrawer.reset();
        }

        if (m_linearZParameterDrawer)
        {
            m_linearZParameterDrawer->eventUIData.unsubscribe(this);
            m_linearZParameterDrawer.reset();
        }
    }

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModulePtr MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::Create(MetaProperty* _metaProperty)
    {
        MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModulePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule, object, init(_metaProperty));
        return object;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::init(MetaProperty* _metaProperty)
    {
        if (!MetaPropertyDrawer::init(_metaProperty))
            return false;

        DataBlock data;
        data.setCString(MAZE_HCS("label"), "Velocity over Lifetime");
        m_enabledDrawer = PropertyDrawerBool::Create(data);
        m_enabledDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::processDataFromUI);

        data.setCString(MAZE_HCS("label"), "Linear X");
        m_linearXParameterDrawer = PropertyDrawerParticleSystemParameterF32::Create(data);
        m_linearXParameterDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::processDataFromUI);

        data.setCString(MAZE_HCS("label"), "Linear Y");
        m_linearYParameterDrawer = PropertyDrawerParticleSystemParameterF32::Create(data);
        m_linearYParameterDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::processDataFromUI);

        data.setCString(MAZE_HCS("label"), "Linear Z");
        m_linearZParameterDrawer = PropertyDrawerParticleSystemParameterF32::Create(data);
        m_linearZParameterDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::buildUI(
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
        m_linearXParameterDrawer->buildUI(verticalLayout->getTransform(), _label);
        m_linearYParameterDrawer->buildUI(verticalLayout->getTransform(), _label);
        m_linearZParameterDrawer->buildUI(verticalLayout->getTransform(), _label);
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::processDataToUI()
    {
        ParticleSystem3DMainModule::VelocityOverLifetimeModule value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        m_processingDataToUI = true;
        {
            m_linearXParameterDrawer->setValue(value.getLinearXParameter());
            m_linearYParameterDrawer->setValue(value.getLinearYParameter());
            m_linearZParameterDrawer->setValue(value.getLinearZParameter());

            m_enabledDrawer->setValue(value.getEnabled());

            m_linearXParameterDrawer->getRootEntity()->setActiveSelf(value.getEnabled());
            m_linearYParameterDrawer->getRootEntity()->setActiveSelf(value.getEnabled());
            m_linearZParameterDrawer->getRootEntity()->setActiveSelf(value.getEnabled());
        }
        m_processingDataToUI = false;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DVelocityOverLifetimeModule::processDataFromUI()
    {
        if (m_processingDataToUI)
            return;

        ParticleSystem3DMainModule::VelocityOverLifetimeModule value;
        value.setEnabled(m_enabledDrawer->getValue());
        value.setLinearXParameter(m_linearXParameterDrawer->getValue());
        value.setLinearYParameter(m_linearYParameterDrawer->getValue());
        value.setLinearZParameter(m_linearZParameterDrawer->getValue());

        for (MetaInstance const& metaInstance : m_metaInstances)
            m_metaProperty->setValue(metaInstance, &value);
    }


} // namespace Maze
//////////////////////////////////////////
