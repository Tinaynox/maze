using Maze;


namespace Sandbox
{
    public class Player : MonoBehaviour
    {
        Transform3D transform;
        TestRotor testRotor;

        public float Speed { get; set; } = 6.0f;


        [EntitySystem]
        public void OnCreate()
        {
            transform = GetComponent<Transform3D>();
            testRotor = GetComponent<TestRotor>();
        }

        [EntitySystem(new string[] { "default" })]
        public void OnUpdate(float _dt)
        {
            testRotor.Speed = transform.position.x * 3.0f;

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
