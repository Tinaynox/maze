using System;
using Maze.Core;
using Maze.Graphics;

namespace Maze.Engine
{
    public class EngineHelper
    {
        public static EcsScene LoadAssetScene(
            string sceneName,
            RenderTarget renderTarget,
            bool additive = true,
            sbyte ecsWorldId = -1)
        {
            int sceneId = InternalCalls.LoadAssetScene(sceneName, renderTarget.ResourceId, additive, ecsWorldId);
            return new EcsScene(sceneId);
        }

        public static EcsScene LoadAssetScene(
            string sceneName,
            RenderTarget renderTarget,
            bool additive,
            EcsWorld ecsWorld)
        {
            return LoadAssetScene(sceneName, renderTarget, additive, ecsWorld.Id);
        }

        public static EcsScene LoadAssetScene(
            string sceneName,
            bool additive = true,
            sbyte ecsWorldId = -1)
        {
            int sceneId = InternalCalls.LoadAssetScene(sceneName, -1, additive, ecsWorldId);
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
