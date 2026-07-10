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
#if (!defined(_MazeRenderQueueDX11_hpp_))
#define _MazeRenderQueueDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-core/system/MazeTimer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderQueueDX11);
    class RenderSystemDX11;
    class ShaderDX11;


    //////////////////////////////////////////
    // Class RenderQueueDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API RenderQueueDX11
        : public RenderQueue
    {
    public:

        //////////////////////////////////////////
        virtual ~RenderQueueDX11();

        //////////////////////////////////////////
        static RenderQueueDX11Ptr Create(RenderTarget* _renderTarget);


        //////////////////////////////////////////
        virtual void draw() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void clear() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderQueueDX11();

        //////////////////////////////////////////
        virtual bool init(RenderTarget* _renderTarget) MAZE_OVERRIDE;

        //////////////////////////////////////////
        RenderSystemDX11* getRenderSystemDX11Raw() const;

        //////////////////////////////////////////
        void bindRenderPass(RenderPass* _renderPass, bool _bindTextures);

        //////////////////////////////////////////
        void processDrawBegin();

    protected:
        Stack<Rect2S> m_scissorRects;
        Vec4F m_clipPlanes[8];
        bool m_clipPlaneEnabled[8];

        F32 m_drawTime = 0.0f;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderQueueDX11_hpp_
//////////////////////////////////////////
