using Maze.Core;
using System;
using System.Runtime.CompilerServices;

namespace Maze.Graphics
{
    public class InternalCalls
    {
        #region MeshRenderer
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshRendererSetMaterial(NativePtr nativeComponentPtr, AssetUnitId material);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshRendererSetRenderMesh(NativePtr nativeComponentPtr, int renderMeshResourceId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshRendererGetRenderMesh(NativePtr nativeComponentPtr, out int outRenderMeshResourceId);
        #endregion

        #region Camera3D
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetAspectRatio(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetOrthographicSize(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetOrthographicSize(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetNearZ(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetNearZ(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetFarZ(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetFarZ(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetRenderMask(NativePtr nativeComponentPtr, out int outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetRenderMask(NativePtr nativeComponentPtr, int value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetRenderTarget(NativePtr nativeComponentPtr, out int outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetRenderTarget(NativePtr nativeComponentPtr, int value);
        #endregion

        #region Mesh
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateMesh();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroyMesh(int meshId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshAddSubMesh(int meshId, int subMeshId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshClear(int meshId);
        #endregion

        #region SubMesh
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateSubMesh();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroySubMesh(int subMeshId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetRenderDrawTopology(int subMeshId, int topology);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetIndicesU16(int subMeshId, ushort* indices, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetIndicesU32(int subMeshId, uint* indices, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetPositions(int subMeshId, Vec3F* positions, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetNormals(int subMeshId, Vec3F* normals, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetTangents(int subMeshId, Vec3F* tangents, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetBitangents(int subMeshId, Vec3F* bitangents, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetColors(int subMeshId, Vec4F* colors, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetTexCoordsVec2F(int subMeshId, int uvIndex, Vec2F* uvs, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetTexCoordsVec4F(int subMeshId, int uvIndex, Vec4F* uvs, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetBlendWeights(int subMeshId, Vec4F* blendWeights, int count);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe void SubMeshSetBlendIndices(int subMeshId, Vec4F* blendIndices, int count);
        #endregion

        #region RenderMesh
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateRenderMesh();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroyRenderMesh(int renderMeshId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void RenderMeshLoadFromMesh(int renderMeshId, int meshId);
        #endregion

        #region RenderWindow
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateRenderWindow(ref Vec2U windowSize, string title);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroyRenderWindow(int resourceId);
        #endregion

        #region EcsRenderScene
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int EcsRenderSceneGetRenderTarget(NativePtr nativeComponentPtr);
        #endregion

        #region Canvas
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void CanvasGetRenderTarget(NativePtr nativeComponentPtr, out int outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void CanvasSetRenderTarget(NativePtr nativeComponentPtr, int value);
        #endregion
    }
}
