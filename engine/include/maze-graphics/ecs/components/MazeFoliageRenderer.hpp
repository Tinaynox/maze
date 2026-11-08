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
#if (!defined(_MazeFoliageRenderer_hpp_))
#define _MazeFoliageRenderer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-graphics/foliage/MazeFoliageData.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(FoliageRenderer);
    MAZE_USING_SHARED_PTR(RenderMask);


    //////////////////////////////////////////
    // Class FoliageRenderer
    //
    // Draws all the instances of a FoliageData from a single entity.
    // Every foliage prefab is flattened once into one merged mesh per material,
    // instances are culled by chunks and drawn instanced
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API FoliageRenderer
        : public Component
        , public IRenderUnitDrawer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(FoliageRenderer, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(FoliageRenderer);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        struct RenderPart
        {
            RenderMeshPtr renderMesh;
            MaterialPtr material;
        };

        //////////////////////////////////////////
        struct ChunkCache
        {
            U32 version = 0u;
            Vec3F boundsCenter = Vec3F::c_zero;
            F32 boundsRadius = 0.0f;
            Vector<TMat> matrices;
        };

        //////////////////////////////////////////
        struct RenderLayer
        {
            AssetUnitId prefabAuid = c_invalidAssetUnitId;
            bool partsBuilt = false;
            Vector<RenderPart> parts;

            // Radius of the merged meshes around the prefab origin
            F32 localRadius = 0.0f;

            FlatHashMap<U64, ChunkCache> chunks;
        };

    public:

        //////////////////////////////////////////
        virtual ~FoliageRenderer();

        //////////////////////////////////////////
        static FoliageRendererPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        inline RenderSystem* getRenderSystem() const { return m_renderSystem; }


        //////////////////////////////////////////
        inline AssetFileId getDataFileId() const { return m_dataFileId; }

        //////////////////////////////////////////
        void setDataFileId(AssetFileId _value);

        //////////////////////////////////////////
        inline FoliageDataPtr const& getData() const { return m_data; }

        //////////////////////////////////////////
        // Creates (or reuses an existing) data file and assigns it to the renderer. Editor only
        bool createDataFile(Path const& _fullPath);


        //////////////////////////////////////////
        inline bool getEnabled() const { return m_enabled; }

        //////////////////////////////////////////
        inline void setEnabled(bool _value) { m_enabled = _value; }

        //////////////////////////////////////////
        inline F32 getDrawDistance() const { return m_drawDistance; }

        //////////////////////////////////////////
        inline void setDrawDistance(F32 _value) { m_drawDistance = _value; }

        //////////////////////////////////////////
        inline bool getCastShadows() const { return m_castShadows; }

        //////////////////////////////////////////
        inline void setCastShadows(bool _value) { m_castShadows = _value; }


        //////////////////////////////////////////
        inline RenderMaskPtr const& getRenderMask() const { return m_renderMask; }


        //////////////////////////////////////////
        // World transform is converted to the local space of the renderer and quantized,
        // the stored instance is returned so it can be removed or re-added exactly
        bool addInstance(
            AssetUnitId _prefabAuid,
            TMat const& _worldTransform,
            FoliageInstance& _outInstance);

        //////////////////////////////////////////
        bool addInstance(
            AssetUnitId _prefabAuid,
            FoliageInstance const& _instance);

        //////////////////////////////////////////
        bool removeInstance(
            AssetUnitId _prefabAuid,
            Vec3F const& _localPosition);

        //////////////////////////////////////////
        // Drops the flattened prefab meshes, they are rebuilt on the next draw (e.g. after a prefab was edited)
        void rebuildRenderLayers();


        //////////////////////////////////////////
        void syncRenderData(TMat const& _worldTransform);

        //////////////////////////////////////////
        inline Vector<RenderLayer> const& getRenderLayers() const { return m_renderLayers; }

        //////////////////////////////////////////
        void releaseRenderData();

    protected:

        //////////////////////////////////////////
        FoliageRenderer();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EntityCopyData _copyData = EntityCopyData()) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void drawDefaultPass(
            RenderQueuePtr const& _renderQueue,
            DefaultPassParams const& _params,
            RenderUnit const& _renderUnit) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void drawShadowPass(
            RenderQueuePtr const& _renderQueue,
            ShadowPassParams const& _params,
            RenderUnit const& _renderUnit) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void buildRenderParts(RenderLayer& _layer);

        //////////////////////////////////////////
        void rebuildChunkCache(
            RenderLayer& _layer,
            FoliageChunk const& _chunk,
            ChunkCache& _cache);

        //////////////////////////////////////////
        void drawInstances(
            RenderQueuePtr const& _renderQueue,
            Frustum const& _frustum,
            Vec3F const* _cameraPosition,
            RenderUnit const& _renderUnit);

    protected:
        RenderSystem* m_renderSystem = nullptr;

        AssetFileId m_dataFileId = c_invalidAssetFileId;
        FoliageDataPtr m_data;

        bool m_enabled = true;
        F32 m_drawDistance = 80.0f;
        bool m_castShadows = false;

        RenderMaskPtr m_renderMask;

        Vector<RenderLayer> m_renderLayers;
        U32 m_syncedDataVersion = U32_MAX;
        TMat m_syncedWorldTransform = TMat::c_identity;
        F32 m_syncedWorldScale = 1.0f;
    };


    //////////////////////////////////////////
    inline S32 EncodeFoliageRenderUnitIndex(S32 _layerIndex, S32 _partIndex) { return (_layerIndex << 8) | _partIndex; }

    //////////////////////////////////////////
    inline void DecodeFoliageRenderUnitIndex(S32 _index, S32& _outLayerIndex, S32& _outPartIndex)
    {
        _outLayerIndex = _index >> 8;
        _outPartIndex = _index & 0xFF;
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFoliageRenderer_hpp_
//////////////////////////////////////////
