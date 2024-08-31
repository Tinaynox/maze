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
#if (!defined(_EditorHelper_hpp_))
#define _EditorHelper_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
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
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-editor-tools/managers/MazeEditorActionManager.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionEntityAdd.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Transform3D);
    

    //////////////////////////////////////////
    namespace EditorHelper
    {
        //////////////////////////////////////////
        bool IsValidSceneMode();

        //////////////////////////////////////////
        EntityPtr CreateNewPrefab2D(
            Transform2DPtr const& _parent = nullptr,
            bool _select = true);

        //////////////////////////////////////////
        EntityPtr CreateNewPrefab3D(
            Transform3DPtr const& _parent = nullptr,
            bool _select = true);

        //////////////////////////////////////////
        EntityPtr CreateEntity(CString _entityName);

        //////////////////////////////////////////
        EntityPtr CreateEntity2D(CString _entityName);

        //////////////////////////////////////////
        EntityPtr CreateEntity3D(CString _entityName);

        //////////////////////////////////////////
        EntityPtr CreateBuiltinMesh(
            BuiltinRenderMeshType _meshType,
            Transform3DPtr const& _parent = nullptr,
            bool _select = true);

        //////////////////////////////////////////
        EntityPtr CreateDirectionalLight(
            CString _entityName,
            Transform3DPtr const& _parent = nullptr,
            bool _select = true);

        //////////////////////////////////////////
        EntityPtr CreateNewParticleSystem3D(
            CString _entityName,
            Transform3DPtr const& _parent = nullptr,
            bool _select = true);

        //////////////////////////////////////////
        bool SaveValidate();

        //////////////////////////////////////////
        void Save();

        //////////////////////////////////////////
        bool SaveAsValidate();

        //////////////////////////////////////////
        void SaveAs();

        //////////////////////////////////////////
        bool LoadValidate();

        //////////////////////////////////////////
        void Load();

        //////////////////////////////////////////
        bool ClearValidate();

        //////////////////////////////////////////
        void Clear();
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorHelper_hpp_
//////////////////////////////////////////
