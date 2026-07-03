using System;

namespace Maze.Core
{
    // A live view over all entities that have every required component
    // and none of the forbidden ones (both native C++ components and
    // C# MonoBehaviours can be mixed freely).
    //
    // Request it via Component.RequestSample(...), then Query/FindQuery.
    // The native sample stays alive until Release() is called (or the
    // mono domain is reloaded/shut down).
    public class EntitiesSample
    {
        public const int InvalidHandle = -1;

        static readonly IntPtr[] s_EmptyEntityPtrs = new IntPtr[0];

        int m_Handle = InvalidHandle;

        public bool Valid => m_Handle != InvalidHandle;

        internal EntitiesSample(int handle)
        {
            m_Handle = handle;
        }

        protected IntPtr[] FetchEntityPtrs()
        {
            if (!Valid)
                return s_EmptyEntityPtrs;

            return InternalCalls.EcsSampleGetEntities(m_Handle) ?? s_EmptyEntityPtrs;
        }

        public Entity[] GetEntities()
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();

            Entity[] entities = new Entity[entityPtrs.Length];
            for (int i = 0; i < entityPtrs.Length; ++i)
                entities[i] = new Entity(new NativePtr(entityPtrs[i]));

            return entities;
        }

        public void Query(Action<Entity> func)
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();
            for (int i = 0; i < entityPtrs.Length; ++i)
            {
                Entity entity = new Entity(new NativePtr(entityPtrs[i]));
                if (entity.Valid)
                    func(entity);
            }
        }

        public Entity FindQuery(Func<Entity, bool> func)
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();
            for (int i = 0; i < entityPtrs.Length; ++i)
            {
                Entity entity = new Entity(new NativePtr(entityPtrs[i]));
                if (entity.Valid && func(entity))
                    return entity;
            }

            return Entity.Invalid;
        }

        public void Release()
        {
            if (!Valid)
                return;

            InternalCalls.EcsSampleRelease(m_Handle);
            m_Handle = InvalidHandle;
        }
    }


    public class EntitiesSample<T0> : EntitiesSample
        where T0 : class
    {
        internal EntitiesSample(int handle)
            : base(handle)
        {
        }

        public void Query(Action<Entity, T0> func)
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();
            for (int i = 0; i < entityPtrs.Length; ++i)
            {
                Entity entity = new Entity(new NativePtr(entityPtrs[i]));
                if (!entity.Valid)
                    continue;

                T0 component0 = entity.GetComponent<T0>();
                if (component0 == null)
                    continue;

                func(entity, component0);
            }
        }

        public Entity FindQuery(Func<Entity, T0, bool> func)
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();
            for (int i = 0; i < entityPtrs.Length; ++i)
            {
                Entity entity = new Entity(new NativePtr(entityPtrs[i]));
                if (!entity.Valid)
                    continue;

                T0 component0 = entity.GetComponent<T0>();
                if (component0 == null)
                    continue;

                if (func(entity, component0))
                    return entity;
            }

            return Entity.Invalid;
        }
    }


    public class EntitiesSample<T0, T1> : EntitiesSample
        where T0 : class
        where T1 : class
    {
        internal EntitiesSample(int handle)
            : base(handle)
        {
        }

        public void Query(Action<Entity, T0, T1> func)
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();
            for (int i = 0; i < entityPtrs.Length; ++i)
            {
                Entity entity = new Entity(new NativePtr(entityPtrs[i]));
                if (!entity.Valid)
                    continue;

                T0 component0 = entity.GetComponent<T0>();
                T1 component1 = entity.GetComponent<T1>();
                if (component0 == null || component1 == null)
                    continue;

                func(entity, component0, component1);
            }
        }

        public Entity FindQuery(Func<Entity, T0, T1, bool> func)
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();
            for (int i = 0; i < entityPtrs.Length; ++i)
            {
                Entity entity = new Entity(new NativePtr(entityPtrs[i]));
                if (!entity.Valid)
                    continue;

                T0 component0 = entity.GetComponent<T0>();
                T1 component1 = entity.GetComponent<T1>();
                if (component0 == null || component1 == null)
                    continue;

                if (func(entity, component0, component1))
                    return entity;
            }

            return Entity.Invalid;
        }
    }


    public class EntitiesSample<T0, T1, T2> : EntitiesSample
        where T0 : class
        where T1 : class
        where T2 : class
    {
        internal EntitiesSample(int handle)
            : base(handle)
        {
        }

        public void Query(Action<Entity, T0, T1, T2> func)
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();
            for (int i = 0; i < entityPtrs.Length; ++i)
            {
                Entity entity = new Entity(new NativePtr(entityPtrs[i]));
                if (!entity.Valid)
                    continue;

                T0 component0 = entity.GetComponent<T0>();
                T1 component1 = entity.GetComponent<T1>();
                T2 component2 = entity.GetComponent<T2>();
                if (component0 == null || component1 == null || component2 == null)
                    continue;

                func(entity, component0, component1, component2);
            }
        }

        public Entity FindQuery(Func<Entity, T0, T1, T2, bool> func)
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();
            for (int i = 0; i < entityPtrs.Length; ++i)
            {
                Entity entity = new Entity(new NativePtr(entityPtrs[i]));
                if (!entity.Valid)
                    continue;

                T0 component0 = entity.GetComponent<T0>();
                T1 component1 = entity.GetComponent<T1>();
                T2 component2 = entity.GetComponent<T2>();
                if (component0 == null || component1 == null || component2 == null)
                    continue;

                if (func(entity, component0, component1, component2))
                    return entity;
            }

            return Entity.Invalid;
        }
    }


    public class EntitiesSample<T0, T1, T2, T3> : EntitiesSample
        where T0 : class
        where T1 : class
        where T2 : class
        where T3 : class
    {
        internal EntitiesSample(int handle)
            : base(handle)
        {
        }

        public void Query(Action<Entity, T0, T1, T2, T3> func)
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();
            for (int i = 0; i < entityPtrs.Length; ++i)
            {
                Entity entity = new Entity(new NativePtr(entityPtrs[i]));
                if (!entity.Valid)
                    continue;

                T0 component0 = entity.GetComponent<T0>();
                T1 component1 = entity.GetComponent<T1>();
                T2 component2 = entity.GetComponent<T2>();
                T3 component3 = entity.GetComponent<T3>();
                if (component0 == null || component1 == null || component2 == null || component3 == null)
                    continue;

                func(entity, component0, component1, component2, component3);
            }
        }

        public Entity FindQuery(Func<Entity, T0, T1, T2, T3, bool> func)
        {
            IntPtr[] entityPtrs = FetchEntityPtrs();
            for (int i = 0; i < entityPtrs.Length; ++i)
            {
                Entity entity = new Entity(new NativePtr(entityPtrs[i]));
                if (!entity.Valid)
                    continue;

                T0 component0 = entity.GetComponent<T0>();
                T1 component1 = entity.GetComponent<T1>();
                T2 component2 = entity.GetComponent<T2>();
                T3 component3 = entity.GetComponent<T3>();
                if (component0 == null || component1 == null || component2 == null || component3 == null)
                    continue;

                if (func(entity, component0, component1, component2, component3))
                    return entity;
            }

            return Entity.Invalid;
        }
    }
}
