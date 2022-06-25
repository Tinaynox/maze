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
#if (!defined(_MazeFontMaterialManager_hpp_))
#define _MazeFontMaterialManager_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(FontMaterialManager);
    MAZE_USING_SHARED_PTR(FontMaterial);
    

    //////////////////////////////////////////
    // Class FontMaterialManager
    //
    //////////////////////////////////////////
    class MAZE_UI_API FontMaterialManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~FontMaterialManager();

        //////////////////////////////////////////
        static void Initialize(FontMaterialManagerPtr& _manager);


        //////////////////////////////////////////
        static inline FontMaterialManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline FontMaterialManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        FontMaterialPtr const& getFontMaterial(HashedCString _assetFileName);

        //////////////////////////////////////////
        inline FontMaterialPtr const& getFontMaterial(String const& _assetFileName) { return getFontMaterial(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        inline FontMaterialPtr const& getFontMaterial(CString _assetFileName) { return getFontMaterial(MAZE_HASHED_CSTRING(_assetFileName)); }

        //////////////////////////////////////////
        FontMaterialPtr const& addFontMaterial(FontMaterialPtr const& _trueTypeFont);


        //////////////////////////////////////////
        String const& getFontMaterialName(FontMaterial const* _font);

    protected:

        //////////////////////////////////////////
        FontMaterialManager();

        //////////////////////////////////////////
        bool init();


    protected:
        static FontMaterialManager* s_instance;

        StringKeyMap<FontMaterialPtr> m_fontMaterialsByName;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFontMaterialManager_hpp_
//////////////////////////////////////////
