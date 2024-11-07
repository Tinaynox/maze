using System;

namespace Maze.Core
{
    public class EcsUtils
    {
        public static object GetComponentByType(
            NativePtr _nativePtr,
            Type _componentType)
        {
            Debug.Log("1");
            int componentId = InternalCalls.GetComponentIdByMonoType(_componentType);

            Debug.Log("A");
            Debug.Log($"_nativePtr={_nativePtr}");
            Debug.Log("B");
            Debug.Log($"_componentType={_componentType}");

            // Native component wrappers
            if (typeof(NativeComponent).IsAssignableFrom(_componentType))
            {
                Debug.Log("33");
                NativePtr componentPtr = InternalCalls.ComponentGetComponent(_nativePtr, componentId);
                if (componentPtr == NativePtr.Zero)
                    return null;

                Debug.Log("3");
                return Activator.CreateInstance(_componentType, componentPtr);
            }

            Debug.Log("4");

            // MonoBehaviour refs
            if (typeof(MonoBehaviour).IsAssignableFrom(_componentType))
                return InternalCalls.ComponentGetMonoBehaviourComponentObject(_nativePtr, componentId);

            Debug.Log("5");

            // Not supported type
            return null;
        }
    }
}
