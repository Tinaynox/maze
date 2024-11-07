using System;

namespace Maze.Core
{
    public class EcsUtils
    {
        public static object GetComponentByType(
            NativePtr _nativePtr,
            Type _componentType)
        {
            int componentId = InternalCalls.GetComponentIdByMonoType(_componentType);

            // Native component wrappers
            if (typeof(NativeComponent).IsAssignableFrom(_componentType))
            {
                NativePtr componentPtr = InternalCalls.ComponentGetComponent(_nativePtr, componentId);
                if (componentPtr == NativePtr.Zero)
                    return null;

                return Activator.CreateInstance(_componentType, componentPtr);
            }

            // MonoBehaviour refs
            if (typeof(MonoBehaviour).IsAssignableFrom(_componentType))
                return InternalCalls.ComponentGetMonoBehaviourComponentObject(_nativePtr, componentId);

            // Not supported type
            return null;
        }
    }
}
