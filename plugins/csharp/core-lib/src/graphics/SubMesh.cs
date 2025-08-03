using Maze.Core;

namespace Maze.Graphics
{
    public enum RenderDrawTopology
    {
        None,
        Points,
        Lines,
        LinesLoop,
        LinesStrip,
        Triangles,
        TrianglesFan,
        Quads
    }

    public class SubMesh : IndexedResource
    {
        public SubMesh()
        {
            m_ResourceId = InternalCalls.CreateSubMesh();
            m_OwnsResourceId = true;
        }

        ~SubMesh()
        {
            ReleaseResource();
        }

        public override void ReleaseResource()
        {
            if (m_OwnsResourceId && m_ResourceId != -1)
            {
                InternalCalls.DestroySubMesh(m_ResourceId);
                m_ResourceId = -1;
                m_OwnsResourceId = false;
            }
        }

        public void SetRenderDrawTopology(RenderDrawTopology topology)
        {
            InternalCalls.SubMeshSetRenderDrawTopology(m_ResourceId, (int)topology);
        }

        public unsafe void SetIndices(ushort[] indices)
        {
            fixed (ushort* ptr = indices)
                InternalCalls.SubMeshSetIndicesU16(m_ResourceId, ptr, indices.Length);
        }

        public unsafe void SetIndices(uint[] indices)
        {
            fixed (uint* ptr = indices)
                InternalCalls.SubMeshSetIndicesU32(m_ResourceId, ptr, indices.Length);
        }

        public unsafe void SetPositions(Vec3F[] positions)
        {
            fixed (Vec3F* ptr = positions)
                InternalCalls.SubMeshSetPositions(m_ResourceId, ptr, positions.Length);
        }

        public unsafe void SetNormals(Vec3F[] normals)
        {
            fixed (Vec3F* ptr = normals)
                InternalCalls.SubMeshSetNormals(m_ResourceId, ptr, normals.Length);
        }

        public unsafe void SetTangents(Vec3F[] tangents)
        {
            fixed (Vec3F* ptr = tangents)
                InternalCalls.SubMeshSetTangents(m_ResourceId, ptr, tangents.Length);
        }

        public unsafe void SetBitangents(Vec3F[] bitangents)
        {
            fixed (Vec3F* ptr = bitangents)
                InternalCalls.SubMeshSetBitangents(m_ResourceId, ptr, bitangents.Length);
        }

        public unsafe void SetColors(Vec4F[] colors)
        {
            fixed (Vec4F* ptr = colors)
                InternalCalls.SubMeshSetColors(m_ResourceId, ptr, colors.Length);
        }

        public unsafe void SetTexCoords(int uvIndex, Vec2F[] uvs)
        {
            fixed (Vec2F* ptr = uvs)
                InternalCalls.SubMeshSetTexCoordsVec2F(m_ResourceId, uvIndex, ptr, uvs.Length);
        }

        public unsafe void SetTexCoords(int uvIndex, Vec4F[] uvs)
        {
            fixed (Vec4F* ptr = uvs)
                InternalCalls.SubMeshSetTexCoordsVec4F(m_ResourceId, uvIndex, ptr, uvs.Length);
        }

        public unsafe void SetBlendWeights(Vec4F[] blendWeights)
        {
            fixed (Vec4F* ptr = blendWeights)
                InternalCalls.SubMeshSetBlendWeights(m_ResourceId, ptr, blendWeights.Length);
        }

        public unsafe void SetBlendIndices(Vec4F[] blendIndices)
        {
            fixed (Vec4F* ptr = blendIndices)
                InternalCalls.SubMeshSetBlendIndices(m_ResourceId, ptr, blendIndices.Length);
        }
    }
}
