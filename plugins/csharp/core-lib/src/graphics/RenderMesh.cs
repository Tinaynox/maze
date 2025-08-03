using Maze.Core;

namespace Maze.Graphics
{
    public class RenderMesh : IndexedResource
    {
        public RenderMesh()
        {
            m_ResourceId = InternalCalls.CreateRenderMesh();
            m_OwnsResourceId = true;
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
            if (m_OwnsResourceId && m_ResourceId != -1)
            {
                InternalCalls.DestroyRenderMesh(m_ResourceId);
                m_ResourceId = -1;
                m_OwnsResourceId = false;
            }
        }

        public void LoadFromMesh(Mesh mesh)
        {
            InternalCalls.RenderMeshLoadFromMesh(m_ResourceId, mesh.ResourceId);
        }

        public void Clear()
        {
            InternalCalls.RenderMeshClear(m_ResourceId);
        }
    }
}
