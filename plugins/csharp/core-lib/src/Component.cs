using System;
using Maze;

namespace Maze
{
    public class Component
    {
        protected IntPtr nativeComponentPtr { get; set; }

        public int GetEntityId()
        {
            return InternalCalls.GetEntityId(nativeComponentPtr);
        }

        public T GetComponent<T>() where T : class
        {
            int componentId = InternalCalls.GetComponentIdByMonoType(typeof(T));

            // Native component wrappers
            if (typeof(NativeComponent).IsAssignableFrom(typeof(T)))
            {
                IntPtr componentPtr = InternalCalls.GetComponent(nativeComponentPtr, componentId);
                if (componentPtr == IntPtr.Zero)
                    return default(T);

                return (T)Activator.CreateInstance(typeof(T), componentPtr);
            }


            // MonoBehaviour refs
            if (typeof(MonoBehaviour).IsAssignableFrom(typeof(T)))
                return (T)(object)InternalCalls.GetMonoBehaviourComponentObject(nativeComponentPtr, componentId);


            // Not supported type
            return default(T);
        }
    }
}
