using System;
using Maze;

namespace Maze.Core
{
    public struct EcsWorld
    {
        readonly sbyte m_Id;
        public sbyte Id => m_Id;
        public static EcsWorld Invalid => new EcsWorld(-1);

        public EcsWorld(sbyte ecsWorldId = -1)
        {
            m_Id = ecsWorldId;
        }

        public override bool Equals(object obj)
        {
            return obj is EcsWorld other && m_Id == other.m_Id;
        }

        public override int GetHashCode()
        {
            return m_Id.GetHashCode();
        }

        public static bool operator ==(EcsWorld value0, EcsWorld value1)
        {
            return value0.m_Id == value1.m_Id;
        }

        public static bool operator !=(EcsWorld value0, EcsWorld value1)
        {
            return value0.m_Id != value1.m_Id;
        }
    }
}
