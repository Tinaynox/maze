using System;
using System.Runtime.CompilerServices;

namespace Maze.Core
{
    public class InternalCalls
    {
        #region Log
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MazeLog(string _text);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MazeLogWarning(string _text);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void MazeLogError(string _text);
        #endregion

        #region Input
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool GetKeyState(int _keyCode);
        #endregion

        #region Ecs
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int GetComponentIdByMonoType(Type _type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr InstantiateEntity(NativePtr _nativeComponentPtr, AssetUnitId _auid);
        #endregion

        #region Entity
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int EntityGetEntityId(NativePtr _nativeEntityPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr EntityGetComponent(NativePtr _nativeEntityPtr, int _componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static MonoBehaviour EntityGetMonoBehaviourComponentObject(NativePtr _nativeEntityPtr, int _componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void EntityRemoveFromEcsWorld(NativePtr _nativeEntityPtr);
        #endregion

        #region Component
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int ComponentGetFrameNumber(NativePtr _nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int ComponentGetEntityId(NativePtr _nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int ComponentGetComponentId(string _name);
      

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr ComponentGetComponent(NativePtr _nativeComponentPtr, int _componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static MonoBehaviour ComponentGetMonoBehaviourComponentObject(NativePtr _nativeComponentPtr, int _componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr ComponentGetEntity(NativePtr _nativeComponentPtr);
        #endregion

        #region Transform3D
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DTranslate(NativePtr _nativeComponentPtr, Vec3F _delta);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DRotate(NativePtr _nativeComponentPtr, Vec3F _axis, float _angle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetPosition(NativePtr _nativeComponentPtr, out Vec3F _outPosition);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetPosition(NativePtr _nativeComponentPtr, Vec3F _pos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetScale(NativePtr _nativeComponentPtr, out Vec3F _outScale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetScale(NativePtr _nativeComponentPtr, Vec3F _scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetParent(NativePtr _nativeComponentPtr, NativePtr _parentPtr);
        #endregion
    }
}
