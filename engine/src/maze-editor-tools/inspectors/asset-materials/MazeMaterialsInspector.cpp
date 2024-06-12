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
#include "maze-editor-tools/inspectors/asset-materials/MazeMaterialsInspector.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
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
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerS32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF64.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2F32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3F32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4F32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2S32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3S32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4S32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2U32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3U32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4U32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerColorF128.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class EditorToolsMaterialChangedEvent
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(EditorToolsMaterialChangedEvent);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EditorToolsMaterialChangedEvent);


    //////////////////////////////////////////
    // Class MaterialsInspector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MaterialsInspector, Inspector);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(MaterialsInspector);

    //////////////////////////////////////////
    MaterialsInspector::MaterialsInspector()
        : m_materialsPropertiesListDirty(false)
    {
        
    }

    //////////////////////////////////////////
    MaterialsInspector::~MaterialsInspector()
    {

    }

    //////////////////////////////////////////
    MaterialsInspectorPtr MaterialsInspector::Create(Transform2DPtr const& _parent)
    {
        MaterialsInspectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MaterialsInspector, object, init(_parent));
        return object;
    }

    //////////////////////////////////////////
    bool MaterialsInspector::init(Transform2DPtr const& _parent)
    {
        if (!Inspector::init(_parent))
            return false;
        

        return true;
    }

    //////////////////////////////////////////
    void MaterialsInspector::update(F32 _dt)
    {
        MAZE_PROFILER_SCOPED_LOCK(INSPECTOR);
        MAZE_PROFILE_EVENT("MaterialsInspector::update");

        if (m_materialsPropertiesListDirty)
            buildMaterialsPropertiesList();

        for (MaterialsRenderPassDrawerPtr const& renderPassDrawer : m_renderPassDrawers)
            renderPassDrawer->updateValues();

        for (ShaderUniformVariantDrawerPtr const& shaderUniformVariantsDrawer : m_shaderUniformVariantsDrawers)
        {
            shaderUniformVariantsDrawer->linkMaterials(getMaterials());
            shaderUniformVariantsDrawer->processDataToUI();
        }

        Set<MaterialPtr> const& materials = getMaterials();
        // #TODO: Multi material editor
        if (materials.size() == 1 && m_materialCopy)
        {
            MaterialPtr const& material = *materials.begin();
            if (!material->isEqual(m_materialCopy))
            {
                if (material->getRenderPass() && m_materialCopy->getRenderPass() &&
                    material->getRenderPass()->getShader() != m_materialCopy->getRenderPass()->getShader())
                    m_materialsPropertiesListDirty = true;

                m_materialChangedTimer = 0.35f;
                m_materialCopy->set(material);
            }
            else
            {
                if (m_materialChangedTimer > 0.0f)
                {
                    m_materialChangedTimer -= _dt;
                    if (m_materialChangedTimer <= 0.0f)
                    {
                        if (EventManager::GetInstancePtr())
                            EventManager::GetInstancePtr()->broadcastEvent<EditorToolsMaterialChangedEvent>(material->getName());
                    }
                }
            }
        }
    }

    //////////////////////////////////////////
    void MaterialsInspector::setMaterials(Set<MaterialPtr> const& _materials)
    {
        m_materials = _materials;

        m_materialsPropertiesListDirty = true;
    }

    //////////////////////////////////////////
    bool MaterialsInspector::setAssetFiles(Set<AssetFilePtr> const& _assetFiles)
    {
        Set<MaterialPtr> materials;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        for (AssetFilePtr const& assetFile : _assetFiles)
        {
            MaterialPtr const& material = materialManager->getMaterial(assetFile);
            if (material)
                materials.insert(material);
        }

        setMaterials(materials);

        return true;
    }

    //////////////////////////////////////////
    bool MaterialsInspector::setObjects(Set<ObjectPtr> const& _objects)
    {
        Set<MaterialPtr> materials;
        for (ObjectPtr const& object : _objects)
            materials.insert(std::static_pointer_cast<Material>(object));

        setMaterials(materials);

        return true;
    }

    //////////////////////////////////////////
    void MaterialsInspector::buildMaterialsPropertiesList()
    {
        Set<MaterialPtr> const& materials = getMaterials();
        if (m_materialCopy && m_materialChangedTimer > 0.0f)
            if (EventManager::GetInstancePtr())
                EventManager::GetInstancePtr()->broadcastEvent<EditorToolsMaterialChangedEvent>(m_materialCopy->getName());

        m_parent->removeAllChildren();

        createRenderPassDrawers();
        createUniformVariantsDrawers();
        createSaveMaterialButton();
        m_materialsPropertiesListDirty = false;

        // #TODO: Multi material editor
        if (materials.size() == 1)
            m_materialCopy = (*materials.begin())->createCopy();
        else
            m_materialCopy.reset();
        m_materialChangedTimer = 0.0f;
    }

    //////////////////////////////////////////
    Set<MaterialPtr> const& MaterialsInspector::getMaterials()
    {
        return m_materials;
    }

    //////////////////////////////////////////
    void MaterialsInspector::clearRenderPassDrawers()
    {
        for (MaterialsRenderPassDrawerPtr const& drawer : m_renderPassDrawers)
            drawer->getEntityRaw()->removeFromEcsWorld();

        m_renderPassDrawers.clear();
    }

    //////////////////////////////////////////
    void MaterialsInspector::createRenderPassDrawers()
    {
        clearRenderPassDrawers();

        Set<MaterialPtr> const& materials = getMaterials();

        // #TODO: Multi material editor
        if (materials.size() == 1)
        {
            MaterialPtr const& material = *materials.begin();

            for (RenderPassType renderPassType = RenderPassType(1); renderPassType < RenderPassType::MAX; ++renderPassType)
            {
                Size renderPassesCount = material->getRenderPassesCount(renderPassType);
                for (Size i = 0; i < renderPassesCount; ++i)
                {
                    EntityPtr renderPassDrawerObject = m_parent->getEntityRaw()->getEcsScene()->createEntity();
                    MaterialsRenderPassDrawerPtr renderPassDrawer = renderPassDrawerObject->createComponent<MaterialsRenderPassDrawer>(renderPassType, (S32)i);
                    renderPassDrawer->getTransform()->setParent(m_parent);
                    m_renderPassDrawers.emplace_back(renderPassDrawer);

                    renderPassDrawer->setMaterials(materials);
                }
            }
        }
    }

    //////////////////////////////////////////
    void MaterialsInspector::clearUniformVariantsDrawers()
    {
        if (m_shaderUniformVariantsRoot)
        {
            if (m_shaderUniformVariantsRoot->getEntityRaw())
                m_shaderUniformVariantsRoot->getEntityRaw()->removeFromEcsWorld();
            m_shaderUniformVariantsRoot.reset();
        }

        if (m_shaderUniformVariantsLayout)
        {
            if (m_shaderUniformVariantsLayout->getEntityRaw())
                m_shaderUniformVariantsLayout->getEntityRaw()->removeFromEcsWorld();
            m_shaderUniformVariantsLayout.reset();
        }

        m_shaderUniformVariantsDrawers.clear();
    }

    //////////////////////////////////////////
    void MaterialsInspector::createUniformVariantsDrawers()
    {
        clearUniformVariantsDrawers();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        Set<MaterialPtr> const& materials = getMaterials();

        // #TODO: Multi material editor
        if (materials.size() == 1)
        {
            MaterialPtr const& material = *materials.begin();

            FastVector<ShaderUniformVariantPtr> const& uniforms = material->getUniforms();
            for (ShaderUniformVariantPtr const& uniformVariant : uniforms)
            {
                if (!material->hasUniform(uniformVariant->getName()))
                    continue;

                HashedString const& uniformName = uniformVariant->getName();
                ShaderUniformType shaderUniformType = uniformVariant->getType();

                ShaderUniformVariantDrawerPtr shaderUniformVariantDrawer;
                
                switch (shaderUniformType)
                {
                    case ShaderUniformType::UniformS32:          shaderUniformVariantDrawer = ShaderUniformVariantDrawerS32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformF32:          shaderUniformVariantDrawer = ShaderUniformVariantDrawerF32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformF64:          shaderUniformVariantDrawer = ShaderUniformVariantDrawerF64::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec2F32:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec2F32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec3F32:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec3F32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec4F32:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec4F32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec2S32:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec2S32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec3S32:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec3S32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec4S32:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec4S32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec2U32:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec2U32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec3U32:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec3U32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec4U32:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec4U32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformColorF128:    shaderUniformVariantDrawer = ShaderUniformVariantDrawerColorF128::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformTexture2D:    shaderUniformVariantDrawer = ShaderUniformVariantDrawerTexture2D::Create(uniformName, shaderUniformType); break;
                    default: continue;
                }

                if (shaderUniformVariantDrawer)
                {
                    if (shaderUniformVariantDrawer)
                        m_shaderUniformVariantsDrawers.emplace_back(shaderUniformVariantDrawer);
                }
            }
        }

        if (!m_shaderUniformVariantsDrawers.empty())
        {
            m_shaderUniformVariantsRoot = SpriteHelper::CreateTransform2D(
                Vec2F(m_parent->getWidth(), 0.0f),
                Vec2F::c_zero,
                m_parent,
                m_parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            SizePolicy2DPtr sizePolicy = m_shaderUniformVariantsRoot->getEntityRaw()->ensureComponent<SizePolicy2D>();
            sizePolicy->setFlag(SizePolicy2D::Height, false);

            VerticalLayout2DPtr verticalLayout = m_shaderUniformVariantsRoot->getEntityRaw()->ensureComponent<VerticalLayout2D>();
            verticalLayout->setAutoHeight(true);
            verticalLayout->setPaddingLeft(8.0f);

            SpriteRenderer2DPtr lineRenderer = SpriteHelper::CreateSprite(
                ColorU32(127, 127, 127),
                Vec2F(m_parent->getWidth(), 1),
                Vec2F(0, 0),
                renderSystem->getMaterialManager()->getSpriteMaterial(),
                m_shaderUniformVariantsRoot,
                m_shaderUniformVariantsRoot->getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            lineRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

            m_shaderUniformVariantsLayout = SpriteHelper::CreateTransform2D(
                Vec2F(m_parent->getWidth() - 16.0f, 50.0f),
                Vec2F(8.0f, 0.0f),
                m_shaderUniformVariantsRoot,
                m_shaderUniformVariantsRoot->getEntityRaw()->getEcsScene());
            SizePolicy2DPtr topBlockSizePolicy = m_shaderUniformVariantsLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
            topBlockSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
            topBlockSizePolicy->setSizeDelta(-16.0f, 0.0f);

            VerticalLayout2DPtr topBlockLayout = m_shaderUniformVariantsLayout->getEntityRaw()->ensureComponent<VerticalLayout2D>();
            topBlockLayout->setVerticalAlignment(VerticalAlignment2D::Middle);
            topBlockLayout->setAutoHeight(true);
            topBlockLayout->setPaddingTop(10.0f);
            topBlockLayout->setSpacing(2.0f);

            EditorToolsUIHelper::CreateText(
                "Uniforms",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(50.0f, 8.0f),
                Vec2F::c_zero,
                m_shaderUniformVariantsLayout,
                m_parent->getEntityRaw()->getEcsScene())->setColor(ColorU32::c_black);

            for (ShaderUniformVariantDrawerPtr const& drawer : m_shaderUniformVariantsDrawers)
                drawer->buildUI(m_shaderUniformVariantsLayout);
        }
    }

    //////////////////////////////////////////
    void MaterialsInspector::clearSaveMaterialButton()
    {
        if (m_saveMaterialButtonRoot)
        {
            m_saveMaterialButtonRoot->getEntityRaw()->removeFromEcsWorld();
            m_saveMaterialButtonRoot.reset();
        }
    }

    //////////////////////////////////////////
    void MaterialsInspector::createSaveMaterialButton()
    {
        clearSaveMaterialButton();

        if (!InspectorManager::GetInstancePtr()->getSaveAssetButtonEnabled())
            return;

        Set<MaterialPtr> const& materials = getMaterials();

        // #TODO: Multi material editor
        if (materials.size() == 1)
        {
            MaterialPtr const& material = *materials.begin();

            String assetFileName = material->getName();
            AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(assetFileName);
            if (assetFile)
            {
                String assetFileFullPath = assetFile->getFullPath();

                m_saveMaterialButtonRoot = SpriteHelper::CreateTransform2D(
                    Vec2F(m_parent->getWidth(), 22.0f),
                    Vec2F::c_zero,
                    m_parent,
                    m_parent->getEntityRaw()->getEcsScene(),
                    Vec2F(0.0f, 1.0f),
                    Vec2F(0.0f, 1.0f));
                SizePolicy2DPtr sizePolicy = m_saveMaterialButtonRoot->getEntityRaw()->ensureComponent<SizePolicy2D>();
                sizePolicy->setFlag(SizePolicy2D::Height, false);

                ClickButton2DPtr button = UIHelper::CreateDefaultClickButton(
                    String("Save Asset File").c_str(),
                    Vec2F(250, 18),
                    Vec2F::c_zero,
                    m_saveMaterialButtonRoot,
                    m_parent->getEntityRaw()->getEcsScene());

                button->eventClick.subscribe(
                    [assetFileName, assetFileFullPath](Button2D* _button, CursorInputEvent const& _event)
                    {
                        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getMaterial(assetFileName);
                        if (material)
                            material->saveToFile(assetFileFullPath);
                    });
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
