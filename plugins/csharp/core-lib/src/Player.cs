using System;
using Maze;


namespace Sandbox
{
    public class Player : MonoBehaviour
    {
        Transform3D transform;

        [EntitySystem]
        public void OnCreate()
        {
            transform = GetComponent<Transform3D>();
        }

        [EntitySystem(
            new string[] { "default" },
            new string[] { "Sandbox.Player2::OnUpdate" })]
        public void OnUpdate(float _dt)
        {
            if (Input.GetKeyState(KeyCode.Left))
            {
                transform.Translate(new Vec3F(-5.0f * _dt, 0.0f, 0.0f));
            }

            if (Input.GetKeyState(KeyCode.Right))
            {
                transform.Translate(new Vec3F(5.0f * _dt, 0.0f, 0.0f));
            }

            if (Input.GetKeyState(KeyCode.Down))
            {
                transform.Translate(new Vec3F(0.0f, -5.0f * _dt, 0.0f));
            }

            if (Input.GetKeyState(KeyCode.Up))
            {
                transform.Translate(new Vec3F(0.0f, 5.0f * _dt, 0.0f));
            }
        }
    }
}
