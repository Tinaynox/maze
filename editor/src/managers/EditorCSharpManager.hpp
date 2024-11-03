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
#if (!defined(_EditorCSharpManager_hpp_))
#define _EditorCSharpManager_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
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
#include "scenes/SceneWorkspace.hpp"
#include "editor/EditorSceneMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorCSharpManager);


    //////////////////////////////////////////
    // Class EditorCSharpManager
    //
    //////////////////////////////////////////
    class EditorCSharpManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ~EditorCSharpManager();

        //////////////////////////////////////////
        static void Initialize(EditorCSharpManagerPtr& _manager);
        

        //////////////////////////////////////////
        static inline EditorCSharpManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorCSharpManager& GetInstance() { return *s_instance; }



        //////////////////////////////////////////
        void updateCSharpFolder();

        //////////////////////////////////////////
        void updateLibraryFolder();

        //////////////////////////////////////////
        void generateCSharpAssembly();

        //////////////////////////////////////////
        void compileCSharpAssembly();

        //////////////////////////////////////////
        void loadCSharpAssembly();

    protected:

        //////////////////////////////////////////
        EditorCSharpManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);


    protected:
        static EditorCSharpManager* s_instance;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorCSharpManager_hpp_
//////////////////////////////////////////
