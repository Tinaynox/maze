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
        #endregion

        #region Mesh
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateMesh();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroyMesh(int resourceId);
        #endregion

        #region SubMesh
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateSubMesh();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroySubMesh(int resourceId);
        #endregion

        #region RenderMesh
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int CreateRenderMesh();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroyRenderMesh(int resourceId);
        #endregion
    }
}
