using Maze;
using Maze.Core;
using Maze.Graphics;
using System;
using System.Collections.Generic;
using System.IO;
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


        void Draw(DataBlock dataBlock)
        {
            if (dataBlock.ParamsCount > 0)
            {
                Debug.LogError("Params:");
                for (int i = 0; i < dataBlock.ParamsCount; ++i)
                {
                    DataBlockParam param = dataBlock.GetParam(i);
                    Debug.LogError($"{i} => {dataBlock.GetSharedString(param.NameId)} type={param.Type} param={param.Value}");
                }
            }

            if (dataBlock.DataBlocksCount > 0)
            { 
                Debug.LogError("DataBlocks:");
                foreach (DataBlock subBlock in dataBlock)
                {
                    Debug.LogError($"Block: {dataBlock.GetSharedString(subBlock.NameId)}");
                    Draw(subBlock);
                }
            }
        }


        [EntitySystem, EnableInEditor]
        public void OnCreate()
        {
            /*
            Debug.LogError("Test1 >>>>");

            DataBlockBinarySerialization.LoadBinaryFile(out DataBlock dataBlock, "AsteroidSmall000.spunit");
            // Draw(dataBlock);

            DataBlockBinarySerialization.SaveBinaryFile(dataBlock, "AsteroidSmall000_Save.spunit");

            DataBlockBinarySerialization.LoadBinaryFile(out DataBlock dataBlock2, "AsteroidSmall000_Save.spunit");
            Draw(dataBlock2);

            Debug.LogError("Test2 <<<<");
            */

            /*
            DataBlock root = new DataBlock();

            Debug.LogError("1");
            DataBlock testBlock = root.AddNewDataBlock("test");
            Debug.LogError("2");
            testBlock.AddS32("a", 42);
            testBlock.AddS16("bb", 255);
            testBlock.AddS8("c", -25);
            testBlock.AddBool("bool", true);
            testBlock.AddS64("d", 23435623622);

            testBlock.SetS16("bb", 23);
            testBlock.SetS16("bbb", 24);

            testBlock.SetVec3F("vec3", new Vec3F(1.0f, 2.0f, 3.0f));

            testBlock.SetString("123", "Hello World!");

            int a = testBlock.GetS32("a");
            short bb = testBlock.GetS16("bb");
            sbyte c = testBlock.GetS8("c");
            long d = testBlock.GetS64("d");
            bool b0 = testBlock.GetBool("bool");
            Debug.LogError($"a={a} bb={bb} c={c} b0={b0} d={d}");
            Debug.LogError($"bbb={testBlock.GetS16("bbb")}");
            Debug.LogError($"string={testBlock.GetString("123")}");
            Debug.LogError($"string2={testBlock.GetString("1234")}");
            Debug.LogError($"vec3={testBlock.GetVec3F("vec3")}");

            root["test"].SetParam("TEST_TEST", 256.0);

            foreach (DataBlock dataBlock in root)
            {
                Debug.LogError($"dataBlock => {dataBlock.Name}");

                for (int i = 0; i < dataBlock.ParamsCount; ++i)
                {
                    DataBlockParam param = dataBlock.GetParam(i);
                    Debug.LogError($"{i} => {dataBlock.GetSharedString(param.NameId)} type={param.Type} param={param.Value}");
                }
            }

            Debug.LogWarning($"{root["test"].GetString("123")}");
            */
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

        [EntitySystem, EnableInEditor]
        public void OnEvent(RenderWindowWillCloseEvent evt)
        {
            if (InternalCalls.GetEditorMainRenderWindow() == evt.renderWindowResourceId)
            {
                DestroyEditors();
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