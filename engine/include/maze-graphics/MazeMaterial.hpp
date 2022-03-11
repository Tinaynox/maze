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
#if (!defined(_MazeMaterial_hpp_))
#define _MazeMaterial_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeRenderPassType.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/serialization/MazeXMLSerializable.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/MazeObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(RenderPass);
    MAZE_USING_SHARED_PTR(Shader);
    MAZE_USING_SHARED_PTR(Texture2D);
    MAZE_USING_SHARED_PTR(TextureCube);
    MAZE_USING_SHARED_PTR(AssetFile);
        

    //////////////////////////////////////////
    // Class Material
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Material
        : public SharedObject<Material>
        , public IXMLElementSerializable
        , public MultiDelegateCallbackReceiver
        , public ISharedCopyable<Material>
        , public Object
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Material, Object);

    public:

        //////////////////////////////////////////
        virtual ~Material();

        //////////////////////////////////////////
        static MaterialPtr Create(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        static MaterialPtr Create(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        static MaterialPtr Create(String const& _assetFileName);

        //////////////////////////////////////////
        static MaterialPtr Create(MaterialPtr const& _material);


        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(String const& _name) { m_name = _name; }


        //////////////////////////////////////////
        RenderPassPtr createRenderPass(RenderPassType _passType = RenderPassType::Default);

        //////////////////////////////////////////
        void addRenderPass(RenderPassPtr const& _renderPass);

        //////////////////////////////////////////
        inline Size getRenderPassesCount(RenderPassType _passType = RenderPassType::Default) const { return m_passes[_passType].size(); }

        //////////////////////////////////////////
        RenderPassPtr const& getRenderPass(RenderPassType _passType = RenderPassType::Default, Size _index = 0);

        //////////////////////////////////////////
        RenderPassPtr const& getFirstRenderPass(RenderPassType _passType = RenderPassType::Default) const;

        //////////////////////////////////////////
        void clearAllRenderPasses();

        //////////////////////////////////////////
        void clearAllUniforms();


        //////////////////////////////////////////
        void loadFromXMLDocument(tinyxml2::XMLDocument& _doc);

        //////////////////////////////////////////
        bool loadFromAssetFile(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        bool loadFromAssetFile(String const& _assetFileName);


        //////////////////////////////////////////
        virtual void set(MaterialPtr const& _material);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, S32 _value);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, F32 _value);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, TexturePtr const& _texture);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Texture2DPtr const& _texture2D);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, TextureCubePtr const& _textureCube);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec2DF const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec3DF const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec4DF const& _vector);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec2DS const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec3DS const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec4DS const& _vector);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec2DU const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec3DU const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec4DU const& _vector);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Mat3DF const& _matrix);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Mat4DF const& _matrix);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, ColorF128 const& _color);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, ShaderUniformVariant const& _shaderUniformVariant);


        //////////////////////////////////////////
        void setUniform(ShaderUniformVariant const& _uniformVariant);

        //////////////////////////////////////////
        ShaderUniformVariantPtr const& getUniform(HashedCString _uniformName);

        //////////////////////////////////////////
        ShaderUniformVariantPtr const& ensureUniform(HashedCString _uniformName, ShaderUniformType _type = ShaderUniformType::None);

        //////////////////////////////////////////
        FastVector<ShaderUniformVariantPtr> const& getUniforms() const { return m_uniforms; }


        //////////////////////////////////////////
        void applyRenderPassUniforms(RenderPass* _renderPass);

        //////////////////////////////////////////
        inline void applyRenderPassUniforms(RenderPassPtr const& _renderPass)
        {
            applyRenderPassUniforms(_renderPass.get());
        }

        //////////////////////////////////////////
        RenderSystem* getRenderSystem() const { return m_renderSystem; }

        //////////////////////////////////////////
        void logUniforms();

        //////////////////////////////////////////
        U32 calculateUniformsCRC32(U32 _seed = 0) const;

    public:

        //////////////////////////////////////////
        virtual void loadFromXMLElement(tinyxml2::XMLElement const* _element) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual tinyxml2::XMLElement* toXMLElement(tinyxml2::XMLDocument& _doc) const MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool saveToFile(String const& _fullpath);

    public:

        //////////////////////////////////////////
        static void FromString(MaterialPtr& _value, CString _data, Size _count);

        //////////////////////////////////////////
        static void ToString(Material const* _value, String& _data);

    protected:

        //////////////////////////////////////////
        Material();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem);

        //////////////////////////////////////////
        virtual bool init(MaterialPtr const& _material);

        //////////////////////////////////////////
        void addUniform(ShaderUniformVariant const& _shaderUniformVariant);


    protected:
        RenderSystem* m_renderSystem;

        String m_name;

        Vector<RenderPassPtr> m_passes[RenderPassType::MAX];
        FastVector<ShaderUniformVariantPtr> m_uniforms;
    };


    //////////////////////////////////////////
    MAZE_NOT_IMPLEMENTED_SERIALIZATION(Material);


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<MaterialPtr>::value), void>::type
        ValueToString(MaterialPtr const& _value, String& _data)
    {
        Material::ToString(_value.get(), _data);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<MaterialPtr>::value), void>::type
        ValueFromString(MaterialPtr& _value, CString _data, Size _count)
    {
        Material::FromString(_value, _data, _count);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<MaterialPtr>::value), U32>::type
        GetValueSerializationSize(MaterialPtr const& _value)
    {
        MAZE_NOT_IMPLEMENTED_RETURN_VALUE(0);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<MaterialPtr>::value), void>::type
        SerializeValue(MaterialPtr const& _value, U8* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<MaterialPtr>::value), void>::type
        DeserializeValue(MaterialPtr& _value, U8 const* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMaterial_hpp_
//////////////////////////////////////////
