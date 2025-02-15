using System;

namespace Maze.Core
{
    public class EcsUtils
    {
        public static object GetComponentByType(
            NativePtr nativePtr,
            Type componentType)
        {
            int componentId = InternalCalls.GetComponentIdByMonoType(componentType);

            // Native component wrappers
            if (typeof(NativeComponent).IsAssignableFrom(componentType))
            {
                NativePtr componentPtr = InternalCalls.ComponentGetComponent(nativePtr, componentId);
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
    }
}
