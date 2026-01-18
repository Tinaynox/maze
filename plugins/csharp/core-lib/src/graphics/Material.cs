using Maze.Core;

namespace Maze.Graphics
{
    public class Material : IndexedResource
    {
        public Material(int resourceId = -1)
        {
            m_ResourceId = resourceId;
        }

        ~Material()
        {
            ReleaseResource();
        }

        public static Material Get(int resourceId)
        {
            if (InternalCalls.MaterialIsValid(resourceId))
                return new Material(resourceId);

            return null;
        }

        public override void ReleaseResource()
        {
            if (m_OwnsResourceId && m_ResourceId != -1)
            {
                InternalCalls.DestroyMaterial(m_ResourceId);
                m_ResourceId = -1;
                m_OwnsResourceId = false;
            }
        }


        public Material Clone()
        {
            int resourceId = InternalCalls.MaterialCreateCopy(m_ResourceId);
            Material newMaterial = new Material(resourceId);
            newMaterial.m_OwnsResourceId = true;
            return newMaterial;
        }


        public int EnsureUniformIndex(string name)
        {
            return InternalCalls.MaterialEnsureUniformIndex(m_ResourceId, name);
        }

        public void SetUniform(int uniformIndex, int value) { InternalCalls.MaterialUniformSetS32(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, float value) { InternalCalls.MaterialUniformSetF32(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, double value) { InternalCalls.MaterialUniformSetF64(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, bool value) { InternalCalls.MaterialUniformSetBool(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec2F value) { InternalCalls.MaterialUniformSetVec2F(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec3F value) { InternalCalls.MaterialUniformSetVec3F(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec4F value) { InternalCalls.MaterialUniformSetVec4F(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec2S value) { InternalCalls.MaterialUniformSetVec2S(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec3S value) { InternalCalls.MaterialUniformSetVec3S(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec4S value) { InternalCalls.MaterialUniformSetVec4S(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec2U value) { InternalCalls.MaterialUniformSetVec2U(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec3U value) { InternalCalls.MaterialUniformSetVec3U(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec4U value) { InternalCalls.MaterialUniformSetVec4U(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec2B value) { InternalCalls.MaterialUniformSetVec2B(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec3B value) { InternalCalls.MaterialUniformSetVec3B(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Vec4B value) { InternalCalls.MaterialUniformSetVec4B(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Mat3F value) { InternalCalls.MaterialUniformSetMat3F(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Mat4F value) { InternalCalls.MaterialUniformSetMat4F(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, TMat value) { InternalCalls.MaterialUniformSetTMat(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, ColorF128 value) { InternalCalls.MaterialUniformSetColorF128(m_ResourceId, uniformIndex, ref value); }
        public void SetUniform(int uniformIndex, Texture2D value) { InternalCalls.MaterialUniformSetTexture2D(m_ResourceId, uniformIndex, value.ResourceId); }


        public void SetUniform(string uniformName, int value) { InternalCalls.MaterialUniformSetS32(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, float value) { InternalCalls.MaterialUniformSetF32(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, double value) { InternalCalls.MaterialUniformSetF64(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, bool value) { InternalCalls.MaterialUniformSetBool(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec2F value) { InternalCalls.MaterialUniformSetVec2F(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec3F value) { InternalCalls.MaterialUniformSetVec3F(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec4F value) { InternalCalls.MaterialUniformSetVec4F(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec2S value) { InternalCalls.MaterialUniformSetVec2S(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec3S value) { InternalCalls.MaterialUniformSetVec3S(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec4S value) { InternalCalls.MaterialUniformSetVec4S(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec2U value) { InternalCalls.MaterialUniformSetVec2U(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec3U value) { InternalCalls.MaterialUniformSetVec3U(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec4U value) { InternalCalls.MaterialUniformSetVec4U(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec2B value) { InternalCalls.MaterialUniformSetVec2B(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec3B value) { InternalCalls.MaterialUniformSetVec3B(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Vec4B value) { InternalCalls.MaterialUniformSetVec4B(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Mat3F value) { InternalCalls.MaterialUniformSetMat3F(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Mat4F value) { InternalCalls.MaterialUniformSetMat4F(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, TMat value) { InternalCalls.MaterialUniformSetTMat(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, ColorF128 value) { InternalCalls.MaterialUniformSetColorF128(m_ResourceId, EnsureUniformIndex(uniformName), ref value); }
        public void SetUniform(string uniformName, Texture2D value) { InternalCalls.MaterialUniformSetTexture2D(m_ResourceId, EnsureUniformIndex(uniformName), value.ResourceId); }
    }
}
