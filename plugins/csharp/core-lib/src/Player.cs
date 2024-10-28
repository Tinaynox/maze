using System;
using Maze;


namespace Sandbox
{
    public class Player : MonoBehaviour
    {
        public Player()
        {
            Debug.Log("Player constructor!");
        }

        public override void OnCreate()
        {
            Debug.LogWarning($"Player.OnCreate {eid}!");
        }

        public override void OnUpdate(float _dt)
        {
            if (Input.GetKeyState(KeyCode.Left))
            {
                Vec3F vec = new Vec3F(-5.0f * _dt, 0.0f, 0.0f);
                InternalCalls.Translate3D(eid, ref vec);
            }

            if (Input.GetKeyState(KeyCode.Right))
            {
                Vec3F vec = new Vec3F(5.0f * _dt, 0.0f, 0.0f);
                InternalCalls.Translate3D(eid, ref vec);
            }

            if (Input.GetKeyState(KeyCode.Down))
            {
                Vec3F vec = new Vec3F(0.0f, -5.0f * _dt, 0.0f);
                InternalCalls.Translate3D(eid, ref vec);
            }

            if (Input.GetKeyState(KeyCode.Up))
            {
                Vec3F vec = new Vec3F(0.0f, 5.0f * _dt, 0.0f);
                InternalCalls.Translate3D(eid, ref vec);
            }
        }
    }
}
