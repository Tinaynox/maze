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
                Debug.Log($"Left {_dt}!");
            }
            else
            if (Input.GetKeyState(KeyCode.Right))
            {
                Debug.Log($"Right {_dt}!");
            }
            else
            if (Input.GetKeyState(KeyCode.Down))
            {
                Debug.Log($"Down {_dt}!");
            }
            else
            if (Input.GetKeyState(KeyCode.Up))
            {
                Debug.Log($"Up {_dt}!");
            }
        }
    }
}
