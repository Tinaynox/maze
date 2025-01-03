using Maze;
using Maze.Core;

public abstract class GameObject : MonoBehaviour
{
    protected Transform3D m_Transform;
    public Transform3D Transform => m_Transform;

    protected GameObject()
    {

    }

    ~GameObject()
    {
        if (GameController.Instance != null)
            GameController.Instance.RemoveGameObject(this);
    }

    public abstract GameObjectType GetGameObjectType();

    public bool IsOverlap(GameObject _obj)
    {
        float minX0 = Transform.X - Transform.Scale.X * 0.5f;
        float minY0 = Transform.Y - Transform.Scale.Y * 0.5f;
        float maxX0 = Transform.X + Transform.Scale.X * 0.5f;
        float maxY0 = Transform.Y + Transform.Scale.Y * 0.5f;

        float minX1 = _obj.Transform.X - _obj.Transform.Scale.X * 0.5f;
        float minY1 = _obj.Transform.Y - _obj.Transform.Scale.Y * 0.5f;
        float maxX1 = _obj.Transform.X + _obj.Transform.Scale.X * 0.5f;
        float maxY1 = _obj.Transform.Y + _obj.Transform.Scale.Y * 0.5f;

        if (minX0 <= maxX1 && maxX0 >= minX1 && minY0 <= maxY1 && maxY0 >= minY1)
            return true;

        return false;
    }
}
