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
#pragma once
#if (!defined(_MazeEditorToolsHelper_hpp_))
#define _MazeEditorToolsHelper_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/utils/MazeDelegate.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    namespace EditorToolsHelper
    {
        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API Set<ClassUID> CollectIntersectingComponentUIDs(Set<EntityPtr> const& _entities);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API Set<MetaClass*> CollectIntersectingComponentMetaClasses(Set<EntityPtr> const& _entities);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API String BuildPropertyName(CString _propertyName, CString _label = nullptr);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API String BuildComponentName(CString _componentName, CString _label = nullptr);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API EntityPtr CreateEntity(CString _entityName, ECSScene* _scene = nullptr);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API EntityPtr CreateEntity2D(CString _entityName, ECSScene* _scene = nullptr);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API EntityPtr CreateEntity3D(CString _entityName, ECSScene* _scene = nullptr);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API String BuildNewAssetFileName(String const& _fullPath);


    } // namespace EditorToolsHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorToolsHelper_hpp_
//////////////////////////////////////////
