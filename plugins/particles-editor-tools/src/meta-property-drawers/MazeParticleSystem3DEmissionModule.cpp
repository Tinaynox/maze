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
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DEmissionModule.hpp"
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
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazeParticleSystemParameterF32.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazeParticleSystemBurst.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MetaPropertyDrawerParticleSystem3DEmissionModule
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MetaPropertyDrawerParticleSystem3DEmissionModule, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerParticleSystem3DEmissionModule);

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DEmissionModule::MetaPropertyDrawerParticleSystem3DEmissionModule()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DEmissionModule::~MetaPropertyDrawerParticleSystem3DEmissionModule()
    {
        if (m_enabledDrawer)
        {
            m_enabledDrawer->eventUIData.unsubscribe(this);
            m_enabledDrawer.reset();
        }

        if (m_emissionPerSecondDrawer)
        {
            m_emissionPerSecondDrawer->eventUIData.unsubscribe(this);
            m_emissionPerSecondDrawer.reset();
        }

        if (m_emissionPerDistanceDrawer)
        {
            m_emissionPerDistanceDrawer->eventUIData.unsubscribe(this);
            m_emissionPerDistanceDrawer.reset();
        }
    }

    //////////////////////////////////////////
    MetaPropertyDrawerParticleSystem3DEmissionModulePtr MetaPropertyDrawerParticleSystem3DEmissionModule::Create(MetaProperty* _metaProperty)
    {
        MetaPropertyDrawerParticleSystem3DEmissionModulePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerParticleSystem3DEmissionModule, object, init(_metaProperty));
        return object;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerParticleSystem3DEmissionModule::init(MetaProperty* _metaProperty)
    {
        if (!MetaPropertyDrawer::init(_metaProperty))
            return false;

        DataBlock data;
        data.setCString(MAZE_HCS("label"), "Emission");
        m_enabledDrawer = PropertyDrawerBool::Create(data);
        m_enabledDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerParticleSystem3DEmissionModule::processDataFromUI);

        data.setCString(MAZE_HCS("label"), "Per Second");
        m_emissionPerSecondDrawer = PropertyDrawerParticleSystemParameterF32Positive::Create(data);
        m_emissionPerSecondDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerParticleSystem3DEmissionModule::processDataFromUI);

        data.setCString(MAZE_HCS("label"), "Per Meters");
        m_emissionPerDistanceDrawer = PropertyDrawerParticleSystemParameterF32Positive::Create(data);
        m_emissionPerDistanceDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerParticleSystem3DEmissionModule::processDataFromUI);

        data.setCString(MAZE_HCS("label"), "Bursts");
        m_burstsDrawer = PropertyDrawerVector::Create(
            ParticleSystemBurst::GetMetaClass()->getClassUID(),
            data);
        m_burstsDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerParticleSystem3DEmissionModule::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DEmissionModule::buildUI(
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
        m_emissionPerSecondDrawer->buildUI(verticalLayout->getTransform(), _label);
        m_emissionPerDistanceDrawer->buildUI(verticalLayout->getTransform(), _label);
        m_burstsDrawer->buildUI(verticalLayout->getTransform(), "Bursts");
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DEmissionModule::processDataToUI()
    {
        ParticleSystem3DMainModule::EmissionModule value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        m_processingDataToUI = true;
        {
            m_emissionPerSecondDrawer->setValue(value.getEmissionPerSecond());
            m_emissionPerDistanceDrawer->setValue(value.getEmissionPerDistance());
            m_enabledDrawer->setValue(value.getEnabled());

            m_emissionPerSecondDrawer->getRootEntity()->setActiveSelf(value.getEnabled());
            m_emissionPerDistanceDrawer->getRootEntity()->setActiveSelf(value.getEnabled());

            DataBlock bursts;
            ValueToDataBlock(value.getBursts(), bursts);
            m_burstsDrawer->setDataBlock(bursts);
        }
        m_processingDataToUI = false;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DEmissionModule::processDataFromUI()
    {
        if (m_processingDataToUI)
            return;

        ParticleSystem3DMainModule::EmissionModule value;
        value.setEnabled(m_enabledDrawer->getValue());
        value.setEmissionPerSecond(m_emissionPerSecondDrawer->getValue());
        value.setEmissionPerDistance(m_emissionPerDistanceDrawer->getValue());


        DataBlock dataBlock;
        m_burstsDrawer->toDataBlock(dataBlock);

        Vector<ParticleSystemBurst> bursts;
        ValueFromDataBlock(bursts, dataBlock);

        bool burstsInderectOrder = false;
        for (S32 i = 0, in = (S32)bursts.size(); i < in - 1; ++i)
        {
            if (bursts[i].time > bursts[i + 1].time)
            {
                burstsInderectOrder = true;
                break;
            }
        }

        if (burstsInderectOrder)
        {
            for (PropertyDrawerPtr const& itemDrawer : m_burstsDrawer->getItemDrawers())
                itemDrawer->castRaw<PropertyDrawerParticleSystemBurst>()->resetSelection();
        }

        value.setBursts(bursts);

        for (MetaInstance const& metaInstance : m_metaInstances)
            m_metaProperty->setValue(metaInstance, &value);
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerParticleSystem3DEmissionModule::processMetaInstancesChanged()
    {
        
    }


} // namespace Maze
//////////////////////////////////////////
