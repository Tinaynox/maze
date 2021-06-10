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
    // Class CanvasGroup
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(CanvasGroup, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, localAlpha, 1.0f, getLocalAlpha, setLocalAlpha),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, blocksRaycasts, true, getBlocksRaycasts, setBlocksRaycasts));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(CanvasGroup);

    //////////////////////////////////////////
    CanvasGroup::CanvasGroup()
        : m_localAlpha(1.0f)
        , m_worldAlpha(1.0f)
        , m_blocksRaycasts(true)
        , m_flags(0)
    {
    }

    //////////////////////////////////////////
    CanvasGroup::~CanvasGroup()
    {

    }

    //////////////////////////////////////////
    CanvasGroupPtr CanvasGroup::Create()
    {
        CanvasGroupPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(CanvasGroup, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool CanvasGroup::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void CanvasGroup::setLocalAlpha(F32 _alpha)
    {
        if (m_localAlpha == _alpha)
            return;

        m_localAlpha = _alpha;

        m_flags |= LocalAlphaChangedEvent;
            
        dirtyWorldAlphaRecursive();
    }

    //////////////////////////////////////////
    void CanvasGroup::dirtyWorldAlphaRecursive()
    {
        m_flags |= (Flags::WorldAlphaDirty | Flags::WorldAlphaChangedEvent);

        _dirtyWorldAlphaRecursive(m_transform.get());
    }

    //////////////////////////////////////////
    void CanvasGroup::_dirtyWorldAlphaRecursive(Transform2D* _transform2D)
    {
        for (Transform2D* transform2D : _transform2D->getChildren())
        {
            CanvasGroup* canvasGroup = transform2D->getEntityRaw()->getComponentRaw<CanvasGroup>();
            if (canvasGroup)
            {
                canvasGroup->dirtyWorldAlphaRecursive();
                continue;
            }

            _dirtyWorldAlphaRecursive(transform2D);
        }
    }

    //////////////////////////////////////////
    F32 CanvasGroup::getWorldAlpha()
    {
        if (m_flags & Flags::WorldAlphaDirty)
            return calculateWorldAlpha();
        else
            return m_worldAlpha;
    }

    //////////////////////////////////////////
    F32 CanvasGroup::calculateWorldAlpha()
    {
        CanvasGroup* parent = getParentCanvasGroup();
        if (parent)
        {
            m_worldAlpha = parent->getWorldAlpha() * m_localAlpha;
        }
        else
        {
            m_worldAlpha = m_localAlpha;
        }

        m_flags &= ~Flags::WorldAlphaDirty;

        return m_worldAlpha;
    }

    //////////////////////////////////////////
    CanvasGroup* CanvasGroup::getParentCanvasGroup()
    {
        Transform2D* transform = m_transform->getParent().get();
        while (transform)
        {
            CanvasGroup* canvasGroup = transform->getEntityRaw()->getComponentRaw<CanvasGroup>();
            if (canvasGroup)
                return canvasGroup;

            transform = transform->getParent().get();
        }

        return nullptr;
    }

    //////////////////////////////////////////
    void CanvasGroup::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

        dirtyWorldAlphaRecursive();
    }

    //////////////////////////////////////////
    void CanvasGroup::setBlocksRaycasts(bool _value)
    {
        if (m_blocksRaycasts == _value)
            return;

        m_blocksRaycasts = _value;
    }
    
    
} // namespace Maze
//////////////////////////////////////////
