using Maze.Core;
using System;
using System.Runtime.CompilerServices;

namespace Maze.Engine
{
    public class InternalCalls
    {
        #region Ecs
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int LoadAssetScene(string sceneName, bool additive, sbyte ecsWorldId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int DestroyScene(int sceneId);
        #endregion

    }
}
