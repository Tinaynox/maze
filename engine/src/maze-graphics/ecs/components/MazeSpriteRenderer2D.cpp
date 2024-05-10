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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(SpriteRenderMode);


    //////////////////////////////////////////
    // Class SpriteRenderer2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SpriteRenderer2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SpriteAssetRef, sprite, SpriteAssetRef(), getSpriteRef, setSpriteRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(MaterialAssetRef, material, MaterialAssetRef(), getMaterialRef, setMaterialRefCopy),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, color, ColorU32::c_white, getColor, setColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SpriteRenderMode, renderMode, SpriteRenderMode::Simple, getRenderMode, setRenderMode));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SpriteRenderer2D);


    //////////////////////////////////////////
    SpriteRenderer2D::SpriteRenderer2D()
        : m_renderSystem(nullptr)
        , m_color(ColorU32::c_white)
        , m_renderMode(SpriteRenderMode::Simple)
        , m_customRenderCallback(nullptr)
    {
    }

    //////////////////////////////////////////
    SpriteRenderer2D::~SpriteRenderer2D()
    {
        setSprite(SpritePtr());
    }

    //////////////////////////////////////////
    SpriteRenderer2DPtr SpriteRenderer2D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        SpriteRenderer2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpriteRenderer2D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool SpriteRenderer2D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;
        return true;
    }

    //////////////////////////////////////////
    bool SpriteRenderer2D::init(
        Component* _component,
        EcsWorld* _world,
        EntityCopyData _copyData)
    {
        if (!Component::init(_component, _world, _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setSpriteRef(SpriteAssetRef const& _sprite)
    {
        if (m_spriteRef.getSprite() == _sprite.getSprite())
            return;

        if (m_spriteRef.getSprite())
        {
            m_spriteRef.getSprite()->eventDataChanged.unsubscribe(this);
        }

        m_spriteRef.setSprite(_sprite.getSprite());

        if (m_spriteRef.getSprite())
        {
            m_spriteRef.getSprite()->eventDataChanged.subscribe(this, &SpriteRenderer2D::notifySpriteDataChanged);

            updateMesh();
            updateMaterial();
        }
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setSprite(HashedString const& _spriteName)
    {
        Maze::RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        Maze::SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        Maze::SpritePtr const& sprite = spriteManager->getSprite(_spriteName.asHashedCString());
        setSprite(sprite);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setMaterialRef(MaterialAssetRef const& _material)
    {
        if (m_materialRef.getMaterial() == _material.getMaterial())
            return;

        m_materialRef.setMaterial(_material.getMaterial());

        updateMaterial();
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setMaterial(CString _materialName)
    {
        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getMaterial(_materialName);
        setMaterial(material);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setMaterialRefCopy(MaterialAssetRef const& _material)
    {
        setMaterial(_material.getMaterial() ? _material.getMaterial()->createCopy() : MaterialPtr());
        updateMaterial();
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setMaterialCopy(MaterialPtr const& _material)
    {
        setMaterial(_material ? _material->createCopy() : MaterialPtr());
        updateMaterial();
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setMaterialCopy(CString _materialName)
    {
        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getMaterial(_materialName);
        setMaterialCopy(material);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setColor(ColorU32 _color)
    {
        if (m_color == _color)
            return;

        m_color = _color;

        updateMaterial();
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setRenderMode(SpriteRenderMode _renderMode)
    {
        if (m_renderMode == _renderMode)
            return;

        m_renderMode = _renderMode;

        updateMesh();
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::setCustomRenderCallback(CustomRenderCallback _value)
    {
        m_customRenderCallback = _value;

        if (m_renderMode == SpriteRenderMode::Custom)
            updateMesh();
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::updateMaterial()
    {
        if (m_meshRenderer)
            m_meshRenderer->setMaterial(getMaterial());

        if (getMaterial())
        {
            getMaterial()->setUniform(MAZE_HCS("u_color"), m_color.toVec4F32());

            Texture2DPtr texture;

            if (getSprite())
            {
                texture = getSprite()->getTexture();

                if (!texture)
                    texture = getMaterial()->getRenderSystem()->getTextureManager()->ensureBuiltinTexture2D(BuiltinTexture2DType::Error);
            }
            else
            {
                texture = getMaterial()->getRenderSystem()->getTextureManager()->ensureBuiltinTexture2D(BuiltinTexture2DType::White);
            }

            if (texture)
            {
                getMaterial()->setUniform(MAZE_HCS("u_baseMap"), texture);
                getMaterial()->setUniform(MAZE_HCS("u_baseMapTexelSize"), 1.0f / (Vec2F)texture->getSize());
            }
        }
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::updateMesh()
    {
        if (!m_canvasRenderer)
            return;

        if (!getEntityRaw() || !getEntityRaw()->getEcsScene())
            return;

        Vec2F const& size = m_transform->getSize();

        Vec4F uv;

        if (getSprite())
        {
            uv = Vec4F(
                getSprite()->getTextureCoordLB(),
                getSprite()->getTextureCoordRT());
        }
        else
        {
            uv = Vec4F(0.0f, 0.0f, 1.0f, 1.0f);
        }

        MeshPtr mesh;
            
        switch (m_renderMode)
        {
            case SpriteRenderMode::Simple:
            {
                mesh = MeshHelper::CreateQuadMesh(
                    size,
                    size * 0.5f,
                    false,
                    uv,
                    Vec4F(1.0f, 1.0f, 1.0f, m_canvasRenderer->getAlpha()));

                break;
            }
            case SpriteRenderMode::Sliced:
            {
                if (getSprite())
                {
                    mesh = MeshHelper::CreateSlicedPanelMesh(
                        size,
                        size * 0.5f,
                        getSprite()->getSliceBorder(),
                        getSprite()->getNativeSize(),
                        uv,
                        Vec4F(1.0f, 1.0f, 1.0f, m_canvasRenderer->getAlpha()));
                }
                else
                {
                    mesh = MeshHelper::CreateQuadMesh(
                        size,
                        size * 0.5f,
                        false,
                        uv,
                        Vec4F(1.0f, 1.0f, 1.0f, m_canvasRenderer->getAlpha()));
                }
                break;
            }
            case SpriteRenderMode::Custom:
            {
                if (!m_customRenderCallback)
                    return;

                mesh = m_customRenderCallback(this);

                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
            }
        }
        m_meshRenderer->setMesh(mesh);
    }

    //////////////////////////////////////////
    void SpriteRenderer2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRenderer>();
        m_canvasRenderer = getEntityRaw()->ensureComponent<CanvasRenderer>();

        m_meshRenderer->setRenderMesh(RenderMeshPtr());
        updateMesh();
        updateMaterial();
    }
    
    //////////////////////////////////////////
    void SpriteRenderer2D::notifySpriteDataChanged(Sprite* _sprite)
    {
        updateMesh();
        updateMaterial();
    }
    
} // namespace Maze
//////////////////////////////////////////
