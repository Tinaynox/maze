using System;
using System.Runtime.InteropServices;
using Maze;

namespace Maze.Core
{
    public class Component
    {
        protected NativePtr NativeComponentPtr { get; set; }

        public int GetEntityId()
        {
            return InternalCalls.ComponentGetEntityId(NativeComponentPtr);
        }

        public Entity GetEntity()
        {
            return new Entity(InternalCalls.ComponentGetEntity(NativeComponentPtr));
        }

        public T GetComponent<T>() where T : class
        {
            return GetComponentByType(typeof(T)) as T;
        }

        public object GetComponentByType(Type componentType)
        {
            return EcsUtils.GetComponentByType(NativeComponentPtr, componentType);
        }

        public EcsScene GetScene()
        {
            return new EcsScene(InternalCalls.ComponentGetSceneId(NativeComponentPtr));
        }


        public Entity CreateEntity()
        {
            return new Entity(InternalCalls.CreateEntity(NativeComponentPtr));
        }

        public Entity InstantiateEntity(AssetUnitId auid)
        {
            return new Entity(InternalCalls.InstantiateEntity(NativeComponentPtr, auid));
        }

        public void SendEvent(Entity entity, MonoEvent monoEvent)
        {
            GCHandle handle = GCHandle.Alloc(monoEvent);
            InternalCalls.ComponentSendMonoEvent(NativeComponentPtr, entity.NativeEntityPtr, GCHandle.ToIntPtr(handle));
            handle.Free();
        }

        public void BroadcastEvent(MonoEvent monoEvent)
        {
            GCHandle handle = GCHandle.Alloc(monoEvent);
            InternalCalls.ComponentBroadcastMonoEvent(NativeComponentPtr, GCHandle.ToIntPtr(handle));
            handle.Free();
        }

        public bool IsEditorMode()
        {
            return InternalCalls.ComponentIsEditorMode(NativeComponentPtr);
        }

        public static bool operator !(Component instance)
        {
            return instance.NativeComponentPtr == NativePtr.Zero;
        }

        public static bool operator true(Component instance)
        {
            return instance.NativeComponentPtr != NativePtr.Zero;
        }

        public static bool operator false(Component instance)
        {
            return instance.NativeComponentPtr == NativePtr.Zero;
        }
    }
}
