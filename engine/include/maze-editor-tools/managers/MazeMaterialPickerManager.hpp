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
#if (!defined(_MazeMaterialPickerManager_hpp_))
#define _MazeMaterialPickerManager_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MaterialPickerManager);
    

    //////////////////////////////////////////
    // Class MaterialPickerManager
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API MaterialPickerManager
        : public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        using MaterialPickerCallback = std::function<void(MaterialPtr const& _material)>;

    public:

        //////////////////////////////////////////
        virtual ~MaterialPickerManager();

        //////////////////////////////////////////
        static void Initialize(MaterialPickerManagerPtr& _manager);


        //////////////////////////////////////////
        static inline MaterialPickerManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline MaterialPickerManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void openMaterialPicker(
            MaterialPickerCallback _callback,
            MaterialPtr const& _material = MaterialPtr());


        //////////////////////////////////////////
        void setMaterial(MaterialPtr const& _material);

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const { return m_material; }



        //////////////////////////////////////////
        bool isMaterialPickerEditorWindowOpened();

        //////////////////////////////////////////
        bool isMaterialPickerEditorWindowFocused();

    public:

        //////////////////////////////////////////
        MultiDelegate<MaterialPtr const&> eventMaterialChanged;

    protected:

        //////////////////////////////////////////
        MaterialPickerManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void openMaterialPickerEditorWindow();

        //////////////////////////////////////////
        void closeMaterialPickerEditorWindow();

        //////////////////////////////////////////
        void openMaterialPickerEditorScene();

        //////////////////////////////////////////
        void closeMaterialPickerEditorScene();

        //////////////////////////////////////////
        void notifyWindowWillClose(Window* _window);

        //////////////////////////////////////////
        void notifyWindowFocusChanged(Window* _window);

    protected:
        static MaterialPickerManager* s_instance;

        RenderWindowPtr m_materialPickerRenderWindow;

        MaterialPickerCallback m_callback;

        MaterialPtr m_material;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMaterialPickerManager_hpp_
//////////////////////////////////////////
