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
        getShaderDX11Raw()->writeUniformDataArray(m_uniformData, (U8 const*)_values, sizeof(F32), (U32)_count);
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(Vec2F const* _vectors, Size _count)
    {
        if (_count == 1)
        {
            getShaderDX11Raw()->writeUniformData(m_uniformData, (U8 const*)_vectors, sizeof(Vec2F));
            return;
        }

        getShaderDX11Raw()->writeUniformDataArray(m_uniformData, (U8 const*)_vectors, sizeof(Vec2F), (U32)_count);
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(Vec3F const* _vectors, Size _count)
    {
        if (_count == 1)
        {
            getShaderDX11Raw()->writeUniformData(m_uniformData, (U8 const*)_vectors, sizeof(Vec3F));
            return;
        }

        getShaderDX11Raw()->writeUniformDataArray(m_uniformData, (U8 const*)_vectors, sizeof(Vec3F), (U32)_count);
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(Vec4F const* _vectors, Size _count)
    {
        getShaderDX11Raw()->writeUniformData(m_uniformData, (U8 const*)_vectors, (U32)(sizeof(Vec4F) * _count));
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(Mat3F const* _matrices, Size _count)
    {
        // Mat3F is 9 contiguous floats - uploaded as float3[3 * n],
        // each 3-component column padded to a 16-byte register
        getShaderDX11Raw()->writeUniformDataArray(m_uniformData, (U8 const*)_matrices, sizeof(F32) * 3, (U32)(_count * 3));
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(Mat4F const* _matrices, Size _count)
    {
        getShaderDX11Raw()->writeUniformData(m_uniformData, (U8 const*)_matrices, (U32)(sizeof(Mat4F) * _count));
    }

    //////////////////////////////////////////
    void ShaderUniformDX11::upload(TMat const* _matrices, Size _count)
    {
        // TMat is 12 contiguous floats - uploaded as float3[4 * n],
        // each Vec3F row padded to a 16-byte register
        getShaderDX11Raw()->writeUniformDataArray(m_uniformData, (U8 const*)_matrices, sizeof(F32) * 3, (U32)(_count * 4));
    }


} // namespace Maze
//////////////////////////////////////////
