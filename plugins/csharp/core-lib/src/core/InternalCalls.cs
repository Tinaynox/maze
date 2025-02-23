using System;
using System.Runtime.CompilerServices;

namespace Maze.Core
{
    public class InternalCalls
    {
        #region Log
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MazeLog(string text);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MazeLogWarning(string text);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MazeLogError(string text);
        #endregion

        #region Input
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool GetKeyState(int keyCode);
        #endregion

        #region Ecs
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int GetComponentIdByMonoType(Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr InstantiateEntity(NativePtr nativeComponentPtr, AssetUnitId auid);
        #endregion

        #region Entity
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int EntityGetEntityId(NativePtr nativeEntityPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr EntityGetComponent(NativePtr nativeEntityPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static MonoBehaviour EntityGetMonoBehaviourComponentObject(NativePtr nativeEntityPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void EntityRemoveFromEcsWorld(NativePtr nativeEntityPtr);
        #endregion

        #region Component
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int ComponentGetFrameNumber(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int ComponentGetEntityId(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int ComponentGetComponentId(string name);
      

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr ComponentGetComponent(NativePtr nativeComponentPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static MonoBehaviour ComponentGetMonoBehaviourComponentObject(NativePtr nativeComponentPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr ComponentGetEntity(NativePtr nativeComponentPtr);
        #endregion

        #region Transform3D
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DTranslate(NativePtr nativeComponentPtr, Vec3F delta);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DRotate(NativePtr nativeComponentPtr, Vec3F axis, float angle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetPosition(NativePtr nativeComponentPtr, out Vec3F outPosition);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetPosition(NativePtr nativeComponentPtr, Vec3F pos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetRotation(NativePtr nativeComponentPtr, out Quaternion outPosition);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetRotation(NativePtr nativeComponentPtr, Quaternion pos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetScale(NativePtr nativeComponentPtr, out Vec3F outScale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetScale(NativePtr nativeComponentPtr, Vec3F scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetParent(NativePtr nativeComponentPtr, NativePtr parentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetLocalTransform(NativePtr nativeComponentPtr, out TMat outTMat);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetWorldTransform(NativePtr nativeComponentPtr, out TMat outTMat);
        #endregion
    }
}
