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
    }
}
