using System;
using Maze;

namespace Maze.Core
{
    public class EcsScene
    {
        int m_SceneId = 0;
        public int SceneId => m_SceneId;

        public EcsScene(int sceneId)
        {
            m_SceneId = sceneId;
        }
    }
}
