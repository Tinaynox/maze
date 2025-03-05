using System;
using Maze.Core;

namespace Maze.Engine
{
    public class EngineUtils
    {
        public static EcsScene LoadAssetScene(string sceneName, bool additive = true)
        {
            int sceneId = InternalCalls.LoadAssetScene(sceneName, additive);
            return new EcsScene(sceneId);
        }
    }
}
