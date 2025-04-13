using System;

namespace Maze.Graphics
{
    public struct GlobalShaderUniformId
    {
        int m_Id;
        public int Id => m_Id;

        public static GlobalShaderUniformId InvalidId => new GlobalShaderUniformId(-1);

        public GlobalShaderUniformId(int id)
        {
            m_Id = id;
        }

        public override bool Equals(object obj)
        {
            return m_Id.Equals(obj);
        }

        public override int GetHashCode()
        {
            return m_Id.GetHashCode();
        }

        public static bool operator ==(GlobalShaderUniformId value0, GlobalShaderUniformId value1)
        {
            return value0.m_Id == value1.m_Id;
        }

        public static bool operator !=(GlobalShaderUniformId value0, GlobalShaderUniformId value1)
        {
            return value0.m_Id != value1.m_Id;
        }

        public override string ToString()
        {
            return m_Id.ToString();
        }
    }
}
