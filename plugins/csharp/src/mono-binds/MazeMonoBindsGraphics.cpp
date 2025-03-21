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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsGraphics.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-core/helpers/MazeThreadHelper.hpp"
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer3D.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"



//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    inline void MeshRendererSetMaterial(Component* _component, AssetUnitId _auid)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<MeshRenderer>::UID(), "Component is not Transform3D!");

        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getOrLoadMaterial(_auid);
        _component->castRaw<MeshRenderer>()->setMaterial(material);
    }

    //////////////////////////////////////////
    inline void MeshRendererSetRenderMesh(Component* _component, S32 _resourceId)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<MeshRenderer>::UID(), "Component is not Transform3D!");

        RenderMesh* renderMesh = RenderMesh::GetResource(_resourceId);
        _component->castRaw<MeshRenderer>()->setRenderMesh(renderMesh ? renderMesh->getSharedPtr() : RenderMeshPtr());
    }

    //////////////////////////////////////////
    inline void MeshRendererGetRenderMesh(Component* _component, S32& _outResourceId)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<MeshRenderer>::UID(), "Component is not Transform3D!");

        RenderMeshPtr const& renderMesh = _component->castRaw<MeshRenderer>()->getRenderMesh();
        if (renderMesh)
            _outResourceId = renderMesh->getResourceId();
        else
            _outResourceId = c_invalidEcsSceneId;
    }

    //////////////////////////////////////////
    inline void Camera3DGetAspectRatio(Component* _component, F32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");
        _outValue = _component->castRaw<Camera3D>()->getAspectRatio();
    }

    //////////////////////////////////////////
    inline void Camera3DGetOrthographicSize(Component* _component, F32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");
        _outValue = _component->castRaw<Camera3D>()->getOrthographicSize();
    }

    //////////////////////////////////////////
    inline void Camera3DSetOrthographicSize(Component* _component, F32 _value)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");
        _component->castRaw<Camera3D>()->setOrthographicSize(_value);
    }

    //////////////////////////////////////////
    inline void Camera3DGetNearZ(Component* _component, F32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");
        _outValue = _component->castRaw<Camera3D>()->getNearZ();
    }

    //////////////////////////////////////////
    inline void Camera3DSetNearZ(Component* _component, F32 _value)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");
        _component->castRaw<Camera3D>()->setNearZ(_value);
    }

    //////////////////////////////////////////
    inline void Camera3DGetFarZ(Component* _component, F32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");
        _outValue = _component->castRaw<Camera3D>()->getFarZ();
    }

    //////////////////////////////////////////
    inline void Camera3DSetFarZ(Component* _component, F32 _value)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");
        _component->castRaw<Camera3D>()->setFarZ(_value);
    }

    //////////////////////////////////////////
    inline void Camera3DGetRenderMask(Component* _component, S32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");
        _outValue = _component->castRaw<Camera3D>()->getRenderMask();
    }

    //////////////////////////////////////////
    inline void Camera3DSetRenderMask(Component* _component, S32 _value)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");
        _component->castRaw<Camera3D>()->setRenderMask(_value);
    }

    //////////////////////////////////////////
    inline void Camera3DGetRenderTarget(Component* _component, S32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");
        RenderTargetPtr const& renderTarget = _component->castRaw<Camera3D>()->getRenderTarget();
        _outValue = renderTarget ? renderTarget->getResourceId() : c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline void Camera3DSetRenderTarget(Component* _component, S32 _resourceId)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");

        RenderTarget* renderTarget = RenderWindow::GetResource(_resourceId);
        _component->castRaw<Camera3D>()->setRenderTarget(renderTarget ? renderTarget->getSharedPtr() : nullptr);
    }

    //////////////////////////////////////////
    inline void Camera3DConvertViewportCoordsToRay(Component* _component, Vec2F const& _positionV, Ray& _ray)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Camera3D>::UID(), "Component is not Camera3D!");

        _ray = _component->castRaw<Camera3D>()->convertViewportCoordsToRay(_positionV);
    }

    //////////////////////////////////////////
    inline S32 CreateSubMesh()
    {
        SubMeshPtr subMesh = SubMesh::Create();
        subMesh.incRef();
        return subMesh->getResourceId();
    }

    //////////////////////////////////////////
    inline void DestroySubMesh(S32 _subMeshId)
    {
        TaskManager::GetInstancePtr()->addMainThreadTask(
            [_subMeshId]()
            {
                if (SubMesh* subMesh = SubMesh::GetResource(_subMeshId))
                    subMesh->getSharedPtr().decRef();
            });
    }

    //////////////////////////////////////////
    inline void SubMeshSetRenderDrawTopology(
        S32 _subMeshId,
        S32 _topology)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setRenderDrawTopology((RenderDrawTopology)_topology);
    }

    //////////////////////////////////////////
    inline void SubMeshSetIndicesU16(
        S32 _subMeshId,
        U16 const* _indices,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setIndices(_indices, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetIndicesU32(
        S32 _subMeshId,
        U32 const* _indices,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setIndices(_indices, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetPositions(
        S32 _subMeshId,
        Vec3F const* _positions,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setPositions(_positions, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetNormals(
        S32 _subMeshId,
        Vec3F const* _normals,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setNormals(_normals, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetTangents(
        S32 _subMeshId,
        Vec3F const* _tangents,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setTangents(_tangents, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetBitangents(
        S32 _subMeshId,
        Vec3F const* _bitangents,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setBitangents(_bitangents, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetColors(
        S32 _subMeshId,
        Vec4F const* _colors,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setColors(_colors, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetTexCoordsVec2F(
        S32 _subMeshId,
        S32 _uvIndex,
        Vec2F const* _uv,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setTexCoords(_uvIndex, _uv, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetTexCoordsVec4F(
        S32 _subMeshId,
        S32 _uvIndex,
        Vec4F const* _uv,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setTexCoords(_uvIndex, _uv, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetBlendWeights(
        S32 _subMeshId,
        Vec4F const* _blendWeights,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setBlendWeights(_blendWeights, (Size)_count);
    }

    //////////////////////////////////////////
    inline void SubMeshSetBlendIndices(
        S32 _subMeshId,
        Vec4F const* _blendIndices,
        S32 _count)
    {
        if (SubMesh* subMesh = SubMesh::GetResourceUnsafe(_subMeshId))
            subMesh->setBlendIndices(_blendIndices, (Size)_count);
    }

    //////////////////////////////////////////
    inline S32 CreateMesh()
    {
        MeshPtr mesh = Mesh::Create();
        mesh.incRef();
        return mesh->getResourceId();
    }

    //////////////////////////////////////////
    inline void DestroyMesh(S32 _meshId)
    {
        TaskManager::GetInstancePtr()->addMainThreadTask(
            [_meshId]()
            {
                if (Mesh* mesh = Mesh::GetResource(_meshId))
                    mesh->getSharedPtr().decRef();
            });
    }

    //////////////////////////////////////////
    inline void MeshAddSubMesh(S32 _meshId, S32 _subMeshId)
    {
        if (Mesh* mesh = Mesh::GetResource(_meshId))
            if (SubMesh* subMesh = SubMesh::GetResource(_subMeshId))
                mesh->addSubMesh(subMesh->getSharedPtr());
    }

    //////////////////////////////////////////
    inline void MeshClear(S32 _meshId)
    {
        if (Mesh* mesh = Mesh::GetResource(_meshId))
            mesh->clear();
    }

    //////////////////////////////////////////
    inline S32 CreateRenderMesh()
    {
        RenderMeshPtr renderMesh = RenderMesh::Create();
        renderMesh.incRef();
        return renderMesh->getResourceId();
    }

    //////////////////////////////////////////
    inline void DestroyRenderMesh(S32 _renderMeshId)
    {
        TaskManager::GetInstancePtr()->addMainThreadTask(
            [_renderMeshId]()
            {
                if (RenderMesh* renderMesh = RenderMesh::GetResource(_renderMeshId))
                    renderMesh->getSharedPtr().decRef();
            });
    }

    //////////////////////////////////////////
    inline void RenderMeshLoadFromMesh(S32 _renderMeshId, S32 _meshId)
    {
        if (RenderMesh* renderMesh = RenderMesh::GetResource(_renderMeshId))
            if (Mesh* mesh = Mesh::GetResource(_meshId))
                renderMesh->loadFromMesh(mesh->getSharedPtr());
    }

    //////////////////////////////////////////
    inline S32 CreateRenderWindow(
        Vec2U const& _size,
        MonoString* _title)
    {
        Char* cstr = mono_string_to_utf8(_title);

        RenderWindowParams params;
        params.windowParams =
            WindowParams::Create(
                _size,
                32,
                cstr);

        mono_free(cstr);

        RenderWindowPtr renderWindow = RenderWindow::Create(params);
        renderWindow.incRef();
        return renderWindow->getResourceId();
    }

    //////////////////////////////////////////
    inline void DestroyRenderWindow(S32 _resourceId)
    {
        auto func =
            [_resourceId]()
            {
                if (RenderWindow* renderTarget = (RenderWindow*)RenderWindow::GetResource(_resourceId))
                    renderTarget->getSharedPtr().decRef();
            };


        if (TaskManager::IsMainThread())
            func();
        else
            TaskManager::GetInstancePtr()->addMainThreadTask(func);
    }

    //////////////////////////////////////////
    inline S32 EcsRenderSceneGetRenderTarget(Component* _component)
    {
        EcsRenderScene* scene = _component->getEntityRaw()->getEcsScene()->castRaw<EcsRenderScene>();
        return scene->getRenderTarget()->getResourceId();
    }

    //////////////////////////////////////////
    inline void CanvasGetRenderTarget(Component* _component, S32& _outValue)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Canvas>::UID(), "Component is not Camera3D!");
        RenderTargetPtr const& renderTarget = _component->castRaw<Canvas>()->getRenderTarget();
        _outValue = renderTarget ? renderTarget->getResourceId() : c_invalidResourceId;
    }

    //////////////////////////////////////////
    inline void CanvasSetRenderTarget(Component* _component, S32 _resourceId)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Canvas>::UID(), "Component is not Camera3D!");

        RenderTarget* renderTarget = RenderWindow::GetResource(_resourceId);
        _component->castRaw<Canvas>()->setRenderTarget(renderTarget ? renderTarget->getSharedPtr() : nullptr);
    }

    //////////////////////////////////////////
    inline void AbstractTextRendererGetText(Component* _component, MonoString* _text)
    {
        MAZE_ERROR_RETURN_IF(!_component->getMetaClass()->isInheritedFrom<AbstractTextRenderer>(), "Component is not AbstractTextRenderer!");
        String const& text = _component->castRaw<AbstractTextRenderer>()->getText();
        mono_string_new(MonoEngine::GetMonoDomain(), text.c_str());
    }

    //////////////////////////////////////////
    inline void AbstractTextRendererSetText(Component* _component, MonoString* _text)
    {
        MAZE_ERROR_RETURN_IF(!_component->getMetaClass()->isInheritedFrom<AbstractTextRenderer>(), "Component is not AbstractTextRenderer!");
        Char* cstr = mono_string_to_utf8(_text);
        _component->castRaw<AbstractTextRenderer>()->setText(cstr);
    }

    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsGraphics()
    {
        // MeshRenderer
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererSetMaterial);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererSetRenderMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshRendererGetRenderMesh);

        // Camera3D
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetOrthographicSize);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DSetOrthographicSize);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetAspectRatio);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetNearZ);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DSetNearZ);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetFarZ);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DSetFarZ);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetRenderMask);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DSetRenderMask);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DGetRenderTarget);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DSetRenderTarget);
        MAZE_GRAPHICS_MONO_BIND_FUNC(Camera3DConvertViewportCoordsToRay);
        
        // SubMesh
        MAZE_GRAPHICS_MONO_BIND_FUNC(CreateSubMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(DestroySubMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetRenderDrawTopology);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetIndicesU16);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetIndicesU32);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetPositions);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetNormals);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetTangents);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetBitangents);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetColors);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetTexCoordsVec2F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetTexCoordsVec4F);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetBlendWeights);
        MAZE_GRAPHICS_MONO_BIND_FUNC(SubMeshSetBlendIndices);

        // Mesh
        MAZE_GRAPHICS_MONO_BIND_FUNC(CreateMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(DestroyMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshAddSubMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(MeshClear);

        // RenderMesh
        MAZE_GRAPHICS_MONO_BIND_FUNC(CreateRenderMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(DestroyRenderMesh);
        MAZE_GRAPHICS_MONO_BIND_FUNC(RenderMeshLoadFromMesh);

        // RenderWindow
        MAZE_GRAPHICS_MONO_BIND_FUNC(CreateRenderWindow);
        MAZE_GRAPHICS_MONO_BIND_FUNC(DestroyRenderWindow);

        // EcsRenderScene
        MAZE_GRAPHICS_MONO_BIND_FUNC(EcsRenderSceneGetRenderTarget);

        // Canvas
        MAZE_GRAPHICS_MONO_BIND_FUNC(CanvasGetRenderTarget);
        MAZE_GRAPHICS_MONO_BIND_FUNC(CanvasSetRenderTarget);

        // AbstractTextRenderer
        MAZE_GRAPHICS_MONO_BIND_FUNC(AbstractTextRendererGetText);
        MAZE_GRAPHICS_MONO_BIND_FUNC(AbstractTextRendererSetText);
    }

} // namespace Maze
//////////////////////////////////////////
