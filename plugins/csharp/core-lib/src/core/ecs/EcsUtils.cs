using System;

namespace Maze.Core
{
    public static class EcsUtils
    {
        public static object GetComponentByType(
            NativePtr nativePtr,
            Type componentType)
        {
            int componentId = InternalCalls.GetComponentIdByMonoType(componentType);

            // Native component wrappers
            if (typeof(NativeComponent).IsAssignableFrom(componentType))
            {
                NativePtr componentPtr = InternalCalls.ComponentGetComponentInheritedFrom(nativePtr, componentId);
                if (componentPtr == NativePtr.Zero)
                    return null;

                return Activator.CreateInstance(componentType, componentPtr);
            }

            // MonoBehaviour refs
            if (typeof(MonoBehaviour).IsAssignableFrom(componentType))
                return InternalCalls.ComponentGetMonoBehaviourComponentObject(nativePtr, componentId);

            // Not supported type
            return null;
        }

        public static object CreateComponent(
            NativePtr nativePtr,
            Type componentType)
        {
            int componentId = InternalCalls.GetComponentIdByMonoType(componentType);

            // Native component
            if (typeof(NativeComponent).IsAssignableFrom(componentType))
            {
                NativePtr componentPtr = InternalCalls.ComponentCreateNativeComponent(nativePtr, componentId);
                if (componentPtr == NativePtr.Zero)
                    return null;

                return Activator.CreateInstance(componentType, componentPtr);
            }

            // MonoBehaviour
            if (typeof(MonoBehaviour).IsAssignableFrom(componentType))
                return InternalCalls.ComponentCreateMonoBehaviourComponentObject(nativePtr, componentId);

            // Not supported type
            return null;
        }
    }
}
