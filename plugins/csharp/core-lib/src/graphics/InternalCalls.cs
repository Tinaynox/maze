using Maze.Core;
using System;
using System.Runtime.CompilerServices;

namespace Maze.Graphics
{
    public class InternalCalls
    {
        #region MeshRenderer
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MeshRendererSetMaterial(NativePtr _nativeComponentPtr, AssetUnitId _material);
        #endregion

        #region Camera3D
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetAspectRatio(NativePtr _nativeComponentPtr, out float _outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetOrthographicSize(NativePtr _nativeComponentPtr, out float _outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetOrthographicSize(NativePtr _nativeComponentPtr, float _value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetNearZ(NativePtr _nativeComponentPtr, out float _outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetNearZ(NativePtr _nativeComponentPtr, float _value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetFarZ(NativePtr _nativeComponentPtr, out float _outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetFarZ(NativePtr _nativeComponentPtr, float _value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DGetRenderMask(NativePtr _nativeComponentPtr, out int _outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Camera3DSetRenderMask(NativePtr _nativeComponentPtr, int _value);
        #endregion
    }
}
