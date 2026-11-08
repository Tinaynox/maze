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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/foliage/MazeFoliageData.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/helpers/MazeByteBufferHelper.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeAABB3D.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
// .mzfoliage, version 1. Raw little-endian (all the targets are), laid out as:
//   Header
//   TypeEntry[typesCount]
//   ChunkEntry[chunksCount]
//   RangeEntry[rangesCount]       - one per (chunk, type) pair, grouped by chunk
//   FoliageInstance[instancesCount]
// Chunk bounds cover the instance positions only, renderers extend them by the mesh bounds.
// Instances are re-bucketed by position on load, so the chunk size may change between versions
//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace
    {
        //////////////////////////////////////////
        static U32 const c_foliageFileMagic = 0x4C465A4D; // "MZFL"
        static U32 const c_foliageFileVersion = 1u;
        static U8 const c_instanceFormatDefault = 0u;

        //////////////////////////////////////////
        struct FoliageFileHeader
        {
            U32 magic;
            U32 version;
            U32 flags;
            F32 chunkSize;
            U32 typesCount;
            U32 chunksCount;
            U32 rangesCount;
            U32 instancesCount;
        };
        static_assert(sizeof(FoliageFileHeader) == 32, "Unexpected FoliageFileHeader size");

        //////////////////////////////////////////
        struct FoliageFileTypeEntry
        {
            U32 prefabAuid;
            U8 instanceFormat;
            U8 reserved[3];
        };
        static_assert(sizeof(FoliageFileTypeEntry) == 8, "Unexpected FoliageFileTypeEntry size");

        //////////////////////////////////////////
        struct FoliageFileChunkEntry
        {
            S32 x;
            S32 z;
            F32 aabbMin[3];
            F32 aabbMax[3];
            U32 firstRange;
            U32 rangesCount;
        };
        static_assert(sizeof(FoliageFileChunkEntry) == 40, "Unexpected FoliageFileChunkEntry size");

        //////////////////////////////////////////
        struct FoliageFileRangeEntry
        {
            U32 typeIndex;
            U32 firstInstance;
            U32 instancesCount;
        };
        static_assert(sizeof(FoliageFileRangeEntry) == 12, "Unexpected FoliageFileRangeEntry size");

        //////////////////////////////////////////
        struct FoliageDataRegistryEntry
        {
            FoliageDataWPtr data;
            FoliageDataPtr keptData;
        };

        //////////////////////////////////////////
        inline FlatHashMap<AssetFileId, FoliageDataRegistryEntry>& GetFoliageDataRegistry()
        {
            static FlatHashMap<AssetFileId, FoliageDataRegistryEntry> s_registry;
            return s_registry;
        }

        //////////////////////////////////////////
        static bool s_keepLoadedFoliageData = false;

        //////////////////////////////////////////
        inline S16 PackQuaternionComponent(F32 _value)
        {
            return (S16)Math::Round(Math::Clamp(_value, -1.0f, 1.0f) * 32767.0f);
        }
    }


    //////////////////////////////////////////
    // Struct FoliageInstance
    //
    //////////////////////////////////////////
    Quaternion FoliageInstance::getRotation() const
    {
        Quaternion result(
            (F32)rotation[3] / 32767.0f,
            (F32)rotation[0] / 32767.0f,
            (F32)rotation[1] / 32767.0f,
            (F32)rotation[2] / 32767.0f);
        result.normalize();
        return result;
    }

    //////////////////////////////////////////
    void FoliageInstance::setRotation(Quaternion const& _rotation)
    {
        Quaternion q = _rotation;
        q.normalize();
        rotation[0] = PackQuaternionComponent(q.x);
        rotation[1] = PackQuaternionComponent(q.y);
        rotation[2] = PackQuaternionComponent(q.z);
        rotation[3] = PackQuaternionComponent(q.w);
    }

    //////////////////////////////////////////
    TMat FoliageInstance::buildTransform() const
    {
        TMat rotationMatrix;
        getRotation().toRotationMatrix(rotationMatrix);

        return
            TMat::CreateTranslation(position).transform(
                rotationMatrix).transform(
                    TMat::CreateScale(scale));
    }


    //////////////////////////////////////////
    // Class FoliageData
    //
    //////////////////////////////////////////
    F32 const FoliageData::c_chunkSize = 16.0f;

    //////////////////////////////////////////
    CString const FoliageData::c_fileExtension = "mzfoliage";

    //////////////////////////////////////////
    FoliageData::FoliageData()
    {
    }

    //////////////////////////////////////////
    FoliageData::~FoliageData()
    {
    }

    //////////////////////////////////////////
    FoliageDataPtr FoliageData::Create()
    {
        FoliageDataPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(FoliageData, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool FoliageData::init()
    {
        return true;
    }

    //////////////////////////////////////////
    FoliageDataPtr FoliageData::GetOrLoad(AssetFileId _assetFileId)
    {
        if (_assetFileId == c_invalidAssetFileId)
            return nullptr;

        FoliageDataPtr alive = GetLoaded(_assetFileId);
        if (alive)
            return alive;

        MAZE_ERROR_RETURN_VALUE_IF(!AssetManager::GetInstancePtr(), nullptr, "AssetManager is not initialized!");

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_assetFileId);
        MAZE_WARNING_RETURN_VALUE_IF(!assetFile, nullptr, "Foliage data file is not found! afid=%u", _assetFileId);

        FoliageDataPtr data = Create();
        data->m_assetFileId = _assetFileId;
        data->m_fullPath = assetFile->getFullPath();

        ByteBuffer byteBuffer;
        if (assetFile->readToByteBuffer(byteBuffer) && byteBuffer.getSize() > 0u)
        {
            MAZE_ERROR_IF(!data->loadFromByteBuffer(byteBuffer),
                "Failed to load foliage data: %s", data->m_fullPath.toUTF8().c_str());
        }

        FoliageDataRegistryEntry& entry = GetFoliageDataRegistry()[_assetFileId];
        entry.data = data;
        entry.keptData = s_keepLoadedFoliageData ? data : nullptr;
        return data;
    }

    //////////////////////////////////////////
    FoliageDataPtr FoliageData::GetLoaded(AssetFileId _assetFileId)
    {
        FlatHashMap<AssetFileId, FoliageDataRegistryEntry>& registry = GetFoliageDataRegistry();
        auto it = registry.find(_assetFileId);
        return it != registry.end() ? it->second.data.lock() : nullptr;
    }

    //////////////////////////////////////////
    void FoliageData::SaveAllDirty()
    {
        FlatHashMap<AssetFileId, FoliageDataRegistryEntry>& registry = GetFoliageDataRegistry();

        Vector<AssetFileId> expired;
        for (auto const& entry : registry)
        {
            FoliageDataPtr data = entry.second.data.lock();
            if (!data)
            {
                expired.push_back(entry.first);
                continue;
            }

            if (data->getDirty())
                MAZE_ERROR_IF(!data->save(), "Failed to save foliage data: %s", data->getFullPath().toUTF8().c_str());
        }

        for (AssetFileId assetFileId : expired)
            registry.erase(assetFileId);
    }

    //////////////////////////////////////////
    void FoliageData::SetKeepLoaded(bool _value)
    {
        s_keepLoadedFoliageData = _value;

        for (auto& entry : GetFoliageDataRegistry())
            entry.second.keptData = _value ? entry.second.data.lock() : nullptr;
    }

    //////////////////////////////////////////
    bool FoliageData::loadFromByteBuffer(ByteBuffer const& _byteBuffer)
    {
        clear();

        U32 size = _byteBuffer.getSize();
        U8 const* data = _byteBuffer.getDataRO();

        MAZE_ERROR_RETURN_VALUE_IF(size < sizeof(FoliageFileHeader), false, "Foliage data is too small!");

        FoliageFileHeader header;
        memcpy(&header, data, sizeof(header));

        MAZE_ERROR_RETURN_VALUE_IF(header.magic != c_foliageFileMagic, false, "Invalid foliage data magic!");
        MAZE_ERROR_RETURN_VALUE_IF(header.version != c_foliageFileVersion, false, "Unsupported foliage data version: %u", header.version);

        U64 typesOffset = sizeof(FoliageFileHeader);
        U64 chunksOffset = typesOffset + (U64)header.typesCount * sizeof(FoliageFileTypeEntry);
        U64 rangesOffset = chunksOffset + (U64)header.chunksCount * sizeof(FoliageFileChunkEntry);
        U64 instancesOffset = rangesOffset + (U64)header.rangesCount * sizeof(FoliageFileRangeEntry);
        U64 endOffset = instancesOffset + (U64)header.instancesCount * sizeof(FoliageInstance);

        MAZE_ERROR_RETURN_VALUE_IF(endOffset > size, false, "Foliage data is truncated!");

        Vector<FoliageFileTypeEntry> types(header.typesCount);
        if (header.typesCount > 0u)
            memcpy(types.data(), data + typesOffset, header.typesCount * sizeof(FoliageFileTypeEntry));

        for (FoliageFileTypeEntry const& type : types)
            MAZE_ERROR_RETURN_VALUE_IF(type.instanceFormat != c_instanceFormatDefault, false,
                "Unsupported foliage instance format: %u", (U32)type.instanceFormat);

        FoliageInstance const* instances = reinterpret_cast<FoliageInstance const*>(data + instancesOffset);

        for (U32 r = 0; r < header.rangesCount; ++r)
        {
            FoliageFileRangeEntry range;
            memcpy(&range, data + rangesOffset + (U64)r * sizeof(FoliageFileRangeEntry), sizeof(range));

            MAZE_ERROR_CONTINUE_IF(range.typeIndex >= header.typesCount, "Invalid foliage range type index!");
            MAZE_ERROR_CONTINUE_IF(
                (U64)range.firstInstance + range.instancesCount > header.instancesCount,
                "Invalid foliage range instances!");

            AssetUnitId prefabAuid = types[range.typeIndex].prefabAuid;
            for (U32 i = 0; i < range.instancesCount; ++i)
            {
                FoliageInstance instance;
                memcpy(&instance, instances + range.firstInstance + i, sizeof(FoliageInstance));
                addInstance(prefabAuid, instance);
            }
        }

        m_dirty = false;
        return true;
    }

    //////////////////////////////////////////
    void FoliageData::saveToByteBuffer(ByteBuffer& _byteBuffer) const
    {
        Vector<FoliageFileTypeEntry> types;
        Vector<S32> typeIndexPerLayer(m_layers.size(), -1);
        for (Size i = 0, in = m_layers.size(); i < in; ++i)
        {
            if (m_layers[i].instancesCount == 0u)
                continue;

            typeIndexPerLayer[i] = (S32)types.size();

            FoliageFileTypeEntry type;
            memset(&type, 0, sizeof(type));
            type.prefabAuid = m_layers[i].prefabAuid;
            type.instanceFormat = c_instanceFormatDefault;
            types.push_back(type);
        }

        // Every (chunk, layer) pair - grouped by chunk, so a chunk is one contiguous range of the range table
        struct ChunkRef
        {
            U64 key;
            S32 layerIndex;
            FoliageChunk const* chunk;
        };
        Vector<ChunkRef> chunkRefs;
        for (Size i = 0, in = m_layers.size(); i < in; ++i)
        {
            if (typeIndexPerLayer[i] < 0)
                continue;

            for (auto const& chunkData : m_layers[i].chunks)
                if (!chunkData.second.instances.empty())
                    chunkRefs.push_back({ chunkData.first, (S32)i, &chunkData.second });
        }

        // Deterministic output for identical data
        eastl::sort(chunkRefs.begin(), chunkRefs.end(),
            [](ChunkRef const& _a, ChunkRef const& _b)
            {
                if (_a.key != _b.key)
                    return _a.key < _b.key;
                return _a.layerIndex < _b.layerIndex;
            });

        Vector<FoliageFileChunkEntry> chunks;
        Vector<FoliageFileRangeEntry> ranges;
        U32 instancesCount = 0u;
        for (Size i = 0, in = chunkRefs.size(); i < in; ++i)
        {
            ChunkRef const& ref = chunkRefs[i];
            if (i == 0 || chunkRefs[i - 1].key != ref.key)
            {
                FoliageFileChunkEntry chunk;
                memset(&chunk, 0, sizeof(chunk));
                GetChunkCoords(ref.key, chunk.x, chunk.z);
                chunk.firstRange = (U32)ranges.size();
                chunk.aabbMin[0] = chunk.aabbMin[1] = chunk.aabbMin[2] = F32_MAX;
                chunk.aabbMax[0] = chunk.aabbMax[1] = chunk.aabbMax[2] = -F32_MAX;
                chunks.push_back(chunk);
            }

            FoliageFileChunkEntry& chunk = chunks.back();
            for (FoliageInstance const& instance : ref.chunk->instances)
            {
                chunk.aabbMin[0] = Math::Min(chunk.aabbMin[0], instance.position.x);
                chunk.aabbMin[1] = Math::Min(chunk.aabbMin[1], instance.position.y);
                chunk.aabbMin[2] = Math::Min(chunk.aabbMin[2], instance.position.z);
                chunk.aabbMax[0] = Math::Max(chunk.aabbMax[0], instance.position.x);
                chunk.aabbMax[1] = Math::Max(chunk.aabbMax[1], instance.position.y);
                chunk.aabbMax[2] = Math::Max(chunk.aabbMax[2], instance.position.z);
            }

            FoliageFileRangeEntry range;
            range.typeIndex = (U32)typeIndexPerLayer[ref.layerIndex];
            range.firstInstance = instancesCount;
            range.instancesCount = (U32)ref.chunk->instances.size();
            ranges.push_back(range);
            ++chunk.rangesCount;

            instancesCount += range.instancesCount;
        }

        FoliageFileHeader header;
        header.magic = c_foliageFileMagic;
        header.version = c_foliageFileVersion;
        header.flags = 0u;
        header.chunkSize = c_chunkSize;
        header.typesCount = (U32)types.size();
        header.chunksCount = (U32)chunks.size();
        header.rangesCount = (U32)ranges.size();
        header.instancesCount = instancesCount;

        Size totalSize =
            sizeof(FoliageFileHeader) +
            types.size() * sizeof(FoliageFileTypeEntry) +
            chunks.size() * sizeof(FoliageFileChunkEntry) +
            ranges.size() * sizeof(FoliageFileRangeEntry) +
            (Size)instancesCount * sizeof(FoliageInstance);

        _byteBuffer.clear();
        _byteBuffer.reserve(totalSize);

        _byteBuffer.append(reinterpret_cast<U8 const*>(&header), sizeof(header));
        if (!types.empty())
            _byteBuffer.append(reinterpret_cast<U8 const*>(types.data()), types.size() * sizeof(FoliageFileTypeEntry));
        if (!chunks.empty())
            _byteBuffer.append(reinterpret_cast<U8 const*>(chunks.data()), chunks.size() * sizeof(FoliageFileChunkEntry));
        if (!ranges.empty())
            _byteBuffer.append(reinterpret_cast<U8 const*>(ranges.data()), ranges.size() * sizeof(FoliageFileRangeEntry));

        for (ChunkRef const& ref : chunkRefs)
            _byteBuffer.append(
                reinterpret_cast<U8 const*>(ref.chunk->instances.data()),
                ref.chunk->instances.size() * sizeof(FoliageInstance));
    }

    //////////////////////////////////////////
    bool FoliageData::saveToFile(Path const& _fullPath)
    {
        ByteBuffer byteBuffer;
        saveToByteBuffer(byteBuffer);

        if (!ByteBufferHelper::SaveBinaryFile(byteBuffer, _fullPath))
            return false;

        if (_fullPath == m_fullPath)
            m_dirty = false;

        return true;
    }

    //////////////////////////////////////////
    bool FoliageData::save()
    {
        MAZE_ERROR_RETURN_VALUE_IF(m_fullPath.empty(), false, "Foliage data has no file!");
        return saveToFile(m_fullPath);
    }

    //////////////////////////////////////////
    S32 FoliageData::findLayerIndex(AssetUnitId _prefabAuid) const
    {
        for (S32 i = 0, in = (S32)m_layers.size(); i < in; ++i)
            if (m_layers[i].prefabAuid == _prefabAuid)
                return i;

        return -1;
    }

    //////////////////////////////////////////
    S32 FoliageData::ensureLayer(AssetUnitId _prefabAuid)
    {
        S32 index = findLayerIndex(_prefabAuid);
        if (index >= 0)
            return index;

        m_layers.emplace_back();
        m_layers.back().prefabAuid = _prefabAuid;
        ++m_version;
        return (S32)m_layers.size() - 1;
    }

    //////////////////////////////////////////
    Size FoliageData::getInstancesCount(AssetUnitId _prefabAuid) const
    {
        S32 index = findLayerIndex(_prefabAuid);
        return index >= 0 ? m_layers[index].instancesCount : 0u;
    }

    //////////////////////////////////////////
    Size FoliageData::getInstancesCount() const
    {
        Size result = 0u;
        for (FoliageLayer const& layer : m_layers)
            result += layer.instancesCount;
        return result;
    }

    //////////////////////////////////////////
    void FoliageData::addInstance(AssetUnitId _prefabAuid, FoliageInstance const& _instance)
    {
        FoliageLayer& layer = m_layers[ensureLayer(_prefabAuid)];

        U64 key = GetChunkKey(_instance.position);
        auto it = layer.chunks.find(key);
        if (it == layer.chunks.end())
        {
            it = layer.chunks.emplace(key, FoliageChunk()).first;
            ++m_version;
        }

        it->second.instances.push_back(_instance);
        it->second.version = ++m_chunkVersionCounter;
        ++layer.instancesCount;

        markChanged();
    }

    //////////////////////////////////////////
    bool FoliageData::removeInstance(AssetUnitId _prefabAuid, Vec3F const& _position, F32 _tolerance)
    {
        S32 layerIndex = findLayerIndex(_prefabAuid);
        if (layerIndex < 0)
            return false;

        FoliageLayer& layer = m_layers[layerIndex];

        U64 key = GetChunkKey(_position);
        auto it = layer.chunks.find(key);
        if (it == layer.chunks.end())
            return false;

        Vector<FoliageInstance>& instances = it->second.instances;

        F32 bestDistanceSq = _tolerance * _tolerance;
        S32 best = -1;
        for (S32 i = 0, in = (S32)instances.size(); i < in; ++i)
        {
            F32 distanceSq = (instances[i].position - _position).squaredLength();
            if (distanceSq <= bestDistanceSq)
            {
                bestDistanceSq = distanceSq;
                best = i;
            }
        }

        if (best < 0)
            return false;

        instances[best] = instances.back();
        instances.pop_back();
        --layer.instancesCount;

        if (instances.empty())
        {
            layer.chunks.erase(it);
            ++m_version;
        }
        else
        {
            it->second.version = ++m_chunkVersionCounter;
        }

        markChanged();
        return true;
    }

    //////////////////////////////////////////
    void FoliageData::clear()
    {
        if (m_layers.empty())
            return;

        m_layers.clear();
        ++m_version;
        markChanged();
    }

    //////////////////////////////////////////
    void FoliageData::markChanged()
    {
        m_dirty = true;
    }

    //////////////////////////////////////////
    U64 FoliageData::GetChunkKey(Vec3F const& _position)
    {
        return GetChunkKey(
            (S32)Math::Floor(_position.x / c_chunkSize),
            (S32)Math::Floor(_position.z / c_chunkSize));
    }

    //////////////////////////////////////////
    U64 FoliageData::GetChunkKey(S32 _x, S32 _z)
    {
        return ((U64)(U32)_x << 32) | (U64)(U32)_z;
    }

    //////////////////////////////////////////
    void FoliageData::GetChunkCoords(U64 _key, S32& _outX, S32& _outZ)
    {
        _outX = (S32)(U32)(_key >> 32);
        _outZ = (S32)(U32)(_key & 0xFFFFFFFFull);
    }

} // namespace Maze
//////////////////////////////////////////
