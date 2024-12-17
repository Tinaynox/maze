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
    MAZE_DECLARE_ENUMCLASS_21_API(MAZE_GRAPHICS_API, BuiltinMaterialType,
        Error,
        UV,
        Normal,
        Color,
        ColorTexture,
        ColorTextureCustomUV,
        ColorTextureAdditive,
        ColorTextureAdditivePA,
        ColorStreamHDR,
        Sprite,
        SpriteAdditivePA,
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
    // Struct MaterialLibraryDataCallbacks
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API MaterialLibraryDataCallbacks
    {
        std::function<void(bool)> requestLoad;
        std::function<void(bool)> requestUnload;
        std::function<void(bool)> requestReload;
        std::function<bool(Set<String> const&)> hasAnyOfTags;
    };


    //////////////////////////////////////////
    // Struct MaterialLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API MaterialLibraryData
    {
        //////////////////////////////////////////
        MaterialLibraryData(
            MaterialPtr const& _material = nullptr,
            MaterialLibraryDataCallbacks const& _callbacks = MaterialLibraryDataCallbacks(),
            DataBlock const& _data = DataBlock::c_empty)
            : material(_material)
            , callbacks(_callbacks)
            , data(_data)
        {}

        MaterialPtr material;
        MaterialLibraryDataCallbacks callbacks;
        DataBlock data;
    };


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
        inline StringKeyMap<MaterialLibraryData> const& getMaterialsLibrary() const { return m_materialsLibrary; }


        //////////////////////////////////////////
        MaterialLibraryData const* getMaterialLibraryData(HashedCString _materialName);

        //////////////////////////////////////////
        MaterialLibraryData const* getMaterialLibraryData(String const& _assetFileName) { return getMaterialLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        MaterialLibraryData const* getMaterialLibraryData(CString _assetFileName) { return getMaterialLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }

        //////////////////////////////////////////
        MaterialPtr const& getOrLoadMaterial(
            HashedCString _assetFileName,
            bool _syncLoad = true);

        //////////////////////////////////////////
        inline MaterialPtr const& getOrLoadMaterial(String const& _assetFileName, bool _syncLoad = true) { return getOrLoadMaterial(MAZE_HASHED_CSTRING(_assetFileName.c_str()), _syncLoad); }

        //////////////////////////////////////////
        inline MaterialPtr const& getOrLoadMaterial(CString _assetFileName, bool _syncLoad = true) { return getOrLoadMaterial(MAZE_HASHED_CSTRING(_assetFileName), _syncLoad); }

        //////////////////////////////////////////
        MaterialPtr const& getOrLoadMaterial(AssetFilePtr const& _assetFile, bool _syncLoad = true);

        //////////////////////////////////////////
        HashedCString getMaterialName(Material const* _material);


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
        inline MaterialPtr const& getSpriteMaterial() const { return getBuiltinMaterial(BuiltinMaterialType::Sprite); }



        //////////////////////////////////////////
        MaterialLibraryData* addMaterialToLibrary(
            MaterialPtr const& _material,
            MaterialLibraryDataCallbacks const& _callbacks = MaterialLibraryDataCallbacks(),
            DataBlock const& _info = DataBlock::c_empty);

        //////////////////////////////////////////
        void removeMaterialFromLibrary(HashedCString _materialName);

        //////////////////////////////////////////
        inline void removeMaterialFromLibrary(CString _materialName) { removeMaterialFromLibrary(HashedCString(_materialName)); }

        //////////////////////////////////////////
        inline void removeMaterialFromLibrary(String const& _materialName) { removeMaterialFromLibrary(HashedCString(_materialName.c_str())); }


        //////////////////////////////////////////
        Vector<MaterialPtr> getMaterialsSorted();


        //////////////////////////////////////////
        void loadAllAssetMaterials();

        //////////////////////////////////////////
        void unloadAssetMaterials(Set<String> const& _tags);

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

        //////////////////////////////////////////
        void notifyAssetFileMoved(AssetFilePtr const& _assetFile, Path const& _prevPath);

    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        StringKeyMap<MaterialLibraryData> m_materialsLibrary;

        MaterialPtr m_builtinMaterials[BuiltinMaterialType::MAX];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMaterialManager_hpp_
//////////////////////////////////////////
