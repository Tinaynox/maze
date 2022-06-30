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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/systems/MazeUISystem2D.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-ui/ecs/components/MazeSystemTextEditBox2D.hpp"
#include "maze-ui/ecs/components/MazeSystemTextDropdown2D.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeTextRenderer2D.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class UISystem2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UISystem2D, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(UISystem2D);

    //////////////////////////////////////////
    UISystem2D::UISystem2D()
    {
    }

    //////////////////////////////////////////
    UISystem2D::~UISystem2D()
    {

    }

    //////////////////////////////////////////
    UISystem2DPtr UISystem2D::Create()
    {
        UISystem2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(UISystem2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool UISystem2D::init()
    {
        
        return true;
    }

    //////////////////////////////////////////
    void UISystem2D::processSystemAdded()
    {
        m_textRenderers2DSample = m_worldRaw->requestInclusiveSample<TextRenderer2D>();
    }

    //////////////////////////////////////////
    void UISystem2D::processUpdate(F32 _dt)
    {
        m_textRenderers2DSample->process(
            [](Entity* _entity, TextRenderer2D* _textRenderer2D)
            {
                if (_textRenderer2D->getTransform()->isSizeChanged())
                    _textRenderer2D->updateMeshData();
                else
                {
                    if (_textRenderer2D->getTransform()->isWorldTransformChanged())
                        _textRenderer2D->updateMeshRendererModelMatrices();

                    if (_textRenderer2D->getCanvasRenderer()->isAlphaDirty())
                        _textRenderer2D->updateMeshRendererColors();
                }
            });
    }
    
} // namespace Maze
//////////////////////////////////////////
