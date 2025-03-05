using System;
using Maze;

namespace Maze.Core
{
    public struct EcsScene
    {
        readonly int m_Id;
        public int Id => m_Id;
        public static EcsScene Invalid => new EcsScene(-1);

        public EcsScene(int sceneId = -1)
        {
            m_Id = sceneId;
        }
    }
}
