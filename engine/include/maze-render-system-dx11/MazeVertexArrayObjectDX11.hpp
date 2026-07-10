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
#if (!defined(_MazeVertexArrayObjectDX11_hpp_))
#define _MazeVertexArrayObjectDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/MazeVertexBufferObjectDX11.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeVertex.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(VertexArrayObjectDX11);
    class RenderSystemDX11;
    class ShaderDX11;


    //////////////////////////////////////////
    // Struct VertexArrayObjectDX11AttributeData
    //
    //////////////////////////////////////////
    struct MAZE_RENDER_SYSTEM_DX11_API VertexArrayObjectDX11AttributeData
    {
        bool used = false;
        VertexAttributeDescription description;
        VertexBufferObjectDX11Ptr vbo;
        Size verticesCount = 0u;
    };


    //////////////////////////////////////////
    // Class VertexArrayObjectDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API VertexArrayObjectDX11
        : public VertexArrayObject
    {
    public:

        //////////////////////////////////////////
        // IA slot used to feed missing attributes with zeroes (stride 0)
        static Size const c_zeroBufferSlot = (Size)VertexAttributeSemantic::MAX;

    public:

        //////////////////////////////////////////
        virtual ~VertexArrayObjectDX11();

        //////////////////////////////////////////
        static VertexArrayObjectDX11Ptr Create(RenderSystemDX11* _renderSystem);


        //////////////////////////////////////////
        RenderSystemDX11* getRenderSystemDX11Raw() const;


        //////////////////////////////////////////
        virtual void setIndices(
            U8 const* _indicesData,
            VertexAttributeType _indicesType,
            Size _indicesCount) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setVerticesData(
            U8 const* _data,
            VertexAttributeDescription _description,
            Size _verticesCount) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual SubMeshPtr readAsSubMesh() const MAZE_OVERRIDE;

#if MAZE_DEBUG
        //////////////////////////////////////////
        virtual void debug() MAZE_OVERRIDE;
#endif


        //////////////////////////////////////////
        inline VertexArrayObjectDX11AttributeData const& getAttribute(VertexAttributeSemantic _semantic) const
        {
            return m_attributes[(Size)_semantic];
        }

        //////////////////////////////////////////
        inline U32 getLayoutHash() const { return m_layoutHash; }

        //////////////////////////////////////////
        inline ID3D11Buffer* getIndexBuffer() const { return m_indexBuffer; }

        //////////////////////////////////////////
        // Binds vertex buffers, index buffer, topology and the input layout for the given shader
        void bind(ShaderDX11* _shader);

    protected:

        //////////////////////////////////////////
        VertexArrayObjectDX11();

        //////////////////////////////////////////
        using VertexArrayObject::init;

        //////////////////////////////////////////
        bool init(RenderSystemDX11* _renderSystem);

        //////////////////////////////////////////
        void updateLayoutHash();

    protected:
        VertexArrayObjectDX11AttributeData m_attributes[(Size)VertexAttributeSemantic::MAX];

        ID3D11Buffer* m_indexBuffer = nullptr;
        Size m_indexBufferSizeBytes = 0u;

        U32 m_layoutHash = 0u;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVertexArrayObjectDX11_hpp_
//////////////////////////////////////////
