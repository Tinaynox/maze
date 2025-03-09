using Maze.Core;

namespace Maze.Graphics
{
    public class AttachToSceneRenderTarget : MonoBehaviour
    {
        [EntitySystem]
        public void OnCreate()
        {
            RenderTarget sceneRenderTarget = new RenderTarget(InternalCalls.EcsRenderSceneGetRenderTarget(NativeComponentPtr));

            Camera3D camera = GetComponent<Camera3D>();
            if (camera)
                camera.RenderTarget = sceneRenderTarget.ResourceId;
        }
    }
}
