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
#if (!defined(_MazeTextureCubeVulkan_hpp_))
#define _MazeTextureCubeVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazeTextureCube.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(TextureCubeVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // Class TextureCubeVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API TextureCubeVulkan
        : public TextureCube
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TextureCubeVulkan, TextureCube);

    public:

        //////////////////////////////////////////
        virtual ~TextureCubeVulkan();

        //////////////////////////////////////////
        static TextureCubeVulkanPtr Create(RenderSystemVulkan* _renderSystem);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;

        //////////////////////////////////////////
        RenderSystemVulkan* getRenderSystemVulkanRaw() const;

        //////////////////////////////////////////
        inline VkImage getImage() const { return m_image; }

        //////////////////////////////////////////
        inline VkImageView getImageView() const { return m_imageView; }

        //////////////////////////////////////////
        VkSampler ensureSampler();


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
        virtual void generateMipmaps() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void reload() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        TextureCubeVulkan();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void destroyTexture();

    protected:
        VkImage m_image = VK_NULL_HANDLE;
        VmaAllocation m_imageAllocation = VK_NULL_HANDLE;
        VkImageView m_imageView = VK_NULL_HANDLE;
        VkFormat m_format = VK_FORMAT_UNDEFINED;
        VkImageLayout m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        U32 m_mipLevels = 1u;
        bool m_hasMipmapsGenerationSupport = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTextureCubeVulkan_hpp_
//////////////////////////////////////////
