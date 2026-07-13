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
#if (!defined(_MazeVertexBufferObjectDX11_hpp_))
#define _MazeVertexBufferObjectDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-graphics/MazeVertexBufferObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(VertexBufferObjectDX11);
    class RenderSystemDX11;


    //////////////////////////////////////////
    // Class VertexBufferObjectDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API VertexBufferObjectDX11
        : public VertexBufferObject
    {
    public:

        //////////////////////////////////////////
        virtual ~VertexBufferObjectDX11();

        //////////////////////////////////////////
        static VertexBufferObjectDX11Ptr Create(
            RenderSystemDX11* _renderSystem,
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping = false);


        //////////////////////////////////////////
        RenderSystemDX11* getRenderSystemDX11Raw() const;

        //////////////////////////////////////////
        inline ID3D11Buffer* getBufferDX11() const { return m_buffer; }

        //////////////////////////////////////////
        inline Size getSizeBytes() const { return m_sizeBytes; }


        //////////////////////////////////////////
        virtual void resize(Size _bytes) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(
            void const* _data,
            Size _bytes) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        VertexBufferObjectDX11();

        //////////////////////////////////////////
        using VertexBufferObject::init;

        //////////////////////////////////////////
        bool init(
            RenderSystemDX11* _renderSystem,
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping);

        //////////////////////////////////////////
        // Dynamic access types map to D3D11_USAGE_DYNAMIC + Map(WRITE_DISCARD)
        bool isDynamicDX11() const;

        //////////////////////////////////////////
        bool ensureBuffer(Size _bytes);

    protected:
        ID3D11Buffer* m_buffer = nullptr;
        Size m_sizeBytes = 0u;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVertexBufferObjectDX11_hpp_
//////////////////////////////////////////
