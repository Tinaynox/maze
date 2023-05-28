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
#if (!defined(_MazeTexture2DMSOpenGL_hpp_))
#define _MazeTexture2DMSOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-graphics/MazeTexture2DMS.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazePixelFormat.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Texture2DMSOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    

    //////////////////////////////////////////
    // Class Texture2DMSOpenGLScopeBind
    //
    //////////////////////////////////////////
    class Texture2DMSOpenGLScopeBind
    {
    public:

        //////////////////////////////////////////    
        Texture2DMSOpenGLScopeBind(Texture2DMSOpenGL* _newTexture);

        //////////////////////////////////////////
        Texture2DMSOpenGLScopeBind(Texture2DMSOpenGLPtr const& _newTexture);

        //////////////////////////////////////////
        ~Texture2DMSOpenGLScopeBind();

    private:
        ContextOpenGL* m_context;

        Texture* m_prevTexture0;
        S32 m_activeTextureIndex;
    };


    //////////////////////////////////////////
    // Class Texture2DMSOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API Texture2DMSOpenGL
        : public Texture2DMS
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Texture2DMSOpenGL, Texture2DMS);

    public:

        //////////////////////////////////////////
        virtual ~Texture2DMSOpenGL();

        //////////////////////////////////////////
        static Texture2DMSOpenGLPtr Create(
            RenderSystem* _renderSystem,
            ContextOpenGL* _contextOpenGL);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;

        
        //////////////////////////////////////////
        inline ContextOpenGL* getContextOpenGL() const { return m_context; }

        //////////////////////////////////////////
        inline RenderSystemOpenGL* getRenderSystemOpenGL() const { return m_renderSystem->castRaw<RenderSystemOpenGL>(); }


        //////////////////////////////////////////
        inline MZGLuint getGLTexture() const { return m_glTexture; }


        //////////////////////////////////////////
        using Texture2DMS::loadEmpty;

        //////////////////////////////////////////
        virtual bool loadEmpty(
            Vec2DU const& _size,
            PixelFormat::Enum _internalPixelFormat = PixelFormat::None,
            S32 _samples = 0) MAZE_OVERRIDE;

        
        //////////////////////////////////////////
        virtual void saveToFileAsTGA(String const& _fileName, Vec2DU _size = Vec2DU::c_zero) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual PixelSheet2D readAsPixelSheet(PixelFormat::Enum _outputFormat = PixelFormat::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        RenderSystemOpenGL* getRenderSystemOpenGLRaw()
        {
            return m_renderSystem->castRaw<RenderSystemOpenGL>();
        }

        //////////////////////////////////////////
        virtual void copyImageFrom(
            U8 const* _pixels,
            PixelFormat::Enum _pixelFormat,
            U32 _width,
            U32 _height,
            U32 _x,
            U32 _y) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        Texture2DMSOpenGL();

        //////////////////////////////////////////
        using Texture2DMS::init;

        //////////////////////////////////////////
        virtual bool init(
            RenderSystem* _renderSystem,
            ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void generateGLObjects();

        //////////////////////////////////////////
        void deleteGLObjects();
    

        //////////////////////////////////////////
        void notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void setContextOpenGL(ContextOpenGL* _contextOpenGL);

    protected:
        ContextOpenGL* m_context;

        MZGLuint m_glTexture;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexture2DMSOpenGL_hpp_
//////////////////////////////////////////
