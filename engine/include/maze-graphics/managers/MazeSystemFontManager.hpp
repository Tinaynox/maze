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
#if (!defined(_MazeSystemFontManager_hpp_))
#define _MazeSystemFontManager_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazeSystemFont.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SystemFontManager);
    MAZE_USING_SHARED_PTR(RenderSystem);


    //////////////////////////////////////////
    // Class SystemFontManager
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API SystemFontManager 
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~SystemFontManager();

        //////////////////////////////////////////
        static void Initialize(SystemFontManagerPtr& _manager, RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        static SystemFontManagerPtr const& GetCurrentInstancePtr();


        //////////////////////////////////////////
        SystemFontPtr const& getSystemFont() const { return m_systemFont; }

        //////////////////////////////////////////
        SystemFontPtr const& getSystemFontOutlined() const { return m_systemFontOutlined; }


        //////////////////////////////////////////
        SystemFontPtr createSystemFontOutlined(
            String const& _name,
            ColorU32 const& _outlineColor);

    protected:

        //////////////////////////////////////////
        SystemFontManager();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        void createSystemFont();

        //////////////////////////////////////////
        void notifySpecialMaterialsCreated();

        //////////////////////////////////////////
        void finalizeSystemFont(SystemFontPtr const& _font);

    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw = nullptr;

        SystemFontPtr m_systemFont;
        SystemFontPtr m_systemFontOutlined;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemFontManager_hpp_
//////////////////////////////////////////
