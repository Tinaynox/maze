using System;

namespace Maze.Core
{
    public struct Entity
    {
        public static readonly Entity Invalid = new Entity(NativePtr.Zero);

        public int Id => GetEntityId();

        public NativePtr NativeEntityPtr { get; private set; }
        public bool Valid => NativeEntityPtr != NativePtr.Zero;

        public bool Active
        {
            get { InternalCalls.EntityGetActive(NativeEntityPtr, out bool active); return active; }
            set { InternalCalls.EntitySetActive(NativeEntityPtr, value); }
        }

        public string Name
        {
            get => GetComponent<Name>()?.Title ?? string.Empty;
        }

        public Entity(NativePtr ptr)
        {
            NativeEntityPtr = ptr;
        }

        public static bool operator ==(Entity value0, Entity value1)
        {
            return value0.Id == value1.Id;
        }

        public static bool operator !=(Entity value0, Entity value1)
        {
            return value0.Id != value1.Id;
        }

        public override bool Equals(object obj)
        {
            if (obj is Entity other)
                return this == other;
            return false;
        }

        public override int GetHashCode() => Id;

        public int GetEntityId()
        {
            return InternalCalls.EntityGetEntityId(NativeEntityPtr);
        }

        public T GetComponent<T>() where T : class
        {
            int componentId = InternalCalls.GetComponentIdByMonoType(typeof(T));

            // Native component wrappers
            if (typeof(NativeComponent).IsAssignableFrom(typeof(T)))
            {
                NativePtr componentPtr = InternalCalls.EntityGetComponent(NativeEntityPtr, componentId);
                if (componentPtr == NativePtr.Zero)
                    return null;

                return (T)Activator.CreateInstance(typeof(T), componentPtr);
            }


            // MonoBehaviour refs
            if (typeof(MonoBehaviour).IsAssignableFrom(typeof(T)))
                return (T)(object)InternalCalls.EntityGetMonoBehaviourComponentObject(NativeEntityPtr, componentId);


            // Not supported type
            return null;
        }

        public T CreateComponent<T>() where T : class
        {
            int componentId = InternalCalls.GetComponentIdByMonoType(typeof(T));

            // Native component
            if (typeof(NativeComponent).IsAssignableFrom(typeof(T)))
            {
                NativePtr componentPtr = InternalCalls.EntityCreateNativeComponent(NativeEntityPtr, componentId);
                if (componentPtr == NativePtr.Zero)
                    return null;

                return (T)Activator.CreateInstance(typeof(T), componentPtr);
            }

            // MonoBehaviour
            if (typeof(MonoBehaviour).IsAssignableFrom(typeof(T)))
                return (T)(object)InternalCalls.EntityCreateMonoBehaviourComponentObject(NativeEntityPtr, componentId);

            return null;
        }

        public T EnsureComponent<T>() where T : class
        {
            T component = GetComponent<T>();
            if (component != null)
                return component;

            return CreateComponent<T>();
        }

        public void Destroy()
        {
            InternalCalls.EntityRemoveFromEcsWorld(NativeEntityPtr);
        }

        public override string ToString()
        {
            return Id.ToString();
        }
    }
}
