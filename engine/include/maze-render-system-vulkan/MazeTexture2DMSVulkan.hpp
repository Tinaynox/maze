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
#if (!defined(_MazeTexture2DMSVulkan_hpp_))
#define _MazeTexture2DMSVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazeTexture2DMS.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Texture2DMSVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // Class Texture2DMSVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API Texture2DMSVulkan
        : public Texture2DMS
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Texture2DMSVulkan, Texture2DMS);

    public:

        //////////////////////////////////////////
        virtual ~Texture2DMSVulkan();

        //////////////////////////////////////////
        static Texture2DMSVulkanPtr Create(RenderSystemVulkan* _renderSystem);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;

        //////////////////////////////////////////
        RenderSystemVulkan* getRenderSystemVulkanRaw() const;

        //////////////////////////////////////////
        inline VkImage getImage() const { return m_image; }

        //////////////////////////////////////////
        inline VkImageView getImageView() const { return m_imageView; }

        //////////////////////////////////////////
        inline VkFormat getFormatVulkan() const { return m_format; }

        //////////////////////////////////////////
        inline VkSampleCountFlagBits getSampleCountVulkan() const { return m_sampleCount; }

        //////////////////////////////////////////
        inline VkImageLayout getCurrentLayout() const { return m_currentLayout; }

        //////////////////////////////////////////
        void transitionTo(VkCommandBuffer _commandBuffer, VkImageLayout _newLayout);


        //////////////////////////////////////////
        virtual bool loadEmpty(
            Vec2U const& _size,
            PixelFormat::Enum _internalPixelFormat,
            S32 _samples) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void copyImageFrom(
            U8 const* _pixels,
            PixelFormat::Enum _pixelFormat,
            U32 _width,
            U32 _height,
            U32 _x,
            U32 _y) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void saveToFileAsTGA(String const& _fileName, Vec2U _size = Vec2U::c_zero) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual PixelSheet2D readAsPixelSheet(PixelFormat::Enum _outputFormat = PixelFormat::None) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        Texture2DMSVulkan();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void destroyTexture();

    protected:
        VkImage m_image = VK_NULL_HANDLE;
        VmaAllocation m_imageAllocation = VK_NULL_HANDLE;
        VkImageView m_imageView = VK_NULL_HANDLE;
        VkFormat m_format = VK_FORMAT_UNDEFINED;
        VkImageAspectFlags m_aspect = VK_IMAGE_ASPECT_COLOR_BIT;
        VkImageLayout m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkSampleCountFlagBits m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexture2DMSVulkan_hpp_
//////////////////////////////////////////
