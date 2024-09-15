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
#include "maze-editor-tools/inspectors/asset-materials/MazeTexture2DsInspector.hpp"
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
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
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
    // Class EditorToolsTexture2DChangedEvent
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(EditorToolsTexture2DChangedEvent);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EditorToolsTexture2DChangedEvent);


    //////////////////////////////////////////
    // Class Texture2DsInspector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture2DsInspector, Inspector);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(Texture2DsInspector);

    //////////////////////////////////////////
    Texture2DsInspector::Texture2DsInspector()
        : m_texturesPropertiesListDirty(false)
    {
        
    }

    //////////////////////////////////////////
    Texture2DsInspector::~Texture2DsInspector()
    {

    }

    //////////////////////////////////////////
    Texture2DsInspectorPtr Texture2DsInspector::Create(Transform2DPtr const& _parent)
    {
        Texture2DsInspectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Texture2DsInspector, object, init(_parent));
        return object;
    }

    //////////////////////////////////////////
    bool Texture2DsInspector::init(Transform2DPtr const& _parent)
    {
        if (!Inspector::init(_parent))
            return false;
        

        return true;
    }

    //////////////////////////////////////////
    void Texture2DsInspector::update(F32 _dt)
    {
        MAZE_PROFILER_SCOPED_LOCK(INSPECTOR);
        MAZE_PROFILE_EVENT("Texture2DsInspector::update");

        if (m_texturesPropertiesListDirty)
            buildTexturesPropertiesList();

        updatePropertyValues();

        Set<Texture2DPtr> const& textures = getTextures();
        // #TODO: Multi texture editor
        if (textures.size() == 1 && m_textureCopy)
        {
            Texture2DPtr const& texture = *textures.begin();
            if (!MetaClassHelper::IsEqual(m_textureCopy->getMetaInstance(), texture->getMetaInstance()))
            {
                m_textureChangedTimer = 0.35f;
                MetaClassHelper::CopyProperties(m_textureCopy->getMetaInstance(), texture->getMetaInstance());
            }
            else
            {
                if (m_textureChangedTimer > 0.0f)
                {
                    m_textureChangedTimer -= _dt;
                    if (m_textureChangedTimer <= 0.0f)
                    {
                        if (EventManager::GetInstancePtr())
                            EventManager::GetInstancePtr()->broadcastEvent<EditorToolsTexture2DChangedEvent>(texture->getName());
                    }
                }
            }
        }
    }

    //////////////////////////////////////////
    void Texture2DsInspector::updatePropertyValues()
    {
        Set<MetaInstance> metaInstances;
        for (Texture2DPtr const& texture : m_textures)
            metaInstances.insert(texture->getMetaInstance());

        for (MetaPropertyDrawerPtr const& propertyDrawer : m_propertyDrawers)
        {
            propertyDrawer->linkMetaInstances(metaInstances);
            propertyDrawer->processDataToUI();
        }
    }

    //////////////////////////////////////////
    void Texture2DsInspector::setTextures(Set<Texture2DPtr> const& _textures)
    {
        m_textures = _textures;

        m_texturesPropertiesListDirty = true;
    }

    //////////////////////////////////////////
    bool Texture2DsInspector::setAssetFiles(Set<AssetFilePtr> const& _assetFiles)
    {
        Set<Texture2DPtr> textures;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        TextureManagerPtr const& textureManager = renderSystem->getTextureManager();

        for (AssetFilePtr const& assetFile : _assetFiles)
        {
            Texture2DPtr const& texture = textureManager->getOrLoadTexture2D(assetFile);
            if (texture)
                textures.insert(texture);
        }

        setTextures(textures);

        return true;
    }

    //////////////////////////////////////////
    bool Texture2DsInspector::setObjects(Set<ObjectPtr> const& _objects)
    {
        Set<Texture2DPtr> textures;
        for (ObjectPtr const& object : _objects)
            textures.insert(std::static_pointer_cast<Texture2D>(object));

        setTextures(textures);

        return true;
    }

    //////////////////////////////////////////
    void Texture2DsInspector::buildTexturesPropertiesList()
    {
        Set<Texture2DPtr> const& textures = getTextures();
        if (m_textureCopy && m_textureChangedTimer > 0.0f)
            EventManager::GetInstancePtr()->broadcastEvent<EditorToolsTexture2DChangedEvent>(m_textureCopy->getName());

        m_parent->destroyAllChildren();

        m_propertyDrawers.clear();
        // #TODO: Multi texture editor
        if (textures.size() == 1)
        {
            Texture2DPtr const& texture = *textures.begin();

            EntityPtr drawerObject = m_parent->getEntityRaw()->getEcsScene()->createEntity();
            Transform2DPtr transform = drawerObject->ensureComponent<Transform2D>();
            transform->setParent(m_parent);
            SizePolicy2DPtr sizePolicy = drawerObject->ensureComponent<SizePolicy2D>();
            sizePolicy->setFlag(SizePolicy2D::Height, false);
            sizePolicy->setSizeDelta(-16.0f, 0.0f);

            VerticalLayout2DPtr verticalLayout = drawerObject->ensureComponent<VerticalLayout2D>();
            verticalLayout->setAutoHeight(true);
            verticalLayout->setPaddingLeft(8.0f);
            verticalLayout->setPaddingTop(5.0f);

            Vector<MetaClass*> const& componentSuperMetaClasses = texture->getMetaClass()->getAllSuperMetaClasses();
            for (MetaClass* metaClass : componentSuperMetaClasses)
            {
                for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
                {
                    MetaProperty* metaProperty = metaClass->getProperty(i);
                    MetaPropertyDrawerPtr propertyDrawer = InspectorManager::GetInstancePtr()->createMetaPropertyDrawer(metaProperty);
                    if (propertyDrawer)
                    {
                        propertyDrawer->buildUI(verticalLayout->getTransform());
                        m_propertyDrawers.push_back(propertyDrawer);
                    }
                }
            }

        }
        m_texturesPropertiesListDirty = false;

        // #TODO: Multi texture editor
        if (textures.size() == 1)
        {
            Texture2DPtr const& texture = *textures.begin();
            m_textureCopy = Texture2D::Create();
            MetaClassHelper::CopyProperties(m_textureCopy->getMetaInstance(), texture->getMetaInstance());
        }
        else
            m_textureCopy.reset();
        m_textureChangedTimer = 0.0f;
    }

    //////////////////////////////////////////
    Set<Texture2DPtr> const& Texture2DsInspector::getTextures()
    {
        return m_textures;
    }

} // namespace Maze
//////////////////////////////////////////
