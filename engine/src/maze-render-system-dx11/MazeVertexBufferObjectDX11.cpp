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
#include "MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/MazeVertexBufferObjectDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class VertexBufferObjectDX11
    //
    //////////////////////////////////////////
    VertexBufferObjectDX11::VertexBufferObjectDX11()
    {
    }

    //////////////////////////////////////////
    VertexBufferObjectDX11::~VertexBufferObjectDX11()
    {
        SafeReleaseDX11(m_buffer);
    }

    //////////////////////////////////////////
    VertexBufferObjectDX11Ptr VertexBufferObjectDX11::Create(
        RenderSystemDX11* _renderSystem,
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping)
    {
        VertexBufferObjectDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(VertexBufferObjectDX11, object, init(_renderSystem, _accessType, _singleMapping));
        return object;
    }

    //////////////////////////////////////////
    bool VertexBufferObjectDX11::init(
        RenderSystemDX11* _renderSystem,
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping)
    {
        if (!VertexBufferObject::init((RenderSystem*)_renderSystem, _accessType, _singleMapping))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemDX11* VertexBufferObjectDX11::getRenderSystemDX11Raw() const
    {
        return m_renderSystem->castRaw<RenderSystemDX11>();
    }

    //////////////////////////////////////////
    bool VertexBufferObjectDX11::isDynamicDX11() const
    {
        return m_accessType >= GPUByteBufferAccessType::DynamicDefault;
    }

    //////////////////////////////////////////
    bool VertexBufferObjectDX11::ensureBuffer(Size _bytes)
    {
        if (m_buffer && m_sizeBytes >= _bytes)
            return true;

        SafeReleaseDX11(m_buffer);
        m_sizeBytes = 0u;

        if (_bytes == 0u)
            return true;

        D3D11_BUFFER_DESC bufferDesc;
        memset(&bufferDesc, 0, sizeof(bufferDesc));
        bufferDesc.ByteWidth = (UINT)_bytes;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        if (isDynamicDX11())
        {
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        else
        {
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        }

        HRESULT hr = getRenderSystemDX11Raw()->getDevice()->CreateBuffer(&bufferDesc, nullptr, &m_buffer);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "Vertex buffer creation failed! hr=0x%08x", (U32)hr);

        m_sizeBytes = _bytes;
        return true;
    }

    //////////////////////////////////////////
    void VertexBufferObjectDX11::resize(Size _bytes)
    {
        ensureBuffer(_bytes);
    }

    //////////////////////////////////////////
    void VertexBufferObjectDX11::upload(
        void const* _data,
        Size _bytes)
    {
        if (!ensureBuffer(_bytes))
            return;

        if (!m_buffer || _bytes == 0u)
            return;

        ID3D11DeviceContext* deviceContext = getRenderSystemDX11Raw()->getDeviceContext();

        if (isDynamicDX11())
        {
            D3D11_MAPPED_SUBRESOURCE mapped;
            HRESULT hr = deviceContext->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
            MAZE_ERROR_RETURN_IF(FAILED(hr), "Vertex buffer map failed! hr=0x%08x", (U32)hr);

            memcpy(mapped.pData, _data, _bytes);
            deviceContext->Unmap(m_buffer, 0);
            return;
        }

        D3D11_BOX box;
        box.left = 0;
        box.right = (UINT)_bytes;
        box.top = 0;
        box.bottom = 1;
        box.front = 0;
        box.back = 1;

        deviceContext->UpdateSubresource(
            m_buffer,
            0,
            &box,
            _data,
            0,
            0);
    }


} // namespace Maze
//////////////////////////////////////////
