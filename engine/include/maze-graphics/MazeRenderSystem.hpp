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
#if (!defined(_MazeRenderSystem_hpp_))
#define _MazeRenderSystem_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/MazeShaderUniform.hpp"
#include "maze-graphics/MazeRenderPassType.hpp"
#include "maze-graphics/MazeGPUByteBufferUtils.hpp"
#include "maze-graphics/MazeVertex.hpp"
#include "maze-graphics/MazeRenderMeshPool.hpp"
#include "maze-graphics/MazeRenderBufferPool.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(Shader);
    MAZE_USING_SHARED_PTR(ShaderUniform);
    MAZE_USING_SHARED_PTR(RenderWindow);
    MAZE_USING_SHARED_PTR(ShaderSystem);
    MAZE_USING_SHARED_PTR(TextureManager);
    MAZE_USING_SHARED_PTR(MaterialManager);
    MAZE_USING_SHARED_PTR(SpriteManager);
    MAZE_USING_SHARED_PTR(RenderMeshManager);
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(VertexArrayObject);
    MAZE_USING_SHARED_PTR(VertexBufferObject);
    MAZE_USING_SHARED_PTR(Texture2D);
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(RenderPass);
    MAZE_USING_SHARED_PTR(RenderQueue);
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(GPUVertexBuffer);
    MAZE_USING_SHARED_PTR(GPUTextureBuffer);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(RenderBuffer);


    //////////////////////////////////////////
    // Class RenderSystem
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderSystem
        : public SharedObject<RenderSystem>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(RenderSystem);

        //////////////////////////////////////////
        friend class RenderTarget;

    public:

        //////////////////////////////////////////
        virtual ~RenderSystem();


        //////////////////////////////////////////
        inline bool getSystemInited() const { return m_systemInited; }


        //////////////////////////////////////////
        virtual String const& getName() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool isTextureFormatSupported(PixelFormat::Enum _pixelFormat) MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual bool setCurrentRenderTarget(RenderTarget* _renderTarget) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline RenderTarget* getCurrentRenderTarget() const { return m_currentRenderTarget; }


        //////////////////////////////////////////
        virtual ShaderUniformPtr createShaderUniform(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual RenderWindowPtr createRenderWindow(RenderWindowParams const& _params) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual VertexArrayObjectPtr createVertexArrayObject(RenderTarget* _renderTarget = nullptr) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual VertexBufferObjectPtr createVertexBufferObject(
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping = false,
            RenderTarget* _renderTarget = nullptr) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Texture2DPtr createTexture2D() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual MaterialPtr createMaterial() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual RenderPassPtr createRenderPass(
            RenderPassType _passType) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual GPUVertexBufferPtr createGPUVertexBuffer(
            VertexDataDescription const& _vertexDataDescription,
            Size _vertexCount,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual GPUTextureBufferPtr createGPUTextureBuffer(
            Vec2DU const& _size,
            PixelFormat::Enum _pixelFormat,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual RenderBufferPtr createRenderBuffer(
            RenderBuffer::RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>(),
            RenderTarget* _renderTarget = nullptr) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool processRenderWindowCreated(RenderWindowPtr const& _renderWindow);

        //////////////////////////////////////////
        virtual bool processRenderWindowDestroyed(RenderWindow* _renderWindow);

        //////////////////////////////////////////
        virtual void clearCurrentRenderTarget(
            bool _colorBuffer = true,
            bool _depthBuffer = true) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline ShaderSystemPtr const& getShaderSystem() const { return m_shaderSystem; }

        //////////////////////////////////////////
        inline TextureManagerPtr const& getTextureManager() const { return m_textureManager; }

        //////////////////////////////////////////
        inline SpriteManagerPtr const& getSpriteManager() const { return m_imageManager; }

        //////////////////////////////////////////
        inline RenderMeshManagerPtr const& getRenderMeshManager() const { return m_renderMeshManager; }

        //////////////////////////////////////////
        inline MaterialManagerPtr const& getMaterialManager() const { return m_materialManager; }


        //////////////////////////////////////////
        inline Set<RenderWindow*> const& getRenderWindows() const { return m_renderWindows; }


        //////////////////////////////////////////
        inline S32 getDrawCalls() const { return m_drawCalls; }

        //////////////////////////////////////////
        inline void incDrawCall() { ++m_drawCalls; }

        //////////////////////////////////////////
        inline void clearDrawCalls() { m_drawCalls = 0; }


        //////////////////////////////////////////
        inline S32 getDrawCallsLimit() const { return m_drawCallsLimit; }

        //////////////////////////////////////////
        inline void setDrawCallsLimit(S32 _value) { m_drawCallsLimit = _value; }

    public:
        MultiDelegate<> eventSystemInited;

    protected:

        //////////////////////////////////////////
        RenderSystem();

        //////////////////////////////////////////
        virtual bool init();    

        //////////////////////////////////////////
        virtual void processRenderTargetDestroyed(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void processSystemInited();

    protected:
        bool m_systemInited;

        RenderTarget* m_currentRenderTarget;

        ShaderSystemPtr m_shaderSystem;

        TextureManagerPtr m_textureManager;
        SpriteManagerPtr m_imageManager;
        RenderMeshManagerPtr m_renderMeshManager;
        MaterialManagerPtr m_materialManager;

        Set<RenderWindow*> m_renderWindows;

        S32 m_drawCalls;
        S32 m_drawCallsLimit;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderSystem_hpp_
//////////////////////////////////////////
