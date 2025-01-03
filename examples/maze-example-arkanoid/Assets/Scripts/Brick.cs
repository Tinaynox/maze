using Maze;
using Maze.Core;
using Maze.Graphics;

public class Brick : GameObject
{
    [SerializeField]
    AssetUnitId m_BrickMaterial1;

    [SerializeField]
    AssetUnitId m_BrickMaterial2;

    [SerializeField]
    AssetUnitId m_BrickMaterial3;
    
    MeshRenderer m_MeshRenderer;

    int m_Health = 1;

    public override GameObjectType GetGameObjectType() => GameObjectType.Brick;


    [EntitySystem]
    public void OnCreate()
    {
        m_Transform = GetComponent<Transform3D>();
        m_MeshRenderer = GetComponent<MeshRenderer>();
        UpdateMeshMaterial();

        if (GameController.Instance != null)
            GameController.Instance.AddGameObject(this);
    }

    [EntitySystem]
    public void OnUpdate(float _dt)
    {

    }

    public void Damage()
    {
        SetHealth(m_Health - 1);

        

        if (m_Health <= 0)
        {
            GameController.Instance.DestroyBrick(this);
        }
    }

    void UpdateMeshMaterial()
    {
        if (m_MeshRenderer == null)
            return;

        if (m_Health == 1)
            m_MeshRenderer.SetMaterial(m_BrickMaterial1);
        else
        if (m_Health == 2)
            m_MeshRenderer.SetMaterial(m_BrickMaterial2);
        else
            m_MeshRenderer.SetMaterial(m_BrickMaterial3);
    }

    public void SetHealth(int _health)
    {
        m_Health = _health;
        UpdateMeshMaterial();
    }
}
