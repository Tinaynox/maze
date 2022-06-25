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
#if (!defined(_MazeMaterialManager_hpp_))
#define _MazeMaterialManager_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MaterialManager);
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Material);


    //////////////////////////////////////////
    // Enum BuiltinMaterialType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_19_API(MAZE_GRAPHICS_API, BuiltinMaterialType,
        Error,
        UV,
        Normal,
        Color,
        ColorTexture,
        ColorTextureCustomUV,
        ColorTextureAdditive,
        ColorTextureAdditivePA,
        ColorHDR,
        HSVRect,
        HSVBand,
        ColorPickerChannel,
        DebugGrid,
        DebugAxis,
        Skybox,
        Specular,
        SpecularDS,
        MeshPreview,
        Font);


    //////////////////////////////////////////
    // Class MaterialManager
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MaterialManager 
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~MaterialManager();

        //////////////////////////////////////////
        static void Initialize(MaterialManagerPtr& _textureManager, RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        static MaterialManagerPtr const& GetCurrentInstance();


        //////////////////////////////////////////
        MaterialPtr const& getMaterial(HashedCString _assetFileName);

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial(String const& _assetFileName) { return getMaterial(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial(CString _assetFileName) { return getMaterial(MAZE_HASHED_CSTRING(_assetFileName)); }

        //////////////////////////////////////////
        MaterialPtr const& getMaterial(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        String const& getMaterialName(Material const* _material);


        //////////////////////////////////////////
        inline MaterialPtr const& getBuiltinMaterial(BuiltinMaterialType _materialType) const { return m_builtinMaterials[(Size)_materialType]; }
        
        //////////////////////////////////////////
        MaterialPtr const& createBuiltinMaterial(BuiltinMaterialType _materialType);

        //////////////////////////////////////////
        MaterialPtr const& ensureBuiltinMaterial(BuiltinMaterialType _materialType);

        //////////////////////////////////////////
        void createBuiltinMaterials();


        //////////////////////////////////////////
        inline MaterialPtr const& getErrorMaterial() const { return getBuiltinMaterial(BuiltinMaterialType::Error); }

        //////////////////////////////////////////
        inline MaterialPtr const& getColorMaterial() const { return getBuiltinMaterial(BuiltinMaterialType::Color); }

        //////////////////////////////////////////
        inline MaterialPtr const& getColorTextureMaterial() const { return getBuiltinMaterial(BuiltinMaterialType::ColorTexture); }



        //////////////////////////////////////////
        MaterialPtr const& addMaterial(MaterialPtr const& _material);

        //////////////////////////////////////////
        Vector<MaterialPtr> getMaterialsSorted();


        //////////////////////////////////////////
        void loadAllAssetMaterials();

    public:
        //////////////////////////////////////////
        MultiDelegate<> eventBuiltinMaterialsCreated;

    protected:

        //////////////////////////////////////////
        MaterialManager();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);
    
        //////////////////////////////////////////
        void notifyRenderSystemInited();

        //////////////////////////////////////////
        void notifyShaderSystemInited();

    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        StringKeyMap<MaterialPtr> m_materialsByName;

        MaterialPtr m_builtinMaterials[BuiltinMaterialType::MAX];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMaterialManager_hpp_
//////////////////////////////////////////
