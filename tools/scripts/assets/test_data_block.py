import data_block
import os
from maze_types import *


if __name__ == '__main__':

    boolValue = True
    s32Value = S32(2147483647)
    s64Value = S64(9223372036854775807)
    u32Value = U32(0xffffffff)
    u64Value = U64(0xffffffffffffffff)
    f32Value = F32(2147440000.0)
    f64Value = F64(9007200000000000.0)
    vec2bValue = Vec2B(boolValue, boolValue)
    vec3bValue = Vec3B(boolValue, boolValue, boolValue)
    vec4bValue = Vec4B(boolValue, boolValue, boolValue, boolValue)
    vec2sValue = Vec2S32(s32Value, s32Value)
    vec3sValue = Vec3S32(s32Value, s32Value, s32Value)
    vec4sValue = Vec4S32(s32Value, s32Value, s32Value, s32Value)
    vec2uValue = Vec2U32(u32Value, u32Value)
    vec3uValue = Vec3U32(u32Value, u32Value, u32Value)
    vec4uValue = Vec4U32(u32Value, u32Value, u32Value, u32Value)
    vec2fValue = Vec2F32(f32Value, f32Value)
    vec3fValue = Vec3F32(f32Value, f32Value, f32Value)
    vec4fValue = Vec4F32(f32Value, f32Value, f32Value, f32Value)
    mat3fValue = Mat3F32(f32Value, 0.0, 0.0,
                         0.0, f32Value, 0.0,
                         0.0, 0.0, f32Value)
    mat4fValue = Mat4F32(f32Value, 0.0, 0.0, 0.0,
                         0.0, f32Value, 0.0, 0.0,
                         0.0, 0.0, f32Value, 0.0,
                         0.0, 0.0, 0.0, f32Value)
    tmValue = TMat(1.0, 0.0, 0.0,
                   0.0, 1.0, 0.0,
                   0.0, 0.0, 1.0,
                   3.0, 4.0, 5.0)
    stringValue = "Single line string"
    stringValue2 = "Multi\n" "\tline\n" "\t\tstring"

    testBlock = data_block.DataBlock()
    testBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_CPP," Simple values")
    testBlock.add_param_bool("boolValue", boolValue)
    testBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP, " Bool")
    testBlock.add_param_s32("s32Value", s32Value)
    testBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP, " S32")
    testBlock.add_param_s64("s64Value", s64Value)
    testBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP, " S64")
    testBlock.add_param_u32("u32Value", u32Value)
    testBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP, " U32")
    testBlock.add_param_u64("u64Value", u64Value)
    testBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP, " U64")
    testBlock.add_param_f32("f32Value", f32Value)
    testBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP, " F32")
    testBlock.add_param_f64("f64Value", f64Value)
    testBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP, " F64")

    subBlock = testBlock.add_new_data_block("subBlock")

    vectorsBlock = subBlock.add_new_data_block("vectors")
    vectorsBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_C, " Vectors ")
    vectorsBlock.add_param_vec2b("vec2bValue", vec2bValue)
    vectorsBlock.add_param_vec3b("vec3bValue", vec3bValue)
    vectorsBlock.add_param_vec4b("vec4bValue", vec4bValue)
    vectorsBlock.add_param_vec2s32("vec2sValue", vec2sValue)
    vectorsBlock.add_param_vec3s32("vec3sValue", vec3sValue)
    vectorsBlock.add_param_vec4s32("vec4sValue", vec4sValue)
    vectorsBlock.add_param_vec2u32("vec2uValue", vec2uValue)
    vectorsBlock.add_param_vec3u32("vec3uValue", vec3uValue)
    vectorsBlock.add_param_vec4u32("vec4uValue", vec4uValue)
    vectorsBlock.add_param_vec2f32("vec2fValue", vec2fValue)
    vectorsBlock.add_param_vec3f32("vec3fValue", vec3fValue)
    vectorsBlock.add_param_vec4f32("vec4fValue", vec4fValue)

    matricesBlock = subBlock.add_new_data_block("matrices")
    matricesBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_C, " Matrices ")
    matricesBlock.add_param_mat3f32("mat3fValue", mat3fValue)
    matricesBlock.add_param_mat4f32("mat4fValue", mat4fValue)
    matricesBlock.add_param_tmat("tmValue", tmValue)

    stringsBlock = subBlock.add_new_data_block("strings")
    stringsBlock.add_param_string(data_block.MAZE_DATA_BLOCK_COMMENT_C, " Strings ")
    stringsBlock.add_param_string("stringValue", stringValue)
    stringsBlock.add_param_string("stringValue2", stringValue2)

    current_directory = os.getcwd()
    print("Current working directory:", current_directory)

    testTextDataBlockFile = "E:/1/1/123.mzdata"
    testBlock.save_text_file(testTextDataBlockFile)

    readTestBlock = data_block.DataBlock.load_text_file(testTextDataBlockFile)
    readTestBlock.save_text_file("E:/1/1/456.mzdata")
