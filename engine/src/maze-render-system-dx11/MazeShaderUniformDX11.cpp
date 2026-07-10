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
#include "maze-render-system-dx11/MazeShaderUniformDX11.hpp"
#include "maze-render-system-dx11/MazeShaderDX11.hpp"
#include "maze-graphics/MazeTexture.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // HLSL constant buffer array elements are aligned to 16 bytes
    static U32 const c_registerSize = 16u;


    //////////////////////////////////////////
    // Class ShaderUniformDX11
    //
    //////////////////////////////////////////
    ShaderUniformDX11::ShaderUniformDX11()
    {

    }

    //////////////////////////////////////////
    ShaderUniformDX11::~ShaderUniformDX11()
    {
    }

    //////////////////////////////////////////
    ShaderUniformDX11Ptr ShaderUniformDX11::Create(
        ShaderPtr const& _shader,
        ShaderUniformType _type)
    {
        ShaderUniformDX11Ptr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(ShaderUniformDX11, object, init(_shader, _type));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderUniformDX11::init(ShaderPtr const& _shader, ShaderUniformType _type)
    {
        if (!ShaderUniform::init(_shader, _type))
            return false;

        return true;
    }

    //////////////////////////////////////////
    ShaderDX11* ShaderUniformDX11::getShaderDX11Raw() const
    {
        return m_shaderRaw->castRaw<ShaderDX11>();
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::processSimpleUniformChanged()
    {
        ShaderDX11* shader = getShaderDX11Raw();

        switch (getType())
        {
            case ShaderUniformType::UniformS32:
            {
                S32 value = getS32();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(value));
                break;
            }

            case ShaderUniformType::UniformF32:
            {
                F32 value = getF32();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(value));
                break;
            }

            case ShaderUniformType::UniformF64:
            {
                F32 value = (F32)getF64();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(value));
                break;
            }

            case ShaderUniformType::UniformBool:
            {
                S32 value = getBool() ? 1 : 0;
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(value));
                break;
            }

            case ShaderUniformType::UniformTexture2D:
            case ShaderUniformType::UniformTextureCube:
            case ShaderUniformType::UniformTexture2DArray:
            {
                // Will be bound in bindTextures
                break;
            }

            case ShaderUniformType::UniformVec2F32:
            {
                Vec4F const& value = getVecF();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(F32) * 2);
                break;
            }

            case ShaderUniformType::UniformVec3F32:
            {
                Vec4F const& value = getVecF();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(F32) * 3);
                break;
            }

            case ShaderUniformType::UniformVec4F32:
            case ShaderUniformType::UniformColorF128:
            {
                Vec4F const& value = getVecF();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(F32) * 4);
                break;
            }

            case ShaderUniformType::UniformVec2S32:
            {
                Vec4S const& value = getVecS();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(S32) * 2);
                break;
            }

            case ShaderUniformType::UniformVec3S32:
            {
                Vec4S const& value = getVecS();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(S32) * 3);
                break;
            }

            case ShaderUniformType::UniformVec4S32:
            {
                Vec4S const& value = getVecS();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(S32) * 4);
                break;
            }

            case ShaderUniformType::UniformVec2U32:
            {
                Vec4U const& value = getVecU();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(U32) * 2);
                break;
            }

            case ShaderUniformType::UniformVec3U32:
            {
                Vec4U const& value = getVecU();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(U32) * 3);
                break;
            }

            case ShaderUniformType::UniformVec4U32:
            {
                Vec4U const& value = getVecU();
                shader->writeUniformData(m_uniformData, (U8 const*)&value, sizeof(U32) * 4);
                break;
            }

            case ShaderUniformType::UniformVec2B:
            case ShaderUniformType::UniformVec3B:
            case ShaderUniformType::UniformVec4B:
            {
                Vec4B const& value = getVecB();
                S32 values[4] =
                {
                    value.x ? 1 : 0,
                    value.y ? 1 : 0,
                    value.z ? 1 : 0,
                    value.w ? 1 : 0
                };

                Size count = 4;
                if (getType() == ShaderUniformType::UniformVec2B)
                    count = 2;
                else
                if (getType() == ShaderUniformType::UniformVec3B)
                    count = 3;

                shader->writeUniformData(m_uniformData, (U8 const*)values, (U32)(sizeof(S32) * count));
                break;
            }

            case ShaderUniformType::UniformMat3F32:
            {
                upload(&getMat3F32(), 1);
                break;
            }

            case ShaderUniformType::UniformMat4F32:
            {
                upload(&getMat4F32(), 1);
                break;
            }

            case ShaderUniformType::UniformTMat:
            {
                upload(&getTMat(), 1);
                break;
            }

            default:
            {
                MAZE_ERROR("Unsupported ShaderUniformType: %s!", getType().toCString());
            }
        }
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(F32 const* _values, Size _count)
    {
        if (_count == 1)
        {
            getShaderDX11Raw()->writeUniformData(m_uniformData, (U8 const*)_values, sizeof(F32));
            return;
        }

        // Array elements are padded to 16 bytes
        Vector<U8> packed(_count * c_registerSize, 0u);
        for (Size i = 0; i < _count; ++i)
            memcpy(&packed[i * c_registerSize], &_values[i], sizeof(F32));

        getShaderDX11Raw()->writeUniformData(m_uniformData, &packed[0], (U32)packed.size());
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(Vec2F const* _vectors, Size _count)
    {
        if (_count == 1)
        {
            getShaderDX11Raw()->writeUniformData(m_uniformData, (U8 const*)_vectors, sizeof(Vec2F));
            return;
        }

        Vector<U8> packed(_count * c_registerSize, 0u);
        for (Size i = 0; i < _count; ++i)
            memcpy(&packed[i * c_registerSize], &_vectors[i], sizeof(Vec2F));

        getShaderDX11Raw()->writeUniformData(m_uniformData, &packed[0], (U32)packed.size());
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(Vec3F const* _vectors, Size _count)
    {
        if (_count == 1)
        {
            getShaderDX11Raw()->writeUniformData(m_uniformData, (U8 const*)_vectors, sizeof(Vec3F));
            return;
        }

        Vector<U8> packed(_count * c_registerSize, 0u);
        for (Size i = 0; i < _count; ++i)
            memcpy(&packed[i * c_registerSize], &_vectors[i], sizeof(Vec3F));

        getShaderDX11Raw()->writeUniformData(m_uniformData, &packed[0], (U32)packed.size());
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(Vec4F const* _vectors, Size _count)
    {
        getShaderDX11Raw()->writeUniformData(m_uniformData, (U8 const*)_vectors, (U32)(sizeof(Vec4F) * _count));
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(Mat3F const* _matrices, Size _count)
    {
        // Each 3-component column is padded to a 16-byte register
        Vector<U8> packed(_count * 3 * c_registerSize, 0u);
        for (Size i = 0; i < _count; ++i)
        {
            F32 const* planeMatrix = _matrices[i].getPlaneMatrix();
            for (Size c = 0; c < 3; ++c)
                memcpy(&packed[(i * 3 + c) * c_registerSize], planeMatrix + c * 3, sizeof(F32) * 3);
        }

        getShaderDX11Raw()->writeUniformData(m_uniformData, &packed[0], (U32)packed.size());
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(Mat4F const* _matrices, Size _count)
    {
        getShaderDX11Raw()->writeUniformData(m_uniformData, (U8 const*)_matrices, (U32)(sizeof(Mat4F) * _count));
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(TMat const* _matrices, Size _count)
    {
        // TMat is uploaded as float3[4 * n]: each Vec3F row padded to a 16-byte register
        Vector<U8> packed(_count * 4 * c_registerSize, 0u);
        for (Size i = 0; i < _count; ++i)
        {
            F32 const* planeMatrix = _matrices[i].getPlaneMatrix();
            for (Size r = 0; r < 4; ++r)
                memcpy(&packed[(i * 4 + r) * c_registerSize], planeMatrix + r * 3, sizeof(F32) * 3);
        }

        getShaderDX11Raw()->writeUniformData(m_uniformData, &packed[0], (U32)packed.size());
    }


} // namespace Maze
//////////////////////////////////////////
