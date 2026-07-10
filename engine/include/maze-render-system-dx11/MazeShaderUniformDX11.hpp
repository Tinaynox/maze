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
#if (!defined(_MazeShaderUniformDX11_hpp_))
#define _MazeShaderUniformDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/MazeShaderDX11.hpp"
#include "maze-graphics/MazeShaderUniform.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(ShaderUniformDX11);


    //////////////////////////////////////////
    // Class ShaderUniformDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API ShaderUniformDX11
        : public ShaderUniform
    {
    public:

        //////////////////////////////////////////
        virtual ~ShaderUniformDX11();

        //////////////////////////////////////////
        static ShaderUniformDX11Ptr Create(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None);


        //////////////////////////////////////////
        using ShaderUniform::setName;

        //////////////////////////////////////////
        inline void setUniformData(ShaderDX11UniformData const& _data) { m_uniformData = _data; }

        //////////////////////////////////////////
        inline ShaderDX11UniformData const& getUniformData() const { return m_uniformData; }


        //////////////////////////////////////////
        virtual void processSimpleUniformChanged() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void upload(F32 const* _values, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Vec2F const* _vectors, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Vec3F const* _vectors, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Vec4F const* _vectors, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Mat3F const* _matrices, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Mat4F const* _matrices, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(TMat const* _matrices, Size _count) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ShaderUniformDX11();

        //////////////////////////////////////////
        virtual bool init(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        ShaderDX11* getShaderDX11Raw() const;

    protected:
        ShaderDX11UniformData m_uniformData;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderUniformDX11_hpp_
//////////////////////////////////////////
