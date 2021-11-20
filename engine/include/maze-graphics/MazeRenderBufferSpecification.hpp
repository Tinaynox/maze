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
#if (!defined(_MazeRenderBufferSpecification_hpp_))
#define _MazeRenderBufferSpecification_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/hash/MazeHashSuperFast.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderBuffer);
    MAZE_USING_SHARED_PTR(RenderBufferUniform);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Texture2D);


    //////////////////////////////////////////
    static Size const c_renderBufferColorTexturesMax = 4;


    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderBufferSpecification
    {
        //////////////////////////////////////////
        struct MAZE_GRAPHICS_API TextureFormat
        {
            TextureFormat(
                PixelFormat::Enum _pixelFormat = PixelFormat::None,
                S32 _samples = 0)
                : pixelFormat(_pixelFormat)
                , samples(_samples)
            {}

            //////////////////////////////////////////
            inline bool operator==(TextureFormat const& _other) const
            {
                return pixelFormat == _other.pixelFormat && samples == _other.samples;
            }

            //////////////////////////////////////////
            inline bool operator!=(TextureFormat const& _other) const
            {
                return !this->operator==(_other);
            }

            PixelFormat::Enum pixelFormat = PixelFormat::None;
            S32 samples = 0;
        };

        //////////////////////////////////////////
        RenderBufferSpecification()
        { }

        //////////////////////////////////////////
        RenderBufferSpecification(
            Vec2DU const& _size,
            TextureFormat _colorTextureFormat)
            : size(_size)
            , colorTextureFormats{_colorTextureFormat, PixelFormat::None}
            , _hash(calculateHash())
        {
        }

        //////////////////////////////////////////
        RenderBufferSpecification(
            Vec2DU const& _size,
            TextureFormat _colorTextureFormat,
            TextureFormat _depthTextureFormat)
            : size(_size)
            , colorTextureFormats{_colorTextureFormat, PixelFormat::None}
            , depthTextureFormat(_depthTextureFormat)
            , _hash(calculateHash())
        {
        }

        //////////////////////////////////////////
        inline U32 calculateHash()
        {
            U32 hash = 0;
            hash = CalculateSuperFastHash((Char const*)&size, sizeof(size));
            hash = CalculateSuperFastHash((Char const*)&colorTextureFormats, sizeof(colorTextureFormats), hash);
            hash = CalculateSuperFastHash((Char const*)&depthTextureFormat, sizeof(depthTextureFormat), hash);
            hash = CalculateSuperFastHash((Char const*)&stencilTextureFormat, sizeof(stencilTextureFormat), hash);

            return hash;
        }

        //////////////////////////////////////////
        inline bool operator==(RenderBufferSpecification const& _other) const
        {
            if (_hash != _other._hash)
                return false;

            for (S32 i = 0; i < (S32)c_renderBufferColorTexturesMax; ++i)
                if (colorTextureFormats[i] != _other.colorTextureFormats[i])
                    return false;

            return (depthTextureFormat == _other.depthTextureFormat) &&
                   (stencilTextureFormat == _other.stencilTextureFormat);
        }

        Vec2DU size;
        TextureFormat colorTextureFormats[c_renderBufferColorTexturesMax] = { {PixelFormat::None, 0} };
        TextureFormat depthTextureFormat = { PixelFormat::None, 0 };
        TextureFormat stencilTextureFormat = { PixelFormat::None, 0 };

        U32 _hash;
    };

    //////////////////////////////////////////
    inline bool operator<(RenderBufferSpecification const& l, RenderBufferSpecification const& r)
    {
        return (l._hash < r._hash);
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderBufferSpecification_hpp_
//////////////////////////////////////////
