using Maze.Core;
using Maze.Graphics;
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

        public static RenderWindow GetEditorMainRenderWindow()
        {
            return new RenderWindow(InternalCalls.GetEditorMainRenderWindow());
        }
    }
}