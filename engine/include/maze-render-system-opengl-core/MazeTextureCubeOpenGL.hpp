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
#if (!defined(_MazeTextureCubeOpenGL_hpp_))
#define _MazeTextureCubeOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-graphics/MazeTextureCube.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazePixelFormat.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(TextureCubeOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    

    //////////////////////////////////////////
    // Class TextureCubeOpenGLScopeBind
    //
    //////////////////////////////////////////
    class TextureCubeOpenGLScopeBind
    {
    public:

        //////////////////////////////////////////    
        TextureCubeOpenGLScopeBind(TextureCubeOpenGL* _newTexture);

        //////////////////////////////////////////
        TextureCubeOpenGLScopeBind(TextureCubeOpenGLPtr const& _newTexture);

        //////////////////////////////////////////
        ~TextureCubeOpenGLScopeBind();

    private:
        ContextOpenGL* m_context;

        Texture* m_prevTexture0 = nullptr;
        S32 m_activeTextureIndex = 0;
    };


    //////////////////////////////////////////
    // Class TextureCubeOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API TextureCubeOpenGL
        : public TextureCube
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TextureCubeOpenGL, TextureCube);

    public:

        //////////////////////////////////////////
        virtual ~TextureCubeOpenGL();

        //////////////////////////////////////////
        static TextureCubeOpenGLPtr Create(
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
        using TextureCube::loadTexture;

        //////////////////////////////////////////
        virtual bool loadTexture(
            Vector<PixelSheet2D> const _pixelSheets[6],
            PixelFormat::Enum _internalPixelFormat = PixelFormat::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setMagFilter(TextureFilter _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setMinFilter(TextureFilter _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setWrapS(TextureWrap _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setWrapT(TextureWrap _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setWrapR(TextureWrap _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        RenderSystemOpenGL* getRenderSystemOpenGLRaw()
        {
            return m_renderSystem->castRaw<RenderSystemOpenGL>();
        }

        //////////////////////////////////////////
        virtual void generateMipmaps() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void reload() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        TextureCubeOpenGL();

        //////////////////////////////////////////
        using TextureCube::init;

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

        bool m_hasPresetMipmaps;

        Vector<PixelSheet2D> m_pixelSheetsTEMP[6];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTextureCubeOpenGL_hpp_
//////////////////////////////////////////
