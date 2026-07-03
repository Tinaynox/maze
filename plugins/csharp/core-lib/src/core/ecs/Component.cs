using System;
using System.Runtime.InteropServices;
using Maze;

namespace Maze.Core
{
    [Flags]
    public enum ComponentFlags : byte
    {
        None = 0,
        SerializationDisabled = 1 << 0,
    }

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
            // handle.Free(); // We free it in MonoEvent::~MonoEvent
        }

        public void SendEvent(MonoEvent monoEvent) => SendEvent(GetEntity(), monoEvent);

        public void SendEventImmediate(Entity entity, MonoEvent monoEvent)
        {
            GCHandle handle = GCHandle.Alloc(monoEvent);
            InternalCalls.ComponentSendMonoEventImmediate(NativeComponentPtr, entity.NativeEntityPtr, GCHandle.ToIntPtr(handle));
            // handle.Free(); // We free it in MonoEvent::~MonoEvent
        }

        public void SendEventImmediate(MonoEvent monoEvent) => SendEventImmediate(GetEntity(), monoEvent);

        public void BroadcastEvent(MonoEvent monoEvent)
        {
            GCHandle handle = GCHandle.Alloc(monoEvent);
            InternalCalls.ComponentBroadcastMonoEvent(NativeComponentPtr, GCHandle.ToIntPtr(handle));
            // handle.Free(); // We free it in MonoEvent::~MonoEvent
        }

        public void BroadcastEventImmediate(MonoEvent monoEvent)
        {
            GCHandle handle = GCHandle.Alloc(monoEvent);
            InternalCalls.ComponentBroadcastMonoEventImmediate(NativeComponentPtr, GCHandle.ToIntPtr(handle));
            // handle.Free(); // We free it in MonoEvent::~MonoEvent
        }

        public void SetFlag(ComponentFlags flag, bool value)
        {
            InternalCalls.ComponentSetFlag(NativeComponentPtr, (byte)flag, value);
        }

        public bool GetFlag(ComponentFlags flag)
        {
            return InternalCalls.ComponentGetFlag(NativeComponentPtr, (byte)flag);
        }

        public Entity GetEntity(int eid)
        {
            return new Entity(InternalCalls.ComponentGetEntityById(NativeComponentPtr, eid));
        }


        int RequestSampleHandle(Type[] required, Type[] forbidden, bool includeInactive)
        {
            int[] requiredIds = new int[required != null ? required.Length : 0];
            for (int i = 0; i < requiredIds.Length; ++i)
                requiredIds[i] = InternalCalls.GetComponentIdByMonoType(required[i]);

            int[] forbiddenIds = new int[forbidden != null ? forbidden.Length : 0];
            for (int i = 0; i < forbiddenIds.Length; ++i)
                forbiddenIds[i] = InternalCalls.GetComponentIdByMonoType(forbidden[i]);

            byte flags = (byte)(includeInactive ? 1 : 0); // EntitiesSampleFlags::IncludeInactive

            return InternalCalls.EcsSampleRequest(NativeComponentPtr, requiredIds, forbiddenIds, flags);
        }

        public EntitiesSample RequestSample(Type[] required, Type[] forbidden = null, bool includeInactive = false)
        {
            int handle = RequestSampleHandle(required, forbidden, includeInactive);
            if (handle == EntitiesSample.InvalidHandle)
                return null;

            return new EntitiesSample(handle);
        }

        public EntitiesSample<T0> RequestSample<T0>(params Type[] forbidden)
            where T0 : class
        {
            int handle = RequestSampleHandle(new Type[] { typeof(T0) }, forbidden, false);
            return handle != EntitiesSample.InvalidHandle ? new EntitiesSample<T0>(handle) : null;
        }

        public EntitiesSample<T0, T1> RequestSample<T0, T1>(params Type[] forbidden)
            where T0 : class where T1 : class
        {
            int handle = RequestSampleHandle(new Type[] { typeof(T0), typeof(T1) }, forbidden, false);
            return handle != EntitiesSample.InvalidHandle ? new EntitiesSample<T0, T1>(handle) : null;
        }

        public EntitiesSample<T0, T1, T2> RequestSample<T0, T1, T2>(params Type[] forbidden)
            where T0 : class where T1 : class where T2 : class
        {
            int handle = RequestSampleHandle(new Type[] { typeof(T0), typeof(T1), typeof(T2) }, forbidden, false);
            return handle != EntitiesSample.InvalidHandle ? new EntitiesSample<T0, T1, T2>(handle) : null;
        }

        public EntitiesSample<T0, T1, T2, T3> RequestSample<T0, T1, T2, T3>(params Type[] forbidden)
            where T0 : class where T1 : class where T2 : class where T3 : class
        {
            int handle = RequestSampleHandle(new Type[] { typeof(T0), typeof(T1), typeof(T2), typeof(T3) }, forbidden, false);
            return handle != EntitiesSample.InvalidHandle ? new EntitiesSample<T0, T1, T2, T3>(handle) : null;
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
