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
#include "maze-editor-tools/inspectors/asset-materials/MazeMaterialsRenderPassDrawer.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDString.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDS32.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDEnumClass.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDBool.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsActionManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorActionHelper.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionActionsGroup.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionCustom.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MaterialsRenderPassDrawer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MaterialsRenderPassDrawer, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(MaterialsRenderPassDrawer);

    //////////////////////////////////////////
    MaterialsRenderPassDrawer::MaterialsRenderPassDrawer()
        : m_renderPassType(RenderPassType::Default)
        , m_renderPassIndex(0)
    {
        
    }

    //////////////////////////////////////////
    MaterialsRenderPassDrawer::~MaterialsRenderPassDrawer()
    {

    }

    //////////////////////////////////////////
    MaterialsRenderPassDrawerPtr MaterialsRenderPassDrawer::Create(
        RenderPassType _renderPassType,
        S32 _renderPassIndex)
    {
        MaterialsRenderPassDrawerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MaterialsRenderPassDrawer, object, init(_renderPassType, _renderPassIndex));
        return object;
    }

    //////////////////////////////////////////
    bool MaterialsRenderPassDrawer::init(
        RenderPassType _renderPassType,
        S32 _renderPassIndex)
    {
        m_renderPassType = _renderPassType;
        m_renderPassIndex = _renderPassIndex;

        return true;
    }

    //////////////////////////////////////////
    void MaterialsRenderPassDrawer::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        SizePolicy2DPtr sizePolicy = getEntityRaw()->ensureComponent<SizePolicy2D>();
        sizePolicy->setFlag(SizePolicy2D::Height, false);

        VerticalLayout2DPtr verticalLayout = getEntityRaw()->ensureComponent<VerticalLayout2D>();
        verticalLayout->setAutoHeight(true);
        verticalLayout->setPaddingLeft(8.0f);


        m_topBlock = SpriteHelper::CreateTransform2D(
            Vec2F(m_transform->getWidth() - 16.0f, 50.0f),
            Vec2F(8.0f, 0.0f),
            m_transform,
            m_transform->getEntityRaw()->getEcsScene());
        SizePolicy2DPtr topBlockSizePolicy = m_topBlock->getEntityRaw()->ensureComponent<SizePolicy2D>();
        topBlockSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        topBlockSizePolicy->setSizeDelta(-16.0f, 0.0f);

        VerticalLayout2DPtr topBlockLayout = m_topBlock->getEntityRaw()->ensureComponent<VerticalLayout2D>();
        topBlockLayout->setVerticalAlignment(VerticalAlignment2D::Middle);
        topBlockLayout->setAutoHeight(true);
        topBlockLayout->setPaddingTop(10.0f);
        topBlockLayout->setPaddingBottom(10.0f);
        topBlockLayout->setSpacing(2.0f);

        EditorToolsUIHelper::CreateText(
            ("Render Pass - " + m_renderPassType.toString() + " #" + StringHelper::ToString(m_renderPassIndex)).c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(50.0f, 8.0f),
            Vec2F::c_zero,
            m_topBlock,
            m_transform->getEntityRaw()->getEcsScene())->setColor(ColorU32::c_black);

        // Shader
        {
            HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(m_transform->getWidth(), 25.0f),
                Vec2F::c_zero,
                m_topBlock,
                m_transform->getEntityRaw()->getEcsScene());
            SizePolicy2DPtr topBlockShaderSizePolicy = layout->getEntityRaw()->ensureComponent<SizePolicy2D>();
            topBlockShaderSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);

            layout->setExpand(true);
            layout->setAutoWidth(false);

            AbstractTextRenderer2DPtr topBlockShaderText = EditorToolsUIHelper::CreateText(
                "Shader",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(50.0f, 8.0f),
                Vec2F::c_zero,
                layout->getTransform(),
                m_transform->getEntityRaw()->getEcsScene());
            topBlockShaderText->setColor(ColorU32::c_black);

            ClickButton2DPtr shaderClickButton = UIHelper::CreateDefaultClickButton(
                nullptr,
                Vec2F(200.0f, 18.0f),
                Vec2F(0, 0),
                layout->getTransform(),
                m_transform->getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 0.5f),
                Vec2F(0.0f, 0.5f));

            m_topBlockShaderNameText = EditorToolsUIHelper::CreateText(
                "",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Center,
                VerticalAlignment2D::Middle,
                shaderClickButton->getTransform()->getSize(),
                Vec2F::c_zero,
                shaderClickButton->getTransform(),
                m_transform->getEntityRaw()->getEcsScene());
            m_topBlockShaderNameText->setColor(ColorU32::c_black);

            ScissorMask2DPtr shaderPanelBackgroundScissorMask = m_topBlockShaderNameText->getEntityRaw()->ensureComponent<ScissorMask2D>();
            shaderPanelBackgroundScissorMask->setPadding(1.0, 1.0f, 1.0f, 1.0f);

            MaterialsRenderPassDrawerWPtr drawerWeak = cast<MaterialsRenderPassDrawer>();

            ContextMenu2DPtr shaderClickButtonContextMenu = shaderClickButton->getEntityRaw()->ensureComponent<ContextMenu2D>();
            shaderClickButtonContextMenu->setCursorButtonIndex(0);
            shaderClickButtonContextMenu->setCallbackFunction(
                [drawerWeak](MenuListTree2DPtr const& _listTree)
                {
                    RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
                    StringKeyMap<ShaderLibraryData> const& shadersLibrary = renderSystem->getShaderSystem()->getShadersLibrary();

                    for (auto const& shaderData : shadersLibrary)
                    {
                        ShaderPtr const& shader = shaderData.second.shader;
                        ShaderWPtr shaderWeak = shader;

                        _listTree->addItem(
                            shader->getName(),
                            [drawerWeak, shaderWeak](String const& _text)
                            {
                                ShaderPtr shader = shaderWeak.lock();
                                if (shader)
                                {
                                    auto drawer = drawerWeak.lock();
                                    if (drawer)
                                        drawer->setShader(shader);
                                }
                            },
                            nullptr,
                            false);
                    }

                    _listTree->rebuildItems();
                });
        }

        {
            MetaPropertyDrawerPtr propertyDrawer = MetaPropertyDrawerS32::Create(
                RenderPass::GetMetaClass()->getProperty("renderQueueIndex"));
            propertyDrawer->buildUI(m_topBlock, "Render Queue");
            m_propertyDrawers.push_back(propertyDrawer);
        }
        {
            MetaPropertyDrawerPtr propertyDrawer = MetaPropertyDrawerEnumClass::Create(
                RenderPass::GetMetaClass()->getProperty("blendOperation"),
                BlendOperation::AllStrings());
            propertyDrawer->buildUI(m_topBlock, "Blend Operation");
            m_propertyDrawers.push_back(propertyDrawer);
        }
        {
            MetaPropertyDrawerPtr propertyDrawer = MetaPropertyDrawerEnumClass::Create(
                RenderPass::GetMetaClass()->getProperty("blendSrcFactor"),
                BlendFactor::AllStrings());
            propertyDrawer->buildUI(m_topBlock, "Src");
            m_propertyDrawers.push_back(propertyDrawer);
        }
        {
            MetaPropertyDrawerPtr propertyDrawer = MetaPropertyDrawerEnumClass::Create(
                RenderPass::GetMetaClass()->getProperty("blendDestFactor"),
                BlendFactor::AllStrings());
            propertyDrawer->buildUI(m_topBlock, "Dest");
            m_propertyDrawers.push_back(propertyDrawer);
        }
        {
            MetaPropertyDrawerPtr propertyDrawer = MetaPropertyDrawerEnumClass::Create(
                RenderPass::GetMetaClass()->getProperty("depthTestCompareFunction"),
                CompareFunction::AllStrings());
            propertyDrawer->buildUI(m_topBlock, "Depth Test");
            m_propertyDrawers.push_back(propertyDrawer);
        }
        {
            MetaPropertyDrawerPtr propertyDrawer = MetaPropertyDrawerBool::Create(
                RenderPass::GetMetaClass()->getProperty("depthWriteEnabled"));
            propertyDrawer->buildUI(m_topBlock, "Depth Write");
            m_propertyDrawers.push_back(propertyDrawer);
        }
        {
            MetaPropertyDrawerPtr propertyDrawer = MetaPropertyDrawerEnumClass::Create(
                RenderPass::GetMetaClass()->getProperty("cullMode"),
                CullMode::AllStrings());
            propertyDrawer->buildUI(m_topBlock, "Cull Mode");
            m_propertyDrawers.push_back(propertyDrawer);
        }
    }

    //////////////////////////////////////////
    void MaterialsRenderPassDrawer::setMaterials(Set<MaterialPtr> const& _materials)
    {
        m_materials = _materials;
    }

    //////////////////////////////////////////
    void MaterialsRenderPassDrawer::updateValues()
    {
        if (m_materials.empty())
        {
            m_topBlockShaderNameText->setText("");
        }
        else
        if (m_materials.size() > 1)
        {
            m_topBlockShaderNameText->setText("---");
        }
        else
        {
            MaterialPtr const& material = *m_materials.begin();
            RenderPassPtr const& renderPass = material->getRenderPass(m_renderPassType, m_renderPassIndex);
            if (renderPass)
            {
                ShaderPtr const& shader = renderPass->getShader();
                if (shader)
                    m_topBlockShaderNameText->setText(shader->getName());
                else
                    m_topBlockShaderNameText->setText("None");
            }
        }

        Set<MetaInstance> renderPassMetaInstances;

        for (MaterialPtr const& material : m_materials)
        {
            RenderPassPtr const& renderPass = material->getRenderPass(m_renderPassType, m_renderPassIndex);
            if (renderPass)
                renderPassMetaInstances.insert(renderPass->getMetaInstance());
        }

        for (MetaPropertyDrawerPtr const& propertyDrawer : m_propertyDrawers)
        {
            propertyDrawer->linkMetaInstances(renderPassMetaInstances);
            propertyDrawer->processDataToUI();
        }
        
    }

    //////////////////////////////////////////
    void MaterialsRenderPassDrawer::setShader(ShaderPtr const& _shader)
    {
        if (EditorToolsActionManager::GetInstancePtr())
        {
            EditorActionActionsGroupPtr group = EditorActionActionsGroup::Create();

            for (MaterialPtr const& material : m_materials)
            {
                RenderPassPtr renderPass = material->getRenderPass(m_renderPassType, m_renderPassIndex);
                if (renderPass)
                {
                    group->addAction(
                        EditorActionCustom::Create(
                            [renderPass, newShader = (ShaderPtr)_shader]() { renderPass->setShader(newShader); },
                            [renderPass, oldShader = renderPass->getShader()]() { renderPass->setShader(oldShader); }));
                }
            }

            if (group->getActionsCount() > 0)
                EditorToolsActionManager::GetInstancePtr()->applyAction(group);
        }
        else
        {
            for (MaterialPtr const& material : m_materials)
            {
                RenderPassPtr const& renderPass = material->getRenderPass(m_renderPassType, m_renderPassIndex);
                if (renderPass)
                    renderPass->setShader(_shader);
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
