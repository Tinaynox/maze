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
        Dictionary<string, Editor> m_EditorsMenuBar = new Dictionary<string, Editor>();


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

        [EntitySystem, EnableInEditor]
        public void OnEvent(MonoPreShutdownEvent evt)
        {
            InternalCalls.MenuBarClearOptions();
            DestroyEditors();
        }

        [EntitySystem, EnableInEditor]
        public void OnEvent(OpenEditorEvent evt)
        {
            Editor editor = m_EditorsMenuBar[evt.editorName];
            if (editor != null)
            {
                editor.SwitchState();
            }
            else
            {
                Debug.LogError($"Undefined editor - {evt.editorName}!");
                return;
            }
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

                        var menuBarAttribute = type.GetCustomAttribute<EditorMenuBarAttribute>();
                        if (menuBarAttribute != null)
                        {
                            if (menuBarAttribute.OptionPath.Length > 0)
                                InternalCalls.MenuBarAddOption(menuBarAttribute.MenuName, menuBarAttribute.OptionPath + "/" + menuBarAttribute.Option);
                            else
                                InternalCalls.MenuBarAddOption(menuBarAttribute.MenuName, menuBarAttribute.Option);

                            m_EditorsMenuBar.Add(menuBarAttribute.Option, editorInstance);
                        }
                    }
                }
            }
        }

        void DestroyEditors()
        {
            foreach (Editor editor in m_Editors)
            {
                editor.Destroy();
            }
            m_Editors.Clear();
            m_EditorsMenuBar.Clear();
        }
    }
}