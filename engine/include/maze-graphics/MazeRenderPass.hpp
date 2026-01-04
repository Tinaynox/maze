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
#if (!defined(_MazeRenderPass_hpp_))
#define _MazeRenderPass_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeBlendMode.hpp"
#include "maze-graphics/MazeCullMode.hpp"
#include "maze-graphics/MazeCompareFunction.hpp"
#include "maze-graphics/MazeRenderPassType.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/serialization/MazeXMLSerializable.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderPass);
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Shader);
    MAZE_USING_MANAGED_SHARED_PTR(Texture2D);
    

    //////////////////////////////////////////
    // Class RenderPass
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderPass
        : public SharedObject<RenderPass>
        , public IXMLElementSerializable
        , public IDataBlockSerializable
        , public ISharedCopyable<RenderPass>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(RenderPass);

        //////////////////////////////////////////
        friend void Material::addRenderPass(RenderPassPtr const&);

        //////////////////////////////////////////
        friend void Material::set(MaterialPtr const&);


    public:

        //////////////////////////////////////////
        virtual ~RenderPass();


        //////////////////////////////////////////
        inline RenderSystem* getRenderSystem() const { return m_renderSystem; }


        //////////////////////////////////////////
        bool set(RenderPass const& _renderPass);

        //////////////////////////////////////////
        inline bool set(RenderPassPtr const& _renderPass) { return set(*_renderPass.get()); }

        //////////////////////////////////////////
        bool isEqual(RenderPass const& _renderPass) const;

        //////////////////////////////////////////
        inline bool isEqual(RenderPassPtr const& _renderPass) const { return isEqual(*_renderPass.get()); }

        
        //////////////////////////////////////////
        MAZE_FORCEINLINE RenderPassType getPassType() const { return m_passType; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void setRenderQueueIndex(U8 _renderQueueIndex) { m_renderQueueIndex = _renderQueueIndex; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE U8 getRenderQueueIndex() const { return m_renderQueueIndex; }


        //////////////////////////////////////////
        void setShader(ShaderPtr const& _shader);

        //////////////////////////////////////////
        inline ShaderPtr const& getShader() const { return m_shaderRef.getShader(); }

        //////////////////////////////////////////
        void setShader(Path const& _shaderResourceName);

        //////////////////////////////////////////
        HashedString const& getShaderName() const;


        //////////////////////////////////////////
        inline ShaderAssetRef const& getShaderRef() const { return m_shaderRef; }

        //////////////////////////////////////////
        inline void setShaderRef(ShaderAssetRef const& _value) { m_shaderRef.setShader(_value.getShader()); }


        //////////////////////////////////////////
        MAZE_FORCEINLINE void setBlendOperation(BlendOperation _blendOperation) { m_blendOperation = _blendOperation; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE BlendOperation getBlendOperation() const { return m_blendOperation; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void setBlendSrcFactor(BlendFactor _blendSrcFactor) { m_blendSrcFactor = _blendSrcFactor; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE BlendFactor getBlendSrcFactor() const { return m_blendSrcFactor; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void setBlendDestFactor(BlendFactor _blendDestFactor) { m_blendDestFactor = _blendDestFactor; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE BlendFactor getBlendDestFactor() const { return m_blendDestFactor; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void setBlendFactors(BlendFactor _src, BlendFactor _dest)
        {
            setBlendSrcFactor(_src);
            setBlendDestFactor(_dest);
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void setBlend(BlendOperation _blendOperation, BlendFactor _src, BlendFactor _dest)
        {
            setBlendOperation(_blendOperation);
            setBlendSrcFactor(_src);
            setBlendDestFactor(_dest);
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void setDepthTestCompareFunction(CompareFunction _depthTestCompareFunction) { m_depthTestCompareFunction = _depthTestCompareFunction; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE CompareFunction getDepthTestCompareFunction() const { return m_depthTestCompareFunction; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void setDepthWriteEnabled(bool _value) { m_depthWriteEnabled = _value; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE bool getDepthWriteEnabled() const { return m_depthWriteEnabled; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void setCullMode(CullMode _cullMode) { m_cullMode = _cullMode; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE CullMode getCullMode() const { return m_cullMode; }

        //////////////////////////////////////////
        void applyRenderPassUniforms();

        //////////////////////////////////////////
        U32 calculateCRC32(U32 _seed = 0) const;

        //////////////////////////////////////////
        bool hasUniform(HashedCString _uniformName);


        //////////////////////////////////////////
        MAZE_FORCEINLINE void dirtyShaderUniforms() { m_shaderUniformsDirty = true; }

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

    protected:

        //////////////////////////////////////////
        RenderPass();

        //////////////////////////////////////////
        virtual bool init(
            RenderSystem* _renderSystem,
            RenderPassType _passType);

        //////////////////////////////////////////
        virtual bool init(
            RenderPassPtr const& _renderPass);

        //////////////////////////////////////////
        void setPassType(RenderPassType _passType) { m_passType = _passType; }
        
        //////////////////////////////////////////
        void setMaterial(Material* _material);


        //////////////////////////////////////////
        void updateShaderUniforms();

    protected:
        RenderSystem* m_renderSystem = nullptr;

        Material* m_material = nullptr;

        RenderPassType m_passType = RenderPassType::None;

        U8 m_renderQueueIndex;

        ShaderAssetRef m_shaderRef;
        FastVector<ShaderUniformPtr> m_shaderUniforms;
        bool m_shaderUniformsDirty = true;

        BlendOperation m_blendOperation = BlendOperation::Add;
        BlendFactor m_blendSrcFactor = BlendFactor::One;
        BlendFactor m_blendDestFactor = BlendFactor::Zero;

        CompareFunction m_depthTestCompareFunction = CompareFunction::Disabled;
        bool m_depthWriteEnabled = true;

        CullMode m_cullMode = CullMode::Off;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderPass_hpp_
//////////////////////////////////////////
