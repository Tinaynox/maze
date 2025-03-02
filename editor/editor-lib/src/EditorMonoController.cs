using Maze;
using Maze.Core;
using System;
using System.Collections.Generic;
using System.Reflection;

namespace Maze.Editor
{
    public class EditorMonoController : MonoBehaviour
    {
        List<Editor> m_Editors = new List<Editor>();


        EditorMonoController()
        {
            CreateEditors();
        }


        [EntitySystem, EnableInEditor]
        public void OnCreate()
        {
            
        }

        [EntitySystem, EnableInEditor]
        public void OnDestroy()
        {
            DestroyEditors();
        }

        [EntitySystem, EnableInEditor]
        public void OnUpdate(float _dt)
        {
            foreach (Editor editor in m_Editors)
                editor.Update(_dt);
        }


        void CreateEditors()
        {
            DestroyEditors();

            foreach (Assembly assembly in AppDomain.CurrentDomain.GetAssemblies())
            { 
                var types = assembly.GetTypes();
                foreach (var type in types)
                {
                    if (typeof(Editor) != type && typeof(Editor).IsAssignableFrom(type))
                    {
                        var editorInstance = Activator.CreateInstance(type) as Editor;
                        m_Editors.Add(editorInstance);
                    }
                }
            }
        }

        void DestroyEditors()
        {
            m_Editors.Clear();
        }
    }
}