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
#include "ecs/components/EditorMainCanvasController.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLine.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLinePool.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorPrefabManager.hpp"
#include "ecs/components/tools/ParticleEffectInfo.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorMainCanvasController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorMainCanvasController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EditorMainCanvasController);

    //////////////////////////////////////////
    EditorMainCanvasController::EditorMainCanvasController()
        : m_canvas(nullptr)
    {
    }

    //////////////////////////////////////////
    EditorMainCanvasController::~EditorMainCanvasController()
    {
        
    }

    //////////////////////////////////////////
    EditorMainCanvasControllerPtr EditorMainCanvasController::Create(Canvas* _canvas)
    {
        EditorMainCanvasControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorMainCanvasController, object, init(_canvas));
        return object;
    }

    //////////////////////////////////////////
    bool EditorMainCanvasController::init(Canvas* _canvas)
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        m_canvas = _canvas;

        return true;
    }

    //////////////////////////////////////////
    void EditorMainCanvasController::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void EditorMainCanvasController::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();


        EntityPtr particleEffectInfoEntity = getEntityRaw()->getECSScene()->createEntity();
        ParticleEffectInfoPtr particleEffectInfo = particleEffectInfoEntity->createComponent<ParticleEffectInfo>();
        particleEffectInfo->getTransform()->setParent(m_canvas->getTransform());
    }

} // namespace Maze
//////////////////////////////////////////
