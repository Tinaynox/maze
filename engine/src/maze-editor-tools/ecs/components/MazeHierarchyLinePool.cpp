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
#include "maze-editor-tools/ecs/components/MazeHierarchyLinePool.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLine.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(ComponentPoolObject<HierarchyLine>, Component);


    //////////////////////////////////////////
    // Class HierarchyLinePool
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(HierarchyLinePool, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(HierarchyLinePool);

    //////////////////////////////////////////
    HierarchyLinePool::HierarchyLinePool()
    {
    }

    //////////////////////////////////////////
    HierarchyLinePool::~HierarchyLinePool()
    {

    }

    //////////////////////////////////////////
    HierarchyLinePoolPtr HierarchyLinePool::Create()
    {
        HierarchyLinePoolPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(HierarchyLinePool, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool HierarchyLinePool::init()
    {
        for (HierarchyLineType type = HierarchyLineType(0); type < HierarchyLineType::MAX; type = HierarchyLineType((S32)type + 1))
        {
            m_pools[(Size)type] = SharedPtr<SharedObjectPool<HierarchyLine>>(
                new SharedObjectPool<HierarchyLine>(
                    [this, type]() -> HierarchyLinePtr
                    {
                        MAZE_PROFILE_EVENT("HierarchyLinePool Create");

                        EntityPtr entity = this->getEntityRaw()->getEcsScene()->createEntity();

                        entity->ensureComponent<ComponentPoolObject<HierarchyLine>>(m_pools[(Size)type]);

                        HierarchyLinePtr hierarchyLine = entity->createComponent<HierarchyLine>(type);
                        
                        return hierarchyLine;
                    },
                    [](HierarchyLinePtr const& _hierarchyLine)
                    {
                        MAZE_PROFILE_EVENT("HierarchyLinePool Get");

                        _hierarchyLine->getEntityRaw()->setActiveSelf(true);
                        _hierarchyLine->prepare();
                    },
                    [](HierarchyLinePtr const& _hierarchyLine)
                    {
                        MAZE_PROFILE_EVENT("HierarchyLinePool Release");

                        _hierarchyLine->getEntityRaw()->setActiveSelf(false);

                        if (_hierarchyLine->getTransform() && _hierarchyLine->getTransform()->getParent())
                        {
                            Entity* parentEntity = _hierarchyLine->getTransform()->getParent()->getEntityRaw();
                            Layout2DPtr layout = parentEntity->getComponentInheritedFrom<Layout2D>();
                            if (layout)
                                layout->dirty();
                        }

                        _hierarchyLine->getTransform()->resetParent();
                        _hierarchyLine->clearChildren();
                        _hierarchyLine->processReleased();
                        _hierarchyLine->setUserData(nullptr);
                    },
                    [](HierarchyLinePtr const& _hierarchyLine)
                    {
                        MAZE_PROFILE_EVENT("HierarchyLinePool Destroy");

                        if (_hierarchyLine->getEntityRaw())
                            _hierarchyLine->getEntityRaw()->removeFromEcsWorld();
                    }));
        }

        return true;
    }

    //////////////////////////////////////////
    HierarchyLinePtr HierarchyLinePool::createHierarchyLine(HierarchyLineType _type)
    {
        return m_pools[(Size)_type]->fetch();
    }

    //////////////////////////////////////////
    void HierarchyLinePool::releaseHierarchyLine(HierarchyLinePtr const& _hierarchyLine)
    {
        m_pools[(Size)_hierarchyLine->getType()]->release(_hierarchyLine);
    }
    
    
} // namespace Maze
//////////////////////////////////////////
