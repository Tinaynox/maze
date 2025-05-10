using Maze.Core;
using System;
using System.Runtime.CompilerServices;

namespace Maze.Engine
{
    public static class InternalCalls
    {
        #region Ecs
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int LoadAssetScene(string sceneName, int resourceId, bool additive, sbyte ecsWorldId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int DestroyScene(int sceneId);
        #endregion

        #region MainRenderWindow
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool IsEngineMainRenderWindowFocused();
        #endregion

    }
}
