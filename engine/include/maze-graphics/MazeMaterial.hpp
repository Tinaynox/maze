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
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/serialization/MazeXMLSerializable.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/MazeObject.hpp"
#include "maze-core/data/MazeDataBlock.hpp"


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
        , public IDataBlockSerializable
        , public IStringSerializable
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
        void clear();

        //////////////////////////////////////////
        void clearAllRenderPasses();

        //////////////////////////////////////////
        void clearAllUniforms();


        //////////////////////////////////////////
        bool isEqual(MaterialPtr const& _material) const;


        //////////////////////////////////////////
        bool loadFromXMLDocument(tinyxml2::XMLDocument& _doc);

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
        void setUniform(HashedCString _uniformName, Vec2F const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec3F const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec4F const& _vector);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec2S const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec3S const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec4S const& _vector);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec2U const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec3U const& _vector);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Vec4U const& _vector);


        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Mat3F const& _matrix);

        //////////////////////////////////////////
        void setUniform(HashedCString _uniformName, Mat4F const& _matrix);

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

        //////////////////////////////////////////
        bool hasUniform(HashedCString _uniformName);

        //////////////////////////////////////////
        bool saveToFile(String const& _fullpath);

    public:

        //////////////////////////////////////////
        virtual void loadFromXMLElement(tinyxml2::XMLElement const* _element) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual tinyxml2::XMLElement* toXMLElement(tinyxml2::XMLDocument& _doc) const MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        static void FromString(MaterialPtr& _value, CString _data, Size _count);

        //////////////////////////////////////////
        static void ToString(Material const* _value, String& _data);

    public:

        //////////////////////////////////////////
        static void IterateMaterials(std::function<bool(Material*)> _cb);

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

    protected:
        static Material* s_instancesList;
        Material* m_instancesListNext = nullptr;
        Material* m_instancesListPrev = nullptr;
    };


    //////////////////////////////////////////
    // Class MaterialAssetRef
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MaterialAssetRef
        : public IStringSerializable
    {
    public:

        //////////////////////////////////////////
        MaterialAssetRef(MaterialPtr const& _value = nullptr)
            : m_material(_value)
        {}

        //////////////////////////////////////////
        void setMaterial(MaterialPtr const& _value) { m_material = _value; }

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const { return m_material; }

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline bool operator==(MaterialAssetRef const& _value) const { return m_material == _value.getMaterial(); }

        //////////////////////////////////////////
        inline bool operator!=(MaterialAssetRef const& _value) const { return m_material != _value.getMaterial(); }

    private:
        MaterialPtr m_material;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMaterial_hpp_
//////////////////////////////////////////
