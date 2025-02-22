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
#if (!defined(_MazeTexture_hpp_))
#define _MazeTexture_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeManagedSharedObject.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/MazeObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Texture);
    

    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_6_API(MAZE_GRAPHICS_API, TextureFilter,
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear);

    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_4_API(MAZE_GRAPHICS_API, TextureWrap,
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder);

    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_5_API(MAZE_GRAPHICS_API, TextureType,
        SingleDimensional,
        TwoDimensional,
        TwoDimensionalMultisample,
        ThreeDimensional,
        Cube);


    //////////////////////////////////////////
    // Class Texture
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Texture
        : public ManagedSharedObject<Texture>
        , public Object
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Texture, Object);

    public:

        //////////////////////////////////////////
        virtual ~Texture();

        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(HashedString const& _name) { m_name = _name; }

        //////////////////////////////////////////
        inline void setName(CString _name) { m_name = _name; }

        //////////////////////////////////////////
        inline void setName(String const& _name) { m_name = HashedString(_name); }

        //////////////////////////////////////////
        inline TextureType getType() const { return m_type; }


        //////////////////////////////////////////
        inline RenderSystem* getRenderSystem() const { return m_renderSystem; }

    public:

        //////////////////////////////////////////
        static void IterateTextures(std::function<bool(Texture*)> _cb);

    protected:

        //////////////////////////////////////////
        Texture();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem);
    
    protected:
        RenderSystem* m_renderSystem;

        HashedString m_name;

        TextureType m_type = TextureType::None;

    protected:
        static Texture* s_instancesList;
        Texture* m_instancesListNext = nullptr;
        Texture* m_instancesListPrev = nullptr;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexture_hpp_
//////////////////////////////////////////
