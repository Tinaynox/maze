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
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class CanvasRenderer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(CanvasRenderer, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(CanvasRenderer);

    //////////////////////////////////////////
    CanvasRenderer::CanvasRenderer()
        : m_alpha(1.0f)
        , m_flags(0)
    {
    }

    //////////////////////////////////////////
    CanvasRenderer::~CanvasRenderer()
    {

    }

    //////////////////////////////////////////
    CanvasRendererPtr CanvasRenderer::Create()
    {
        CanvasRendererPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(CanvasRenderer, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool CanvasRenderer::init()
    {

        return true;
    }

    //////////////////////////////////////////
    F32 CanvasRenderer::getAlpha()
    {
        if (m_flags & AlphaDirty)
            return calculateAlpha();
        else
            return m_alpha;
    }

    //////////////////////////////////////////
    F32 CanvasRenderer::calculateAlpha()
    {
        CanvasGroup* parent = getParentCanvasGroup();
        if (parent)
        {
            setAlpha(parent->getWorldAlpha());
        }
        else
        {
            setAlpha(1.0f);
        }

        m_flags &= ~AlphaDirty;

        return m_alpha;
    }

    //////////////////////////////////////////
    void CanvasRenderer::setAlpha(F32 _alpha)
    {
        if (m_alpha == _alpha)
            return;

        m_alpha = _alpha;

        m_flags |= Flags::AlphaChanged;
    }

    //////////////////////////////////////////
    CanvasGroup* CanvasRenderer::getParentCanvasGroup()
    {
        Transform2D* transform = m_transform.get();
        while (transform)
        {
            Entity* entity = transform->getEntityRaw();
            if (!entity)
                return nullptr;

            CanvasGroup* canvasGroup = entity->getComponentRaw<CanvasGroup>();
            if (canvasGroup)
                return canvasGroup;

            transform = transform->getParent().get();
        }

        return nullptr;
    }

    //////////////////////////////////////////
    void CanvasRenderer::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
    }
    
    
} // namespace Maze
//////////////////////////////////////////
