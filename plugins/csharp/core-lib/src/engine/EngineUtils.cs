using System;
using Maze.Core;

namespace Maze.Engine
{
    public class EngineUtils
    {
        public static EcsScene LoadAssetScene(
            string sceneName,
            bool additive = true,
            sbyte ecsWorldId = -1)
        {
            int sceneId = InternalCalls.LoadAssetScene(sceneName, additive, ecsWorldId);
            return new EcsScene(sceneId);
        }

        public static EcsScene LoadAssetScene(
            string sceneName,
            bool additive,
            EcsWorld ecsWorld)
        {
            return LoadAssetScene(sceneName, additive, ecsWorld.Id);
        }

        public static void DestroyScene(EcsScene scene)
        {
            InternalCalls.DestroyScene(scene.Id);
        }
    }
}
