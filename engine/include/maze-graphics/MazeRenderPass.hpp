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
        inline RenderPassType getPassType() const { return m_passType; }

        //////////////////////////////////////////
        inline void setRenderQueueIndex(S32 _renderQueueIndex) { m_renderQueueIndex = _renderQueueIndex; }

        //////////////////////////////////////////
        inline S32 getRenderQueueIndex() const { return m_renderQueueIndex; }


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
        inline void setBlendOperation(BlendOperation _blendOperation) { m_blendOperation = _blendOperation; }

        //////////////////////////////////////////
        inline BlendOperation getBlendOperation() const { return m_blendOperation; }

        //////////////////////////////////////////
        inline void setBlendSrcFactor(BlendFactor _blendSrcFactor) { m_blendSrcFactor = _blendSrcFactor; }

        //////////////////////////////////////////
        inline BlendFactor getBlendSrcFactor() const { return m_blendSrcFactor; }

        //////////////////////////////////////////
        inline void setBlendDestFactor(BlendFactor _blendDestFactor) { m_blendDestFactor = _blendDestFactor; }    

        //////////////////////////////////////////
        inline BlendFactor getBlendDestFactor() const { return m_blendDestFactor; }        

        //////////////////////////////////////////
        inline void setBlendFactors(BlendFactor _src, BlendFactor _dest)
        {
            setBlendSrcFactor(_src);
            setBlendDestFactor(_dest);
        }

        //////////////////////////////////////////
        inline void setBlend(BlendOperation _blendOperation, BlendFactor _src, BlendFactor _dest)
        {
            setBlendOperation(_blendOperation);
            setBlendSrcFactor(_src);
            setBlendDestFactor(_dest);
        }

        //////////////////////////////////////////
        inline void setDepthTestCompareFunction(CompareFunction _depthTestCompareFunction) { m_depthTestCompareFunction = _depthTestCompareFunction; }

        //////////////////////////////////////////
        inline CompareFunction getDepthTestCompareFunction() const { return m_depthTestCompareFunction; }

        //////////////////////////////////////////
        inline void setDepthWriteEnabled(bool _value) { m_depthWriteEnabled = _value; }

        //////////////////////////////////////////
        inline bool getDepthWriteEnabled() const { return m_depthWriteEnabled; }

        //////////////////////////////////////////
        inline void setCullMode(CullMode _cullMode) { m_cullMode = _cullMode; }

        //////////////////////////////////////////
        inline CullMode getCullMode() const { return m_cullMode; }

        //////////////////////////////////////////
        void applyRenderPassUniforms();

        //////////////////////////////////////////
        U32 calculateCRC32(U32 _seed = 0) const;

        //////////////////////////////////////////
        bool hasUniform(HashedCString _uniformName);

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

    protected:
        RenderSystem* m_renderSystem;

        Material* m_material;

        RenderPassType m_passType;

        S32 m_renderQueueIndex;

        ShaderAssetRef m_shaderRef;

        BlendOperation m_blendOperation;
        BlendFactor m_blendSrcFactor;
        BlendFactor m_blendDestFactor;

        CompareFunction m_depthTestCompareFunction;
        bool m_depthWriteEnabled;

        CullMode m_cullMode;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderPass_hpp_
//////////////////////////////////////////
