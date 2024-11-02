using System;
using System.Runtime.CompilerServices;

namespace Maze.Core
{
    public class InternalCalls
    {
#region Log
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MazeLog(string _text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MazeLogWarning(string _text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MazeLogError(string _text);
#endregion

#region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool GetKeyState(int _keyCode);
        #endregion

#region Ecs
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static int GetFrameNumber(IntPtr _nativeComponentPtr);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static int GetEntityId(IntPtr _nativeComponentPtr);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static int GetComponentId(string _name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static int GetComponentIdByMonoType(Type _type);      

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static IntPtr GetComponent(IntPtr _nativeComponentPtr, int _componentId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static MonoBehaviour GetMonoBehaviourComponentObject(IntPtr _nativeComponentPtr, int _componentId);
        #endregion

        #region Transform3D
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Transform3DTranslate(IntPtr _nativeComponentPtr, Vec3F _delta);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Transform3DRotate(IntPtr _nativeComponentPtr, Vec3F _axis, float _angle);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Transform3DGetPosition(IntPtr _nativeComponentPtr, out Vec3F _delta);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Transform3DSetPosition(IntPtr _nativeComponentPtr, Vec3F _delta);
#endregion
    }
}
