using Maze.Core;
using System.Runtime.InteropServices;


namespace Maze.Editor
{
    public class EditorHelper
    {
        private EditorHelper() { }

        public static EcsWorld GetEditorMainSceneEcsWorld()
        {
            return new EcsWorld(InternalCalls.GetEditorMainSceneEcsWorld());
        }

        public static EcsScene GetEditorOpenedScene()
        {
            return new EcsScene(InternalCalls.GetEditorOpenedScene());
        }

    }
}