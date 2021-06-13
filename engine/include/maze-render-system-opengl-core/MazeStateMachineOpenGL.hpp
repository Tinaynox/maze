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
#if (!defined(_MazeStateMachineOpenGL_hpp_))
#define _MazeStateMachineOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeBlendMode.hpp"
#include "maze-graphics/MazeCullMode.hpp"
#include "maze-graphics/MazeCompareFunction.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeTextureCube.hpp"
#include "maze-render-system-opengl-core/MazeConfigOpenGL.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    MAZE_USING_SHARED_PTR(StateMachineOpenGL);
    MAZE_USING_SHARED_PTR(RenderBuffer);


    //////////////////////////////////////////
    // Class StateMachineOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API StateMachineOpenGL
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        StateMachineOpenGL(ContextOpenGL* _context);

        //////////////////////////////////////////
        ~StateMachineOpenGL();

        //////////////////////////////////////////
        void setup();

        //////////////////////////////////////////
        void syncStates();


        //////////////////////////////////////////
        inline MZGLenum getActiveTexture() const { return m_activeTexture; }

        //////////////////////////////////////////
        void activeTexture(MZGLenum _activeTexture);


        //////////////////////////////////////////
        inline MZGLuint getBindedTexture() const
        {
            S32 activeTextureIndex = m_activeTexture - MAZE_GL_TEXTURE0;
            return m_bindTextureIds[activeTextureIndex];
        }


        //////////////////////////////////////////
        inline bool getBlendEnabled() const { return m_blendEnabled; }

        //////////////////////////////////////////
        void setBlendEnabled(bool _value);


        //////////////////////////////////////////
        inline BlendFactor getBlendSrcFactor() const { return m_blendSrcFactor; }

        //////////////////////////////////////////
        inline BlendFactor getBlendDestFactor() const { return m_blendDestFactor; }

        //////////////////////////////////////////
        void setBlendFactors(BlendFactor _srcBlendFactor, BlendFactor _destBlendFactor);


        //////////////////////////////////////////
        void setDepthTestEnabled(bool _depthTestEnabled);

        //////////////////////////////////////////
        inline bool getDepthTestEnabled() const { return m_depthTestEnabled; }

        //////////////////////////////////////////
        void setDepthTestCompareFunction(CompareFunction _depthTestCompareFunction);

        //////////////////////////////////////////
        inline CompareFunction getDepthTestCompareFunction() const { return m_depthTestCompareFunction; }


        //////////////////////////////////////////
        void setDepthWriteEnabled(bool _depthWriteEnabled);

        //////////////////////////////////////////
        inline bool getDepthWriteEnabled() const { return m_depthWriteEnabled; }


        //////////////////////////////////////////
        void setCullEnabled(bool _cullEnabled);

        //////////////////////////////////////////
        inline bool getCullEnabled() const { return m_depthWriteEnabled; }

        //////////////////////////////////////////
        void setCullMode(CullMode _cullMode);

        //////////////////////////////////////////
        inline CullMode getCullMode() const { return m_cullMode; }


        //////////////////////////////////////////
        inline Rect2DS const& getViewportRect() const { return m_viewportRect; }

        //////////////////////////////////////////
        void setViewportRect(Rect2DS const& _viewportRect);


        //////////////////////////////////////////
        void setScissorTestEnabled(bool _value);

        //////////////////////////////////////////
        inline bool getScissorTestEnabled() const { return m_scissorTest; }


        //////////////////////////////////////////
        void setScissorRect(Rect2DS const& _scissorRect);

        //////////////////////////////////////////
        inline Rect2DS const& getScissorRect() const { return m_scissorRect; }


        //////////////////////////////////////////
        inline MZGLuint getBindedFrameBuffer() const { return m_frameBuffer; }

        //////////////////////////////////////////
        void bindVertexArrayObject(MZGLuint _vertexArrayObject);

        //////////////////////////////////////////
        inline MZGLuint getBindedVertexArrayObject() const { return m_vertexArrayObject; }

        //////////////////////////////////////////
        void bindArrayBuffer(MZGLuint _arrayBuffer);

        //////////////////////////////////////////
        MZGLuint getBindedArrayBuffer() const;

        //////////////////////////////////////////
        void bindElementArrayBuffer(MZGLuint _elementArrayBuffer);

        //////////////////////////////////////////
        MZGLuint getBindedElementArrayBuffer() const;


        //////////////////////////////////////////
        void setClearColor(Vec4DF const& _clearColor);

        //////////////////////////////////////////
        inline Vec4DF const& getClearColor() const { return m_clearColor; }

        //////////////////////////////////////////
        void setClearDepth(F32 _clearDepth);

        //////////////////////////////////////////
        inline F32 getClearDepth() const { return m_clearDepth; }


        //////////////////////////////////////////
        void setWireframeRender(bool _value);

        //////////////////////////////////////////
        inline F32 getWireframeRender() const { return m_wireframeRender; }



        //////////////////////////////////////////
        void setMultiSampleEnabled(MZGLboolean _value);

        //////////////////////////////////////////
        inline MZGLboolean getMultiSampleEnable() const { return m_multiSampleEnabled; }
        

        //////////////////////////////////////////
        inline MZGLuint getProgram() const { return m_program; }


        //////////////////////////////////////////
        inline S32 getAntialiasingLevelSupport() const { return m_antialiasingLevelSupport; }

        //////////////////////////////////////////
        inline void setAntialiasingLevelSupport(S32 _value) { m_antialiasingLevelSupport = _value; }

        //////////////////////////////////////////
        inline bool getSRGBSupport() const { return m_sRgbSupport; }

        //////////////////////////////////////////
        inline void setSRGBSupport(bool _value) { m_sRgbSupport = _value; }

        //////////////////////////////////////////
        inline Shader* getCurrentShader() const { return m_currentShader; }

        //////////////////////////////////////////
        void setCurrentShader(Shader* _value);

        //////////////////////////////////////////
        inline Texture* getCurrentTexture(Size _i) { return m_currentTextures[_i]; }

        //////////////////////////////////////////
        void bindTexture(Texture* _texture);

        //////////////////////////////////////////
        void bindTexture2D(Texture2D* _texture);

        //////////////////////////////////////////
        void bindTextureCube(TextureCube* _texture);

        //////////////////////////////////////////
        inline RenderBuffer* getCurrentRenderBuffer() const { return m_currentRenderBuffer; }

        //////////////////////////////////////////
        void setCurrentRenderBuffer(RenderBuffer* _value);

        //////////////////////////////////////////
        inline Vec2DU const& getPixelBufferSize() const { return m_pixelBufferSize; }

        //////////////////////////////////////////
        inline void setPixelBufferSize(Vec2DU const& _pixelBufferSize) { m_pixelBufferSize = _pixelBufferSize; }


        //////////////////////////////////////////
        void processContextWillBeDestroyed();

    protected:

        //////////////////////////////////////////
        void notifyGLContextCreated(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL);

        
        //////////////////////////////////////////
        void useProgram(MZGLuint _program);

        //////////////////////////////////////////
        void bindFrameBuffer(MZGLuint _frameBuffer);

        //////////////////////////////////////////
        void bindTexture(MZGLenum _textureTarget, MZGLuint _textureId);

        //////////////////////////////////////////
        inline void bindTexture2D(MZGLuint _textureId) { bindTexture(MAZE_GL_TEXTURE_2D, _textureId); }

        //////////////////////////////////////////
        inline void bindTextureCube(MZGLuint _textureId) { bindTexture(MAZE_GL_TEXTURE_CUBE_MAP, _textureId); }
    
    protected:
        ContextOpenGL* m_context;

        MZGLenum m_activeTexture;
        MZGLenum m_bindTextureTargets[MAZE_GL_MAX_TEXTURES_COUNT];
        MZGLuint m_bindTextureIds[MAZE_GL_MAX_TEXTURES_COUNT];
        MZGLuint m_program;
        MZGLuint m_frameBuffer;
        MZGLuint m_vertexArrayObject;

        Rect2DS m_viewportRect;
        bool m_scissorTest;
        Rect2DS m_scissorRect;
        Vec4DF m_clearColor;
        F32 m_clearDepth;

        // BlendMode
        bool m_blendEnabled;
        BlendFactor m_blendSrcFactor;
        BlendFactor m_blendDestFactor;

        // DepthTest
        bool m_depthTestEnabled;
        CompareFunction m_depthTestCompareFunction;

        // DepthWrite
        bool m_depthWriteEnabled;

        // CullMode
        bool m_cullEnabled;
        CullMode m_cullMode;

        MZGLboolean m_multiSampleEnabled;
        
        bool m_wireframeRender;


        S32 m_antialiasingLevelSupport;
        bool m_sRgbSupport;

        Shader* m_currentShader;
        Texture* m_currentTextures[MAZE_GL_MAX_TEXTURES_COUNT];
        RenderBuffer* m_currentRenderBuffer;
        Vec2DU m_pixelBufferSize;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeStateMachineOpenGL_hpp_
//////////////////////////////////////////
