using Maze.Core;

namespace Maze.Graphics
{
    public class RenderMesh : IndexedResource
    {
        public RenderMesh()
        {
            m_ResourceId = InternalCalls.CreateRenderMesh();
        }

        public RenderMesh(int resourceId)
        {
            m_ResourceId = resourceId;
        }

        ~RenderMesh()
        {
            ReleaseResource();
        }

        public override void ReleaseResource()
        {
            if (m_ResourceId != -1)
            {
                InternalCalls.DestroyRenderMesh(m_ResourceId);
                m_ResourceId = -1;
            }
        }

        public void LoadFromMesh(Mesh mesh)
        {
            InternalCalls.RenderMeshLoadFromMesh(m_ResourceId, mesh.ResourceId);
        }
    }
}
