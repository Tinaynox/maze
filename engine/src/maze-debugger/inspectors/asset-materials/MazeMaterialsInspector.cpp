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
#include "maze-debugger/inspectors/asset-materials/MazeMaterialsInspector.hpp"
#include "maze-debugger/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-debugger/managers/MazeInspectorManager.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerS32.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF32.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF64.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DF.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DF.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DF.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DS.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DS.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DS.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DU.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DU.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DU.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerColorF128.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{


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

        if (m_materialsPropertiesListDirty)
            buildMaterialsPropertiesList();

        for (MaterialsRenderPassDrawerPtr const& renderPassDrawer : m_renderPassDrawers)
            renderPassDrawer->updateValues();

        for (ShaderUniformVariantDrawerPtr const& shaderUniformVariantsDrawer : m_shaderUniformVariantsDrawers)
        {
            shaderUniformVariantsDrawer->linkMaterials(getMaterials());
            shaderUniformVariantsDrawer->processDataToUI();
        }
    }

    //////////////////////////////////////////
    void MaterialsInspector::setMaterials(Set<MaterialPtr> const& _materials)
    {
        m_materials = _materials;

        m_materialsPropertiesListDirty = true;
    }

    //////////////////////////////////////////
    void MaterialsInspector::buildMaterialsPropertiesList()
    {
        m_parent->removeAllChildren();

        createRenderPassDrawers();
        createUniformVariantsDrawers();
        createSaveMaterialButton();
        m_materialsPropertiesListDirty = false;
    }

    //////////////////////////////////////////
    Set<MaterialPtr> MaterialsInspector::getMaterials()
    {
        return m_materials;
    }

    //////////////////////////////////////////
    void MaterialsInspector::clearRenderPassDrawers()
    {
        for (MaterialsRenderPassDrawerPtr const& drawer : m_renderPassDrawers)
            drawer->getEntityRaw()->removeFromECSWorld();

        m_renderPassDrawers.clear();
    }

    //////////////////////////////////////////
    void MaterialsInspector::createRenderPassDrawers()
    {
        clearRenderPassDrawers();

        Set<MaterialPtr> materials = getMaterials();

        // #TODO: Multi material editor
        if (materials.size() == 1)
        {
            MaterialPtr const& material = *materials.begin();

            for (RenderPassType renderPassType = RenderPassType(1); renderPassType < RenderPassType::MAX; ++renderPassType)
            {
                Size renderPassesCount = material->getRenderPassesCount(renderPassType);
                for (Size i = 0; i < renderPassesCount; ++i)
                {
                    EntityPtr renderPassDrawerObject = m_parent->getEntityRaw()->getECSScene()->createEntity();
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
                m_shaderUniformVariantsRoot->getEntityRaw()->removeFromECSWorld();
            m_shaderUniformVariantsRoot.reset();
        }

        if (m_shaderUniformVariantsLayout)
        {
            if (m_shaderUniformVariantsLayout->getEntityRaw())
                m_shaderUniformVariantsLayout->getEntityRaw()->removeFromECSWorld();
            m_shaderUniformVariantsLayout.reset();
        }

        m_shaderUniformVariantsDrawers.clear();
    }

    //////////////////////////////////////////
    void MaterialsInspector::createUniformVariantsDrawers()
    {
        clearUniformVariantsDrawers();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        Set<MaterialPtr> materials = getMaterials();

        // #TODO: Multi material editor
        if (materials.size() == 1)
        {
            MaterialPtr const& material = *materials.begin();

            FastVector<ShaderUniformVariantPtr> const& uniforms = material->getUniforms();
            for (ShaderUniformVariantPtr const& uniformVariant : uniforms)
            {
                HashedString const& uniformName = uniformVariant->getName();
                ShaderUniformType shaderUniformType = uniformVariant->getType();

                ShaderUniformVariantDrawerPtr shaderUniformVariantDrawer;
                
                switch (shaderUniformType)
                {
                    case ShaderUniformType::UniformS32:          shaderUniformVariantDrawer = ShaderUniformVariantDrawerS32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformF32:          shaderUniformVariantDrawer = ShaderUniformVariantDrawerF32::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformF64:          shaderUniformVariantDrawer = ShaderUniformVariantDrawerF64::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec2DF:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec2DF::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec3DF:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec3DF::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec4DF:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec4DF::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec2DS:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec2DS::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec3DS:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec3DS::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec4DS:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec4DS::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec2DU:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec2DU::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec3DU:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec3DU::Create(uniformName, shaderUniformType); break;
                    case ShaderUniformType::UniformVec4DU:       shaderUniformVariantDrawer = ShaderUniformVariantDrawerVec4DU::Create(uniformName, shaderUniformType); break;
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
                Vec2DF(m_parent->getWidth(), 0.0f),
                Vec2DF::c_zero,
                m_parent,
                m_parent->getEntityRaw()->getECSScene(),
                Vec2DF(0.0f, 1.0f),
                Vec2DF(0.0f, 1.0f));
            SizePolicy2DPtr sizePolicy = m_shaderUniformVariantsRoot->getEntityRaw()->ensureComponent<SizePolicy2D>();
            sizePolicy->setFlag(SizePolicy2D::Height, false);

            VerticalLayout2DPtr verticalLayout = m_shaderUniformVariantsRoot->getEntityRaw()->ensureComponent<VerticalLayout2D>();
            verticalLayout->setAutoHeight(true);
            verticalLayout->setPaddingLeft(8.0f);

            SpriteRenderer2DPtr lineRenderer = SpriteHelper::CreateSprite(
                ColorU32(127, 127, 127),
                Vec2DF(m_parent->getWidth(), 1),
                Vec2DF(0, 0),
                renderSystem->getMaterialManager()->getColorMaterial(),
                m_shaderUniformVariantsRoot,
                m_shaderUniformVariantsRoot->getEntityRaw()->getECSScene(),
                Vec2DF(0.0f, 1.0f),
                Vec2DF(0.0f, 1.0f));
            lineRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

            m_shaderUniformVariantsLayout = SpriteHelper::CreateTransform2D(
                Vec2DF(m_parent->getWidth() - 16.0f, 50.0f),
                Vec2DF(8.0f, 0.0f),
                m_shaderUniformVariantsRoot,
                m_shaderUniformVariantsRoot->getEntityRaw()->getECSScene());
            SizePolicy2DPtr topBlockSizePolicy = m_shaderUniformVariantsLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
            topBlockSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
            topBlockSizePolicy->setSizeDelta(-16.0f, 0.0f);

            VerticalLayout2DPtr topBlockLayout = m_shaderUniformVariantsLayout->getEntityRaw()->ensureComponent<VerticalLayout2D>();
            topBlockLayout->setVerticalAlignment(VerticalAlignment2D::Middle);
            topBlockLayout->setAutoHeight(true);
            topBlockLayout->setPaddingTop(10.0f);
            topBlockLayout->setSpacing(2.0f);

            SpriteHelper::CreateSystemText(
                "Uniforms",
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2DF(50.0f, 8.0f),
                Vec2DF::c_zero,
                m_shaderUniformVariantsLayout,
                m_parent->getEntityRaw()->getECSScene())->setColor(ColorU32::c_black);

            for (ShaderUniformVariantDrawerPtr const& drawer : m_shaderUniformVariantsDrawers)
                drawer->buildUI(m_shaderUniformVariantsLayout);
        }
    }

    //////////////////////////////////////////
    void MaterialsInspector::clearSaveMaterialButton()
    {
        if (m_saveMaterialButtonRoot)
        {
            m_saveMaterialButtonRoot->getEntityRaw()->removeFromECSWorld();
            m_saveMaterialButtonRoot.reset();
        }
    }

    //////////////////////////////////////////
    void MaterialsInspector::createSaveMaterialButton()
    {
        clearSaveMaterialButton();

        Set<MaterialPtr> materials = getMaterials();

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
                    Vec2DF(m_parent->getWidth(), 22.0f),
                    Vec2DF::c_zero,
                    m_parent,
                    m_parent->getEntityRaw()->getECSScene(),
                    Vec2DF(0.0f, 1.0f),
                    Vec2DF(0.0f, 1.0f));
                SizePolicy2DPtr sizePolicy = m_saveMaterialButtonRoot->getEntityRaw()->ensureComponent<SizePolicy2D>();
                sizePolicy->setFlag(SizePolicy2D::Height, false);

                ClickButton2DPtr button = UIHelper::CreateDefaultClickButton(
                    (String("Save ") + material->getName()).c_str(),
                    Vec2DF(250, 18),
                    Vec2DF::c_zero,
                    m_saveMaterialButtonRoot,
                    m_parent->getEntityRaw()->getECSScene());

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
