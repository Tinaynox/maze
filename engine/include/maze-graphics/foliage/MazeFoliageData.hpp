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
#if (!defined(_MazeFoliageData_hpp_))
#define _MazeFoliageData_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/assets/MazeAssetFileId.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/math/MazeTMat.hpp"
#include "maze-core/system/MazePath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(FoliageData);


    //////////////////////////////////////////
    // Struct FoliageInstance
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API FoliageInstance
    {
        // Local to the entity of the FoliageRenderer
        Vec3F position = Vec3F::c_zero;
        F32 scale = 1.0f;

        // Normalized quaternion (x, y, z, w) * 32767
        S16 rotation[4] = { 0, 0, 0, 32767 };

        //////////////////////////////////////////
        Quaternion getRotation() const;

        //////////////////////////////////////////
        void setRotation(Quaternion const& _rotation);

        //////////////////////////////////////////
        TMat buildTransform() const;
    };

    // Persisted as is, the file format depends on it
    static_assert(sizeof(FoliageInstance) == 24, "FoliageInstance layout is a part of the .mzfoliage format");


    //////////////////////////////////////////
    // Struct FoliageChunk
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API FoliageChunk
    {
        Vector<FoliageInstance> instances;

        // Unique across the data, so a chunk recreated with the same key never matches a stale cache
        U32 version = 0u;
    };


    //////////////////////////////////////////
    // Struct FoliageLayer
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API FoliageLayer
    {
        AssetUnitId prefabAuid = c_invalidAssetUnitId;
        FlatHashMap<U64, FoliageChunk> chunks;
        Size instancesCount = 0u;
    };


    //////////////////////////////////////////
    // Class FoliageData
    //
    // Instances of the foliage prefabs, bucketed by XZ chunks.
    // Loaded from .mzfoliage asset files and shared by AssetFileId, so every scene
    // instance (editor, playtest) referring to the same file sees the same in-memory data
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API FoliageData
    {
    public:

        //////////////////////////////////////////
        static F32 const c_chunkSize;

        //////////////////////////////////////////
        static CString const c_fileExtension;

    public:

        //////////////////////////////////////////
        static FoliageDataPtr Create();

        //////////////////////////////////////////
        static FoliageDataPtr GetOrLoad(AssetFileId _assetFileId);

        //////////////////////////////////////////
        static FoliageDataPtr GetLoaded(AssetFileId _assetFileId);

        //////////////////////////////////////////
        static void SaveAllDirty();

        //////////////////////////////////////////
        // Editor: loaded data stays in memory after its renderers are gone, so undo/redo
        // across the renderer removal works with the same in-memory state
        static void SetKeepLoaded(bool _value);

        //////////////////////////////////////////
        ~FoliageData();


        //////////////////////////////////////////
        bool loadFromByteBuffer(ByteBuffer const& _byteBuffer);

        //////////////////////////////////////////
        void saveToByteBuffer(ByteBuffer& _byteBuffer) const;

        //////////////////////////////////////////
        bool saveToFile(Path const& _fullPath);

        //////////////////////////////////////////
        bool save();


        //////////////////////////////////////////
        inline Path const& getFullPath() const { return m_fullPath; }

        //////////////////////////////////////////
        inline AssetFileId getAssetFileId() const { return m_assetFileId; }

        //////////////////////////////////////////
        inline bool getDirty() const { return m_dirty; }


        //////////////////////////////////////////
        // Changes when layers or chunks are added or removed
        inline U32 getVersion() const { return m_version; }


        //////////////////////////////////////////
        inline S32 getLayersCount() const { return (S32)m_layers.size(); }

        //////////////////////////////////////////
        inline FoliageLayer const& getLayer(S32 _index) const { return m_layers[_index]; }

        //////////////////////////////////////////
        S32 findLayerIndex(AssetUnitId _prefabAuid) const;

        //////////////////////////////////////////
        S32 ensureLayer(AssetUnitId _prefabAuid);

        //////////////////////////////////////////
        Size getInstancesCount(AssetUnitId _prefabAuid) const;

        //////////////////////////////////////////
        Size getInstancesCount() const;


        //////////////////////////////////////////
        void addInstance(AssetUnitId _prefabAuid, FoliageInstance const& _instance);

        //////////////////////////////////////////
        bool removeInstance(AssetUnitId _prefabAuid, Vec3F const& _position, F32 _tolerance = 0.001f);

        //////////////////////////////////////////
        void clear();


        //////////////////////////////////////////
        static U64 GetChunkKey(Vec3F const& _position);

        //////////////////////////////////////////
        static U64 GetChunkKey(S32 _x, S32 _z);

        //////////////////////////////////////////
        static void GetChunkCoords(U64 _key, S32& _outX, S32& _outZ);

    protected:

        //////////////////////////////////////////
        FoliageData();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void markChanged();

    protected:
        Vector<FoliageLayer> m_layers;

        U32 m_version = 0u;
        U32 m_chunkVersionCounter = 0u;
        bool m_dirty = false;

        AssetFileId m_assetFileId = c_invalidAssetFileId;
        Path m_fullPath;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFoliageData_hpp_
//////////////////////////////////////////
