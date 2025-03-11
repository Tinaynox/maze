using Maze.Core;
using Maze.Graphics;

namespace Maze.Editor
{
    public class EditorCanvas : MonoBehaviour
    {
        [EntitySystem, EnableInEditor]
        public void OnCreate()
        {
            EcsScene currentScene = GetScene();
            if (currentScene == EditorHelper.GetEditorOpenedScene())
                return;

            RenderTarget sceneRenderTarget = GraphicsHelper.GetRenderTarget(NativeComponentPtr);

            Canvas canvas = GetComponent<Canvas>();
            if (canvas)
                canvas.RenderTarget = sceneRenderTarget.ResourceId;
        }
    }
}