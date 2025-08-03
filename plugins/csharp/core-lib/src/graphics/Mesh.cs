using Maze.Core;

namespace Maze.Graphics
{
    public class Mesh : IndexedResource
    {
        public Mesh()
        {
            m_ResourceId = InternalCalls.CreateMesh();
            m_OwnsResourceId = true;
        }

        ~Mesh()
        {
            ReleaseResource();
        }

        public override void ReleaseResource()
        {
            if (m_OwnsResourceId && m_ResourceId != -1)
            {
                InternalCalls.DestroyMesh(m_ResourceId);
                m_ResourceId = -1;
                m_OwnsResourceId = false;
            }
        }

        public void AddSubMesh(SubMesh subMesh)
        {
            InternalCalls.MeshAddSubMesh(m_ResourceId, subMesh.ResourceId);
        }

        public void Clear()
        {
            InternalCalls.MeshClear(m_ResourceId);
        }
    }
}
