using Maze.Core;

namespace Maze.Graphics
{
    public class AttachToSceneRenderTarget : MonoBehaviour
    {
        [EntitySystem]
        public void OnCreate()
        {
            RenderTarget sceneRenderTarget = GraphicsHelper.GetRenderTarget(NativeComponentPtr);

            Camera3D camera = GetComponent<Camera3D>();
            if (camera)
                camera.RenderTarget = sceneRenderTarget.ResourceId;
        }
    }
}
