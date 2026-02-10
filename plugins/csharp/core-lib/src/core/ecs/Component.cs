using System;
using System.Runtime.InteropServices;
using Maze;

namespace Maze.Core
{
    public class Component
    {
        public EcsScene Scene => GetScene();

        protected NativePtr NativeComponentPtr { get; set; }

        public int GetEntityId()
        {
            return InternalCalls.ComponentGetEntityId(NativeComponentPtr);
        }

        public Entity GetEntity()
        {
            return new Entity(InternalCalls.ComponentGetEntity(NativeComponentPtr));
        }
        public Entity Entity => GetEntity();

        public T GetComponent<T>() where T : class
        {
            return GetComponentByType(typeof(T)) as T;
        }

        public object GetComponentByType(Type componentType)
        {
            return EcsUtils.GetComponentByType(NativeComponentPtr, componentType);
        }

        public T CreateComponent<T>() where T : class
        {
            return CreateComponent(typeof(T)) as T;
        }

        public object CreateComponent(Type componentType)
        {
            return EcsUtils.CreateComponent(NativeComponentPtr, componentType);
        }

        public T EnsureComponent<T>() where T : class
        {
            T cmp = GetComponent<T>();
            if (cmp != null)
                return cmp;
            return CreateComponent<T>();
        }

        public EcsScene GetScene()
        {
            return new EcsScene(InternalCalls.ComponentGetSceneId(NativeComponentPtr));
        }


        public Entity CreateEntity()
        {
            return new Entity(InternalCalls.CreateEntity(NativeComponentPtr));
        }

        public Entity CreateEntity(string name, Transform2D parent)
        {
            Entity entity = CreateEntity();
            entity.CreateComponent<Name>().Title = name;
            entity.CreateComponent<Transform2D>().SetParent(parent);
            return entity;
        }

        public Entity CreateEntity(string name, Transform3D parent)
        {
            Entity entity = CreateEntity();
            entity.CreateComponent<Name>().Title = name;
            entity.CreateComponent<Transform3D>().SetParent(parent);
            return entity;
        }

        public Entity InstantiateEntity(AssetUnitId auid)
        {
            return new Entity(InternalCalls.InstantiateEntity(NativeComponentPtr, auid));
        }

        public Entity InstantiateEntity(string prefabName)
        {
            AssetUnitId prefabAUID = AssetUnitId.Get(prefabName);
            if (prefabAUID == AssetUnitId.InvalidId)
                return Entity.Invalid;

            return InstantiateEntity(prefabAUID);
        }

        public Entity InstantiateEntity(string prefabName, Transform2D parent)
        {
            Entity entity = InstantiateEntity(prefabName);
            if (!entity.Valid)
                return Entity.Invalid;

            entity.EnsureComponent<Transform2D>().SetParent(parent);
            return entity;
        }

        public Entity InstantiateEntity(string prefabName, Transform3D parent)
        {
            Entity entity = InstantiateEntity(prefabName);
            if (!entity.Valid)
                return Entity.Invalid;

            entity.EnsureComponent<Transform3D>().SetParent(parent);
            return entity;
        }

        public void SendEvent(Entity entity, MonoEvent monoEvent)
        {
            GCHandle handle = GCHandle.Alloc(monoEvent);
            InternalCalls.ComponentSendMonoEvent(NativeComponentPtr, entity.NativeEntityPtr, GCHandle.ToIntPtr(handle));
            handle.Free();
        }

        public void SendEvent(MonoEvent monoEvent) => SendEvent(GetEntity(), monoEvent);

        public void SendEventImmediate(Entity entity, MonoEvent monoEvent)
        {
            GCHandle handle = GCHandle.Alloc(monoEvent);
            InternalCalls.ComponentSendMonoEventImmediate(NativeComponentPtr, entity.NativeEntityPtr, GCHandle.ToIntPtr(handle));
            handle.Free();
        }

        public void SendEventImmediate(MonoEvent monoEvent) => SendEventImmediate(GetEntity(), monoEvent);

        public void BroadcastEvent(MonoEvent monoEvent)
        {
            GCHandle handle = GCHandle.Alloc(monoEvent);
            InternalCalls.ComponentBroadcastMonoEvent(NativeComponentPtr, GCHandle.ToIntPtr(handle));
            handle.Free();
        }

        public void BroadcastEventImmediate(MonoEvent monoEvent)
        {
            GCHandle handle = GCHandle.Alloc(monoEvent);
            InternalCalls.ComponentBroadcastMonoEventImmediate(NativeComponentPtr, GCHandle.ToIntPtr(handle));
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
