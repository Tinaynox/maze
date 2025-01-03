using Maze;
using Maze.Core;

public class Brick : GameObject
{
    public override GameObjectType GetGameObjectType() => GameObjectType.Brick;


    [EntitySystem]
    public void OnCreate()
    {
        m_Transform = GetComponent<Transform3D>();

        if (GameController.Instance != null)
            GameController.Instance.AddGameObject(this);
    }

    [EntitySystem]
    public void OnUpdate(float _dt)
    {

    }

}
