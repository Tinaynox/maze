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
#if (!defined(_MazeGlobalShaderUniform_hpp_))
#define _MazeGlobalShaderUniform_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeRenderDrawTopology.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeManagedSharedObject.hpp"
#include "maze-core/utils/MazeIndexedResource.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(GlobalShaderUniform);
    

    //////////////////////////////////////////
    // Class GlobalShaderUniform
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API GlobalShaderUniform
        : public IndexedResource<GlobalShaderUniform>
    {
    public:

        //////////////////////////////////////////
        ~GlobalShaderUniform();

        //////////////////////////////////////////
        static GlobalShaderUniformPtr Create(
            RenderSystem* _renderSystem,
            HashedString const& _name);


        //////////////////////////////////////////
        void setName(HashedString const& _name) { m_name = _name; }

        //////////////////////////////////////////
        HashedString const& getName() const { return m_name; }


        //////////////////////////////////////////
        ShaderUniformVariant const& getVariant() const { return m_variant; }

        //////////////////////////////////////////
        void setVariant(ShaderUniformVariant const& _variant);

        //////////////////////////////////////////
        void setVariantImmediate(ShaderUniformVariant const& _variant);



        //////////////////////////////////////////
        void setValue(S32 _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(F32 _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(F64 _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(bool _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Texture2D* _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Texture2DPtr const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(TextureCube* _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(TextureCubePtr const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec2F const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec3F const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec4F const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec2S const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec3S const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec4S const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec2U const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec3U const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec4U const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec2B const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec3B const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec4B const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Mat3F const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Mat4F const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(TMat const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(ColorF128 const& _value) { setVariant(ShaderUniformVariant(m_renderSystem, _value)); }

        //////////////////////////////////////////
        void setValue(Vec3F const* _vec, U32 _size);

        //////////////////////////////////////////
        void setValue(Vec4F const* _vec, U32 _size);

    protected:

        //////////////////////////////////////////
        GlobalShaderUniform();

        //////////////////////////////////////////
        bool init(
            RenderSystem* _renderSystem,
            HashedString const& _name);


    protected:
        RenderSystem* m_renderSystem = nullptr;

        HashedString m_name;
        ShaderUniformVariant m_variant;
        ByteBuffer m_heapData;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGlobalShaderUniform_hpp_
//////////////////////////////////////////
