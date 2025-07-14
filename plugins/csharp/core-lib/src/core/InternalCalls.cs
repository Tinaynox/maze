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

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool GetCursorButtonState(int cursorId, int buttonIndex);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetCursorPosition(int cursorId, out Vec2F outPosition);
        #endregion

        #region Ecs
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int GetComponentIdByMonoType(Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr InstantiateEntity(NativePtr nativeComponentPtr, AssetUnitId auid);
        #endregion

        #region EcsScene
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static string EcsSceneGetAssetFilePath(int sceneId);
        #endregion

        #region Entity
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr CreateEntity(NativePtr nativeEntityPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int EntityGetEntityId(NativePtr nativeEntityPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr EntityGetComponent(NativePtr nativeEntityPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static MonoBehaviour EntityGetMonoBehaviourComponentObject(NativePtr nativeEntityPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr EntityCreateNativeComponent(NativePtr nativeEntityPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static MonoBehaviour EntityCreateMonoBehaviourComponentObject(NativePtr nativeEntityPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void EntityRemoveFromEcsWorld(NativePtr nativeEntityPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void EntityGetActive(NativePtr nativeEntityPtr, out bool outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void EntitySetActive(NativePtr nativeEntityPtr, bool value);
        #endregion

        #region Component
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int ComponentGetFrameNumber(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int ComponentGetEntityId(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int ComponentGetSceneId(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int ComponentGetComponentId(string name);
      

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr ComponentGetComponent(NativePtr nativeComponentPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr ComponentGetComponentInheritedFrom(NativePtr nativeComponentPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static MonoBehaviour ComponentGetMonoBehaviourComponentObject(NativePtr nativeComponentPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr ComponentCreateNativeComponent(NativePtr nativeComponentPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static MonoBehaviour ComponentCreateMonoBehaviourComponentObject(NativePtr nativeComponentPtr, int componentId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static NativePtr ComponentGetEntity(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void ComponentSendMonoEvent(NativePtr nativeComponentPtr, NativePtr receiverEntity, IntPtr monoEvent);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void ComponentSendMonoEventImmediate(NativePtr nativeComponentPtr, NativePtr receiverEntity, IntPtr monoEvent);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void ComponentBroadcastMonoEvent(NativePtr nativeComponentPtr, IntPtr monoEvent);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void ComponentBroadcastMonoEventImmediate(NativePtr nativeComponentPtr, IntPtr monoEvent);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool ComponentIsEditorMode(NativePtr nativeComponentPtr);
        #endregion

        #region Transform2D
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int Transform2DGetChildCount(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform2DGetChild(NativePtr nativeComponentPtr, int index, out NativePtr outChild);
        #endregion


        #region Transform3D
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DTranslate(NativePtr nativeComponentPtr, ref Vec3F delta);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DRotate(NativePtr nativeComponentPtr, ref Vec3F axis, float angle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetPosition(NativePtr nativeComponentPtr, out Vec3F outPosition);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetPosition(NativePtr nativeComponentPtr, ref Vec3F pos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetWorldPosition(NativePtr nativeComponentPtr, out Vec3F outPosition);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetRotation(NativePtr nativeComponentPtr, out Quaternion outPosition);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetRotation(NativePtr nativeComponentPtr, ref Quaternion pos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetScale(NativePtr nativeComponentPtr, out Vec3F outScale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetScale(NativePtr nativeComponentPtr, ref Vec3F scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetParent(NativePtr nativeComponentPtr, NativePtr parentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetLocalTransform(NativePtr nativeComponentPtr, out TMat outTMat);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetLocalTransform(NativePtr nativeComponentPtr, TMat tm);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetWorldTransform(NativePtr nativeComponentPtr, out TMat outTMat);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetWorldTransform(NativePtr nativeComponentPtr, TMat tm);
        #endregion

        #region Name
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static string NameGetName(NativePtr nativeComponentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void NameSetName(NativePtr nativeComponentPtr, string name);
        #endregion

        #region AssetFile
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool AssetFileIdIsValid(uint assetFileId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static unsafe bool AssetFileReadAsDataBlock(
            uint assetFileId,
            out uint size,
            out byte* bytes);
        #endregion
    }
}
