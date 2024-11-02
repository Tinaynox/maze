using Maze;


namespace Sandbox
{
    public class Player : MonoBehaviour
    {
        Transform3D transform;
        TestRotor testRotor;

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



        [EntitySystem]
        public void OnCreate()
        {
            transform = GetComponent<Transform3D>();
            testRotor = GetComponent<TestRotor>();
        }

        [EntitySystem(new string[] { "default" })]
        public void OnUpdate(float _dt)
        {
            testRotor.Speed = transform.position.X * 3.0f;

            if (Input.GetKeyState(KeyCode.Left))
            {
                transform.Translate(new Vec3F(-Speed * _dt, 0.0f, 0.0f));
            }

            if (Input.GetKeyState(KeyCode.Right))
            {
                transform.Translate(new Vec3F(Speed * _dt, 0.0f, 0.0f));
            }

            if (Input.GetKeyState(KeyCode.Down))
            {
                transform.Translate(new Vec3F(0.0f, -Speed * _dt, 0.0f));
            }

            if (Input.GetKeyState(KeyCode.Up))
            {
                transform.Translate(new Vec3F(0.0f, Speed * _dt, 0.0f));
            }
        }
    }
}
