using Maze.Core;
using Maze.Graphics;

namespace Sandbox
{
    public class Player : MonoBehaviour
    {
        Transform3D m_Transform;
        TestRotor m_TestRotor;

        public float Speed { get; set; } = 6.0f;

        public string PString { get; set; } = "Some";
        public bool PBool { get; set; } = true;
        public char PS8 { get; set; } = 'a';
        public short PS16 { get; set; } = 10000;
        public int PS32 { get; set; } = 50000;
        public long PS64 { get; set; } = 150000;
        public byte PU8 { get; set; } = 65;
        public ushort PU16 { get; set; } = 10000;
        public uint PU32 { get; set; } = 50000;
        public ulong PU64 { get; set; } = 150000;
        public float PF32 { get; set; } = 3.14f;
        public double PF64 { get; set; } = 6.28;

        public Vec2F PVec2F { get; set; } = new Vec2F(6.0f, 7.0f);
        public Vec3F PVec3F { get; set; } = new Vec3F(1.0f, 2.0f, 3.0f);
        public Vec4F PVec4F { get; set; } = new Vec4F(7.0f, 7.0f, 7.0f, 7.0f);
        public Rect2F PRect2F { get; set; } = new Rect2F(0.0f, 0.0f, 1.0f, 2.0f);


        public ColorU32 PColorU32 { get; set; } = new ColorU32(50, 255, 50, 255);
        public ColorF128 PColorF128 { get; set; } = new ColorF128(1.0f, 0.66f, 0.0f, 1.0f);


        [EntitySystem]
        public void OnCreate()
        {
            Debug.LogError("Some!");
            m_Transform = GetEntity().GetComponent<Transform3D>();
            m_TestRotor = GetEntity().GetComponent<TestRotor>();
        }

        [EntitySystem(new string[] { "default" })]
        public void OnUpdate(float _dt)
        {
            m_TestRotor.Speed = m_Transform.position.X * 3.0f;

            if (Input.GetKeyState(KeyCode.Left))
            {
                m_Transform.Translate(new Vec3F(-Speed * _dt, 0.0f, 0.0f));
            }

            if (Input.GetKeyState(KeyCode.Right))
            {
                m_Transform.Translate(new Vec3F(Speed * _dt, 0.0f, 0.0f));
            }

            if (Input.GetKeyState(KeyCode.Down))
            {
                m_Transform.Translate(new Vec3F(0.0f, -Speed * _dt, 0.0f));
            }

            if (Input.GetKeyState(KeyCode.Up))
            {
                m_Transform.Translate(new Vec3F(0.0f, Speed * _dt, 0.0f));
            }
        }
    }
}
