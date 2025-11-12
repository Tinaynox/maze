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
#if (!defined(_MazeShaderUniform_hpp_))
#define _MazeShaderUniform_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeManagedSharedPtr.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(ShaderUniform);
    

    //////////////////////////////////////////
    // Class ShaderUniform
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API ShaderUniform
        : public SharedObject<ShaderUniform>
    {
    private:

        //////////////////////////////////////////
        friend class Shader;

    public:

        //////////////////////////////////////////
        virtual ~ShaderUniform();

        //////////////////////////////////////////
        static ShaderUniformPtr Create(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None);


        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_value.getName(); }

        //////////////////////////////////////////
        inline ShaderUniformVariant const& getValue() const { return m_value; }

        //////////////////////////////////////////
        inline ShaderUniformType const& getType() const { return m_value.getType(); }

        //////////////////////////////////////////
        inline void const* getPtr() const { return m_value.getPtr(); }

        //////////////////////////////////////////
        inline U32 getCount() const { return m_value.getCount(); }

        //////////////////////////////////////////
        inline S32 getS32() const { return m_value.getS32(); }

        //////////////////////////////////////////
        inline F32 getF32() const { return m_value.getF32(); }

        //////////////////////////////////////////
        inline F64 getF64() const { return m_value.getF64(); }

        //////////////////////////////////////////
        inline bool getBool() const { return m_value.getBool(); }

        //////////////////////////////////////////
        inline TexturePtr const& getTexture() const { return m_value.getTexture(); }

        //////////////////////////////////////////
        inline Texture2DPtr getTexture2D() const { return Maze::static_pointer_cast<Texture2D>(m_value.getTexture()); }

        //////////////////////////////////////////
        inline TextureCubePtr getTextureCube() const { return Maze::static_pointer_cast<TextureCube>(m_value.getTexture()); }

        //////////////////////////////////////////
        inline Vec4F const& getVecF() const { return m_value.getVecF(); }

        //////////////////////////////////////////
        inline Vec2F getVec2F32() const { return m_value.getVec2F32(); }

        //////////////////////////////////////////
        inline Vec3F getVec3F32() const { return m_value.getVec3F32(); }

        //////////////////////////////////////////
        inline Vec4F getVec4F32() const { return m_value.getVec4F32(); }


        //////////////////////////////////////////
        inline Vec4S const& getVecS() const { return m_value.getVecS(); }

        //////////////////////////////////////////
        inline Vec2S getVec2S32() const { return m_value.getVec2S32(); }

        //////////////////////////////////////////
        inline Vec3S getVec3S32() const { return m_value.getVec3S32(); }

        //////////////////////////////////////////
        inline Vec4S getVec4S32() const { return m_value.getVec4S32(); }


        //////////////////////////////////////////
        inline Vec4U const& getVecU() const { return m_value.getVecU(); }

        //////////////////////////////////////////
        inline Vec2U getVec2U32() const { return m_value.getVec2U32(); }

        //////////////////////////////////////////
        inline Vec3U getVec3U32() const { return m_value.getVec3U32(); }

        //////////////////////////////////////////
        inline Vec4U getVec4U32() const { return m_value.getVec4U32(); }


        //////////////////////////////////////////
        inline Vec4B const& getVecB() const { return m_value.getVecB(); }

        //////////////////////////////////////////
        inline Vec2B getVec2B() const { return m_value.getVec2B(); }

        //////////////////////////////////////////
        inline Vec3B getVec3B() const { return m_value.getVec3B(); }

        //////////////////////////////////////////
        inline Vec4B getVec4B() const { return m_value.getVec4B(); }
        

        
        //////////////////////////////////////////
        inline Mat3F const& getMat3F32() const { return m_value.getMat3F32(); }
        
        //////////////////////////////////////////
        inline Mat4F const& getMat4F32() const { return m_value.getMat4F32(); }

        //////////////////////////////////////////
        inline TMat const& getTMat() const { return m_value.getTMat(); }

        
        
        
        //////////////////////////////////////////
        bool set(S32 _value);
        
        //////////////////////////////////////////
        bool set(F32 _value);
        
        //////////////////////////////////////////
        bool set(F64 _value);

        //////////////////////////////////////////
        bool set(bool _value);
        
        //////////////////////////////////////////
        bool set(Texture2DPtr const& _texture2D);
        
        //////////////////////////////////////////
        inline bool set(Texture2D const* _texture2D)
        {
            return set(const_cast<Texture2D*>(_texture2D)->cast<Texture2D>());
        }

        //////////////////////////////////////////
        bool set(Texture2D const** _textures, U32 _count);

        //////////////////////////////////////////
        bool set(TextureCubePtr const& _textureCube);

        //////////////////////////////////////////
        inline bool set(TextureCube const* _textureCube)
        {
            return set(const_cast<TextureCube*>(_textureCube)->cast<TextureCube>());
        }
        
        //////////////////////////////////////////
        bool set(Vec2F const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec3F const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec4F const& _vector);


        //////////////////////////////////////////
        bool set(Vec2S const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec3S const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec4S const& _vector);


        //////////////////////////////////////////
        bool set(Vec2U const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec3U const& _vector);
        
        //////////////////////////////////////////
        bool set(Vec4U const& _vector);


        //////////////////////////////////////////
        bool set(Vec2B const& _vector);

        //////////////////////////////////////////
        bool set(Vec3B const& _vector);

        //////////////////////////////////////////
        bool set(Vec4B const& _vector);


        //////////////////////////////////////////
        bool set(Mat3F const& _matrix);
        
        //////////////////////////////////////////
        bool setAffineMat(Mat3F const& _matrix);
        
        //////////////////////////////////////////
        bool set(Mat4F const& _matrix);

        //////////////////////////////////////////
        bool set(TMat const& _matrix);
       
        //////////////////////////////////////////
        bool set(ShaderUniformVariant const& _variant);
       

        //////////////////////////////////////////
        virtual void upload(F32 const* _value, Size _count) MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual void upload(Vec2F const* _vectors, Size _count) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void upload(Vec3F const* _vectors, Size _count) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void upload(Vec4F const* _vectors, Size _count) MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual void upload(Mat3F const* _matrices, Size _count) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void upload(Mat4F const* _matrices, Size _count) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void upload(TMat const* _matrices, Size _count) MAZE_ABSTRACT;
        
        
        //////////////////////////////////////////
        bool set(ColorF128 const& _value);


        //////////////////////////////////////////
        inline Shader* getShaderRaw() const { return m_shaderRaw; }

    protected:

        //////////////////////////////////////////
        ShaderUniform();

        //////////////////////////////////////////
        virtual bool init(ShaderPtr const& _shader, ShaderUniformType _type = ShaderUniformType::None);

        //////////////////////////////////////////
        void setName(HashedCString _name) { m_value.setName(HashedString(_name)); }

        
        //////////////////////////////////////////
        virtual void processUniformWillBeChanged() {};

        //////////////////////////////////////////
        virtual void processSimpleUniformChanged() MAZE_ABSTRACT;


    
    protected:
        ShaderWPtr m_shader;
        Shader* m_shaderRaw;

        ShaderUniformVariant m_value;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderUniform_hpp_
//////////////////////////////////////////
