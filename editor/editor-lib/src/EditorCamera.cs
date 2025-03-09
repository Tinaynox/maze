using Maze.Core;
using Maze.Graphics;

namespace Maze.Editor
{
    public class EditorCamera : MonoBehaviour
    {
        [EntitySystem, EnableInEditor]
        public void OnCreate()
        {
            EcsScene currentScene = GetScene();
            if (currentScene == EditorHelper.GetEditorOpenedScene())
                return;

            RenderTarget sceneRenderTarget = GraphicsHelper.GetRenderTarget(NativeComponentPtr);

            Camera3D camera = GetComponent<Camera3D>();
            if (camera)
                camera.RenderTarget = sceneRenderTarget.ResourceId;
        }
    }
}