import data_block
import os


if __name__ == '__main__':
    testBlock = data_block.DataBlock()
    testBlock.add_param("test", data_block.DataBlockParamType.S32, 42)
    testBlock.add_param("test2", data_block.DataBlockParamType.S32, 12)
    testBlock.set_param(1, data_block.DataBlockParamType.S32, 77)
    testBlock.create_data_block("sub").add_param("sub_test", data_block.DataBlockParamType.S32, 777)

    current_directory = os.getcwd()
    print("Current working directory:", current_directory)

    testBlock.save_text_file("E:/1/1/123.mzdata")

    print(testBlock.get_param("test").value)
    print(testBlock.get_param_value("test", 100))
    print(testBlock.get_param_value("test2", 100))
    print(testBlock.get_data_block("sub").get_param_value("sub_test", 100))
