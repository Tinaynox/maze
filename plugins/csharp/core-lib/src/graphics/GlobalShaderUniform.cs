using Maze.Core;
using System;

namespace Maze.Graphics
{
    public class GlobalShaderUniform
    {
        GlobalShaderUniformId m_Id;
        public GlobalShaderUniformId Id => m_Id;


        public GlobalShaderUniform(GlobalShaderUniformId id)
        {
            m_Id = id;
        }

        public static GlobalShaderUniform Get(GlobalShaderUniformId id)
        {
            if (InternalCalls.GlobalShaderUniformIdIsValid(id.Id))
                return new GlobalShaderUniform(id);

            return null;
        }

        public static GlobalShaderUniform Ensure(string name)
        {
            int id = InternalCalls.GlobalShaderUniformEnsure(name);
            if (id == GlobalShaderUniformId.InvalidId.Id)
                return null;

            return new GlobalShaderUniform(new GlobalShaderUniformId(id));
        }

        public void Set(int value) { InternalCalls.GlobalShaderUniformSetS32(m_Id.Id, ref value); }
        public void Set(float value) { InternalCalls.GlobalShaderUniformSetF32(m_Id.Id, ref value); }
        public void Set(double value) { InternalCalls.GlobalShaderUniformSetF64(m_Id.Id, ref value); }
        public void Set(bool value) { InternalCalls.GlobalShaderUniformSetBool(m_Id.Id, ref value); }
        public void Set(Vec2F value) { InternalCalls.GlobalShaderUniformSetVec2F(m_Id.Id, ref value); }
        public void Set(Vec3F value) { InternalCalls.GlobalShaderUniformSetVec3F(m_Id.Id, ref value); }
        public void Set(Vec4F value) { InternalCalls.GlobalShaderUniformSetVec4F(m_Id.Id, ref value); }
        public void Set(Vec2S value) { InternalCalls.GlobalShaderUniformSetVec2S(m_Id.Id, ref value); }
        public void Set(Vec3S value) { InternalCalls.GlobalShaderUniformSetVec3S(m_Id.Id, ref value); }
        public void Set(Vec4S value) { InternalCalls.GlobalShaderUniformSetVec4S(m_Id.Id, ref value); }
        public void Set(Vec2U value) { InternalCalls.GlobalShaderUniformSetVec2U(m_Id.Id, ref value); }
        public void Set(Vec3U value) { InternalCalls.GlobalShaderUniformSetVec3U(m_Id.Id, ref value); }
        public void Set(Vec4U value) { InternalCalls.GlobalShaderUniformSetVec4U(m_Id.Id, ref value); }
        public void Set(Vec2B value) { InternalCalls.GlobalShaderUniformSetVec2B(m_Id.Id, ref value); }
        public void Set(Vec3B value) { InternalCalls.GlobalShaderUniformSetVec3B(m_Id.Id, ref value); }
        public void Set(Vec4B value) { InternalCalls.GlobalShaderUniformSetVec4B(m_Id.Id, ref value); }
        public void Set(Mat3F value) { InternalCalls.GlobalShaderUniformSetMat3F(m_Id.Id, ref value); }
        public void Set(Mat4F value) { InternalCalls.GlobalShaderUniformSetMat4F(m_Id.Id, ref value); }
        public void Set(TMat value) { InternalCalls.GlobalShaderUniformSetTMat(m_Id.Id, ref value); }
        public void Set(ColorF128 value) { InternalCalls.GlobalShaderUniformSetColorF128(m_Id.Id, ref value); }
    }
}
