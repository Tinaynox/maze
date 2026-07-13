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
#include "maze-render-system-dx11/MazeVertexArrayObjectDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeShaderDX11.hpp"
#include "maze-render-system-dx11/MazeHelpersDX11.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class VertexArrayObjectDX11
    //
    //////////////////////////////////////////
    VertexArrayObjectDX11::VertexArrayObjectDX11()
    {
    }

    //////////////////////////////////////////
    VertexArrayObjectDX11::~VertexArrayObjectDX11()
    {
        SafeReleaseDX11(m_indexBuffer);
    }

    //////////////////////////////////////////
    VertexArrayObjectDX11Ptr VertexArrayObjectDX11::Create(RenderSystemDX11* _renderSystem)
    {
        VertexArrayObjectDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(VertexArrayObjectDX11, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool VertexArrayObjectDX11::init(RenderSystemDX11* _renderSystem)
    {
        if (!VertexArrayObject::init((RenderSystem*)_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemDX11* VertexArrayObjectDX11::getRenderSystemDX11Raw() const
    {
        return m_renderSystem->castRaw<RenderSystemDX11>();
    }

    //////////////////////////////////////////
    void VertexArrayObjectDX11::setIndices(
        U8 const* _indicesData,
        VertexAttributeType _indicesType,
        Size _indicesCount)
    {
        MAZE_ERROR_RETURN_IF(
            _indicesType != VertexAttributeType::U16 && _indicesType != VertexAttributeType::U32,
            "Unsupported indices type: %d!",
            (S32)_indicesType);

        m_indicesType = _indicesType;
        m_indicesCount = _indicesCount;

        Size bytes = GetVertexAttributeTypeSize(_indicesType) * _indicesCount;
        if (bytes == 0u)
        {
            SafeReleaseDX11(m_indexBuffer);
            m_indexBufferSizeBytes = 0u;
            return;
        }

        ID3D11Device* device = getRenderSystemDX11Raw()->getDevice();
        ID3D11DeviceContext* deviceContext = getRenderSystemDX11Raw()->getDeviceContext();

        if (!m_indexBuffer || m_indexBufferSizeBytes < bytes)
        {
            SafeReleaseDX11(m_indexBuffer);
            m_indexBufferSizeBytes = 0u;

            D3D11_BUFFER_DESC bufferDesc;
            memset(&bufferDesc, 0, sizeof(bufferDesc));
            bufferDesc.ByteWidth = (UINT)bytes;
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

            D3D11_SUBRESOURCE_DATA initialData;
            memset(&initialData, 0, sizeof(initialData));
            initialData.pSysMem = _indicesData;

            HRESULT hr = device->CreateBuffer(&bufferDesc, &initialData, &m_indexBuffer);
            MAZE_ERROR_RETURN_IF(FAILED(hr), "Index buffer creation failed! hr=0x%08x", (U32)hr);

            m_indexBufferSizeBytes = bytes;
        }
        else
        {
            D3D11_BOX box;
            box.left = 0;
            box.right = (UINT)bytes;
            box.top = 0;
            box.bottom = 1;
            box.front = 0;
            box.back = 1;

            deviceContext->UpdateSubresource(m_indexBuffer, 0, &box, _indicesData, 0, 0);
        }
    }

    //////////////////////////////////////////
    void VertexArrayObjectDX11::setVerticesData(
        U8 const* _data,
        VertexAttributeDescription _description,
        Size _verticesCount)
    {
        VertexArrayObjectDX11AttributeData& attribute = m_attributes[(Size)_description.semantic];

        if (!attribute.vbo)
        {
            attribute.vbo = VertexBufferObjectDX11::Create(
                getRenderSystemDX11Raw(),
                GPUByteBufferAccessType::DynamicDefault,
                false);
            MAZE_ERROR_RETURN_IF(!attribute.vbo, "VBO cannot be created!");
        }

        attribute.used = true;
        attribute.description = _description;
        attribute.verticesCount = _verticesCount;

        Size bytesPerType = GetVertexAttributeTypeSize(_description.type);
        Size bytesPerVertexData = bytesPerType * _description.count;
        Size requiredBytes = bytesPerVertexData * _verticesCount;

        attribute.vbo->upload(_data, requiredBytes);

        updateLayoutHash();
    }

    //////////////////////////////////////////
    void VertexArrayObjectDX11::updateLayoutHash()
    {
        // FNV-1a over the attributes layout
        U32 hash = 2166136261u;

        auto combine =
            [&hash](U32 _value)
            {
                hash ^= _value;
                hash *= 16777619u;
            };

        for (Size i = 0; i < (Size)VertexAttributeSemantic::MAX; ++i)
        {
            VertexArrayObjectDX11AttributeData const& attribute = m_attributes[i];
            if (!attribute.used)
                continue;

            combine((U32)i + 1u);
            combine((U32)attribute.description.type);
            combine((U32)attribute.description.count);
            combine(attribute.description.normalized ? 1u : 0u);
        }

        m_layoutHash = hash;
    }

    //////////////////////////////////////////
    void VertexArrayObjectDX11::bind(ShaderDX11* _shader)
    {
        RenderSystemDX11* renderSystem = getRenderSystemDX11Raw();
        ID3D11DeviceContext* deviceContext = renderSystem->getDeviceContext();

        // Vertex buffers - one IA slot per semantic
        for (Size i = 0; i < (Size)VertexAttributeSemantic::MAX; ++i)
        {
            VertexArrayObjectDX11AttributeData const& attribute = m_attributes[i];
            if (!attribute.used || !attribute.vbo)
                continue;

            ID3D11Buffer* buffer = attribute.vbo->getBufferDX11();
            UINT stride = (UINT)attribute.description.stride;
            UINT offset = 0;
            deviceContext->IASetVertexBuffers((UINT)i, 1, &buffer, &stride, &offset);
        }

        // Default-value buffer feeds attributes the VAO doesn't provide (stride 0 - constant data)
        {
            ID3D11Buffer* zeroBuffer = renderSystem->getZeroVertexBuffer();
            UINT stride = 0;
            UINT offset = 0;
            deviceContext->IASetVertexBuffers((UINT)c_zeroBufferSlot, 1, &zeroBuffer, &stride, &offset);
        }

        // Index buffer
        DXGI_FORMAT indexFormat = (m_indicesType == VertexAttributeType::U32) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
        deviceContext->IASetIndexBuffer(m_indexBuffer, indexFormat, 0);

        // Topology
        deviceContext->IASetPrimitiveTopology(GetRenderDrawTopologyDX11(m_renderDrawTopology));

        // Input layout
        ID3D11InputLayout* inputLayout = renderSystem->ensureInputLayout(_shader, this);
        deviceContext->IASetInputLayout(inputLayout);
    }

    //////////////////////////////////////////
    SubMeshPtr VertexArrayObjectDX11::readAsSubMesh() const
    {
        MAZE_NOT_IMPLEMENTED;
        return nullptr;
    }

#if MAZE_DEBUG
    //////////////////////////////////////////
    void VertexArrayObjectDX11::debug()
    {
    }
#endif


} // namespace Maze
//////////////////////////////////////////
