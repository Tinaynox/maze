from enum import Enum
import os
from maze_types import *


class DataBlockParamType(Enum):
    NONE = 0
    S32 = 1
    S64 = 2
    U32 = 3
    U64 = 4
    F32 = 5
    F64 = 6
    Bool = 7
    Vec4S8 = 8
    Vec4U8 = 9
    Vec2S32 = 10
    Vec3S32 = 11
    Vec4S32 = 12
    Vec2U32 = 13
    Vec3U32 = 14
    Vec4U32 = 15
    Vec2F32 = 16
    Vec3F32 = 17
    Vec4F32 = 18
    Vec2B = 19
    Vec3B = 20
    Vec4B = 21
    Mat3F32 = 22
    Mat4F32 = 23
    TMat = 24
    String = 25


class DataBlockParamTypeInfo:
    def __init__(self, name="", type=S32):
        self.name = name
        self.type = type


data_block_param_type_info = {
    DataBlockParamType.NONE: DataBlockParamTypeInfo("unknown", None),
    DataBlockParamType.S32: DataBlockParamTypeInfo("S32", S32),
    DataBlockParamType.S64: DataBlockParamTypeInfo("S64", S64),
    DataBlockParamType.U32: DataBlockParamTypeInfo("U32", U32),
    DataBlockParamType.U64: DataBlockParamTypeInfo("U64", U64),
    DataBlockParamType.F32: DataBlockParamTypeInfo("F32", F32),
    DataBlockParamType.F64: DataBlockParamTypeInfo("F64", F64),
    DataBlockParamType.Bool: DataBlockParamTypeInfo("Bool", Bool),
    DataBlockParamType.Vec4S8: DataBlockParamTypeInfo("Vec4S8", Vec4S8),
    DataBlockParamType.Vec4U8: DataBlockParamTypeInfo("Vec4U8", Vec4U8),
    DataBlockParamType.Vec2S32: DataBlockParamTypeInfo("Vec2S", Vec2S32),
    DataBlockParamType.Vec3S32: DataBlockParamTypeInfo("Vec3S", Vec3S32),
    DataBlockParamType.Vec4S32: DataBlockParamTypeInfo("Vec4S", Vec4S32),
    DataBlockParamType.Vec2U32: DataBlockParamTypeInfo("Vec2U", Vec2U32),
    DataBlockParamType.Vec3U32: DataBlockParamTypeInfo("Vec3U", Vec3U32),
    DataBlockParamType.Vec4U32: DataBlockParamTypeInfo("Vec4U", Vec4U32),
    DataBlockParamType.Vec2F32: DataBlockParamTypeInfo("Vec2F", Vec2F32),
    DataBlockParamType.Vec3F32: DataBlockParamTypeInfo("Vec3F", Vec3F32),
    DataBlockParamType.Vec4F32: DataBlockParamTypeInfo("Vec4F", Vec4F32),
    DataBlockParamType.Vec2B: DataBlockParamTypeInfo("Vec2B", Vec2B),
    DataBlockParamType.Vec3B: DataBlockParamTypeInfo("Vec3B", Vec3B),
    DataBlockParamType.Vec4B: DataBlockParamTypeInfo("Vec4B", Vec4B),
    DataBlockParamType.Mat3F32: DataBlockParamTypeInfo("Mat3F", Mat3F32),
    DataBlockParamType.Mat4F32: DataBlockParamTypeInfo("Mat4F", Mat4F32),
    DataBlockParamType.TMat: DataBlockParamTypeInfo("TMat", TMat),
    DataBlockParamType.String: DataBlockParamTypeInfo("String", String)
}

data_block_param_type_from_string = {
    "unknown": DataBlockParamType.NONE,
    "S32": DataBlockParamType.S32,
    "S64": DataBlockParamType.S64,
    "U32": DataBlockParamType.U32,
    "U64": DataBlockParamType.U64,
    "F32": DataBlockParamType.F32,
    "F64": DataBlockParamType.F64,
    "Bool": DataBlockParamType.Bool,
    "Vec4S8": DataBlockParamType.Vec4S8,
    "Vec4U8": DataBlockParamType.Vec4U8,
    "Vec2S32": DataBlockParamType.Vec2S32,
    "Vec2S": DataBlockParamType.Vec2S32,
    "Vec3S32": DataBlockParamType.Vec3S32,
    "Vec3S": DataBlockParamType.Vec3S32,
    "Vec4S32": DataBlockParamType.Vec4S32,
    "Vec4S": DataBlockParamType.Vec4S32,
    "Vec2U32": DataBlockParamType.Vec2U32,
    "Vec2U": DataBlockParamType.Vec2U32,
    "Vec3U32": DataBlockParamType.Vec3U32,
    "Vec3U": DataBlockParamType.Vec3U32,
    "Vec4U32": DataBlockParamType.Vec4U32,
    "Vec4U": DataBlockParamType.Vec4U32,
    "Vec2F32": DataBlockParamType.Vec2F32,
    "Vec2F": DataBlockParamType.Vec2F32,
    "Vec3F32": DataBlockParamType.Vec3F32,
    "Vec3F": DataBlockParamType.Vec3F32,
    "Vec4F32": DataBlockParamType.Vec4F32,
    "Vec4F": DataBlockParamType.Vec4F32,
    "Vec2B": DataBlockParamType.Vec2B,
    "Vec3B": DataBlockParamType.Vec3B,
    "Vec4B": DataBlockParamType.Vec4B,
    "Mat3F32": DataBlockParamType.Mat3F32,
    "Mat3F": DataBlockParamType.Mat3F32,
    "Mat4F32": DataBlockParamType.Mat4F32,
    "Mat4F": DataBlockParamType.Mat4F32,
    "TMat": DataBlockParamType.TMat,
    "String": DataBlockParamType.String
}


class DataBlockParam:
    def __init__(self, param_name="", param_type=DataBlockParamType.NONE, param_value=None):
        assert isinstance(param_name, str), "param_name must be an str"
        self.name = param_name
        self.type = param_type
        self.value = param_value

    def set(self, param_name, param_type, param_value):
        assert isinstance(param_name, str), "param_name must be an str"
        self.name = param_name
        self.type = param_type
        self.value = param_value

    def set(self, param_type, param_value):
        self.type = param_type
        self.value = param_value


MAZE_DATA_BLOCK_COMMENT_PREFIX = "@\x0C"
MAZE_DATA_BLOCK_COMMENT_SUFFIX_CPP = "@\x01"
MAZE_DATA_BLOCK_COMMENT_SUFFIX_C = "@\x02"
MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_CPP = "@\x011"
MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_C = "@\x012"
MAZE_DATA_BLOCK_COMMENT_CPP = MAZE_DATA_BLOCK_COMMENT_PREFIX + MAZE_DATA_BLOCK_COMMENT_SUFFIX_CPP
MAZE_DATA_BLOCK_COMMENT_C = MAZE_DATA_BLOCK_COMMENT_PREFIX + MAZE_DATA_BLOCK_COMMENT_SUFFIX_C
MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP = MAZE_DATA_BLOCK_COMMENT_PREFIX + MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_CPP
MAZE_DATA_BLOCK_COMMENT_ENDLINE_C = MAZE_DATA_BLOCK_COMMENT_PREFIX + MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_C


def _is_data_block_identifier_char(ch):
    return (('a' <= ch <= 'z') or
            ('A' <= ch <= 'Z') or
            ('0' <= ch <= '9') or
            ch == '_' or
            ch == '-' or
            ch == '.' or
            ch == '~')


def _is_name_is_simple(name):
    result = True
    if name is None or name == "":
        result = False

    for ch in name:
        result = _is_data_block_identifier_char(ch)
        if not result:
            break
    return result


def _write_eol(file):
    file.write("\n")


def _write_indent(file, size):
    for _ in range(size):
        file.write(' ')


def _write_complex_string(file, string):
    quote = "\""
    quote_len = 1

    has_line_break = False
    has_quote = False
    has_tick = False

    for ch in string:
        if ch == '\n' or ch == '\r':
            has_line_break = True
        elif ch == '\"':
            has_quote = True
        elif ch == '\'':
            has_tick = True
        else:
            continue

        if has_line_break and has_quote and has_tick:
            break

    if has_line_break and not has_quote:
        quote = "\"\"\"\n\"\"\""
        quote_len = 4
    elif has_line_break and not has_tick:
        quote = "'''\n'''"
        quote_len = 4
    elif has_line_break:
        quote = "\"\"\"\n\"\"\""
        quote_len = 4
    elif has_quote and not has_tick:
        quote = "'"

    file.write(quote[:quote_len])

    for i in range(len(string)):
        ch = string[i]
        if ch == '~':
            file.write("~~")
        elif ch == quote[0] and (quote_len == 1 or string[i + 1]):
            file.write("~\"" if ch == '\"' else "~\'")
        elif ch == '\r' and quote_len == 1:
            file.write("~r")
        elif ch == '\n' and quote_len == 1:
            file.write("~n")
        elif ch == '\t':
            file.write("~t")
        else:
            file.write(ch)

    file.write(quote[-quote_len:])


def _format_bool(value):
    return "true" if value else "false"


class DataBlock:
    def __init__(self, data_block_name=""):
        assert isinstance(data_block_name, str), "data_block_name must be an str"
        self.name = data_block_name
        self.params = []
        self.data_blocks = []

    # Params
    def add_param(self, param_name, param_type, param_value):
        if param_type == DataBlockParamType.S32:
            self.params.append(DataBlockParam(param_name, param_type, S32(param_value)))
        elif param_type == DataBlockParamType.S64:
            self.params.append(DataBlockParam(param_name, param_type, S64(param_value)))
        elif param_type == DataBlockParamType.U32:
            self.params.append(DataBlockParam(param_name, param_type, U32(param_value)))
        elif param_type == DataBlockParamType.U64:
            self.params.append(DataBlockParam(param_name, param_type, U64(param_value)))
        elif param_type == DataBlockParamType.F32:
            self.params.append(DataBlockParam(param_name, param_type, F32(param_value)))
        elif param_type == DataBlockParamType.F64:
            self.params.append(DataBlockParam(param_name, param_type, F64(param_value)))
        elif param_type == DataBlockParamType.Bool:
            self.params.append(DataBlockParam(param_name, param_type, True if (param_value == 'true') else False))
        elif param_type == DataBlockParamType.Vec4S8:
            self.params.append(DataBlockParam(param_name, param_type, Vec4S8.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec4U8:
            self.params.append(DataBlockParam(param_name, param_type, Vec4U8.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec2S32:
            self.params.append(DataBlockParam(param_name, param_type, Vec2S32.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec3S32:
            self.params.append(DataBlockParam(param_name, param_type, Vec3S32.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec4S32:
            self.params.append(DataBlockParam(param_name, param_type, Vec4S32.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec2U32:
            self.params.append(DataBlockParam(param_name, param_type, Vec2U32.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec3U32:
            self.params.append(DataBlockParam(param_name, param_type, Vec3U32.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec4U32:
            self.params.append(DataBlockParam(param_name, param_type, Vec4U32.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec2F32:
            self.params.append(DataBlockParam(param_name, param_type, Vec2F32.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec3F32:
            self.params.append(DataBlockParam(param_name, param_type, Vec3F32.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec4F32:
            self.params.append(DataBlockParam(param_name, param_type, Vec4F32.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec2B:
            self.params.append(DataBlockParam(param_name, param_type, Vec2B.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec3B:
            self.params.append(DataBlockParam(param_name, param_type, Vec3B.from_str(param_value)))
        elif param_type == DataBlockParamType.Vec4B:
            self.params.append(DataBlockParam(param_name, param_type, Vec4B.from_str(param_value)))
        elif param_type == DataBlockParamType.Mat3F32:
            self.params.append(DataBlockParam(param_name, param_type, Mat3F32.from_str(param_value)))
        elif param_type == DataBlockParamType.Mat4F32:
            self.params.append(DataBlockParam(param_name, param_type, Mat4F32.from_str(param_value)))
        elif param_type == DataBlockParamType.TMat:
            self.params.append(DataBlockParam(param_name, param_type, TMat.from_str(param_value)))
        elif param_type == DataBlockParamType.String:
            self.params.append(DataBlockParam(param_name, param_type, param_value))

    def add_param_s32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.S32, param_value))

    def add_param_s64(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.S64, param_value))

    def add_param_u32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.U32, param_value))

    def add_param_u64(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.U64, param_value))

    def add_param_f32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.F32, param_value))

    def add_param_f64(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.F64, param_value))

    def add_param_bool(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Bool, param_value))

    def add_param_vec4s8(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec4S8, param_value))

    def add_param_vec4u8(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec4U8, param_value))

    def add_param_vec2s32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec2S32, param_value))

    def add_param_vec3s32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec3S32, param_value))

    def add_param_vec4s32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec4S32, param_value))

    def add_param_vec2u32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec2U32, param_value))

    def add_param_vec3u32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec3U32, param_value))

    def add_param_vec4u32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec4U32, param_value))

    def add_param_vec2f32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec2F32, param_value))

    def add_param_vec3f32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec3F32, param_value))

    def add_param_vec4f32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec4F32, param_value))

    def add_param_vec2b(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec2B, param_value))

    def add_param_vec3b(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec3B, param_value))

    def add_param_vec4b(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Vec4B, param_value))

    def add_param_mat3f32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Mat3F32, param_value))

    def add_param_mat4f32(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.Mat4F32, param_value))

    def add_param_tmat(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.TMat, param_value))

    def add_param_string(self, param_name, param_value):
        self.params.append(DataBlockParam(param_name, DataBlockParamType.String, param_value))

    def get_param(self, param_name):
        return next((x for x in self.params if x.name == param_name), None)

    def get_param_value(self, param_name, default_value):
        param = self.get_param(param_name)
        if param is not None:
            return param.value

        return default_value

    def set_param(self, param_index, param_type, param_value):
        if 0 <= param_index < len(self.params):
            param = self.params[param_index]
            param.set(param_type, param_value)

    # Data Blocks
    def add_data_block(self, data_block):
        assert isinstance(data_block, DataBlock), "value must be an DataBlock"
        self.data_blocks.append(data_block)
        return data_block

    def add_new_data_block(self, data_block_name):
        new_data_block = DataBlock(data_block_name)
        self.add_data_block(new_data_block)
        return new_data_block

    def get_data_block(self, data_block_name, default_value=None):
        if default_value is None:
            default_value = DataBlock()
        return next((x for x in self.data_blocks if x.name == data_block_name), default_value)

    def get_data_block_as_string_list(self, data_block_name):
        result = []
        child_block = self.get_data_block(data_block_name)
        for i in range(len(child_block.params)):
            param = child_block.params[i]
            if param.type == DataBlockParamType.String:
                result.append(param.value)
        return result

    # Utils
    def is_empty(self):
        return len(self.params) == 0 and len(self.data_blocks) == 0

    # Serialization
    def save_text_file(self, file_path):
        with open(file_path, "w") as file:
            self.write_to_file(file, 0)

    def write_to_file(self, file, level):
        skip_next_indent = False

        # Params
        for i in range(len(self.params)):
            param = self.params[i]
            name = param.name
            name_is_simple = _is_name_is_simple(name)

            # Indent
            if level > 0:
                if not skip_next_indent:
                    _write_indent(file, level * 2)
                else:
                    skip_next_indent = False

            if len(name) >= 3 and name.startswith(MAZE_DATA_BLOCK_COMMENT_PREFIX):
                if name.startswith(MAZE_DATA_BLOCK_COMMENT_ENDLINE_C) or name.startswith(MAZE_DATA_BLOCK_COMMENT_C):
                    file.write("/*")
                    file.write(param.value)
                    file.write("*/")
                    _write_eol(file)
                elif name.startswith(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP) or name.startswith(
                        MAZE_DATA_BLOCK_COMMENT_CPP):
                    file.write("//")
                    file.write(param.value)
                    _write_eol(file)
                continue

            if _is_name_is_simple(name):
                file.write(name)
            else:
                _write_complex_string(file, name)

            file.write(":")
            file.write(data_block_param_type_info[param.type].name)
            file.write(" = ")
            if param.type == DataBlockParamType.S32:
                file.write(str(param.value))
            elif param.type == DataBlockParamType.S64:
                file.write(str(param.value))
            elif param.type == DataBlockParamType.U32:
                file.write(str(param.value))
            elif param.type == DataBlockParamType.U64:
                file.write(str(param.value))
            elif param.type == DataBlockParamType.F32:
                file.write(str(param.value))
            elif param.type == DataBlockParamType.F64:
                file.write(str(param.value))
            elif param.type == DataBlockParamType.Bool:
                file.write(_format_bool(param.value))
            elif param.type == DataBlockParamType.Vec4S8:
                file.write("{0}, {1}, {2}, {3}".format(param.value.x, param.value.y, param.value.z, param.value.w))
            elif param.type == DataBlockParamType.Vec4U8:
                file.write("{0}, {1}, {2}, {3}".format(param.value.x, param.value.y, param.value.z, param.value.w))
            elif param.type == DataBlockParamType.Vec2S32:
                file.write("{0}, {1}".format(param.value.x, param.value.y))
            elif param.type == DataBlockParamType.Vec3S32:
                file.write("{0}, {1}, {2}".format(param.value.x, param.value.y, param.value.z))
            elif param.type == DataBlockParamType.Vec4S32:
                file.write("{0}, {1}, {2}, {3}".format(param.value.x, param.value.y, param.value.z, param.value.w))
            elif param.type == DataBlockParamType.Vec2U32:
                file.write("{0}, {1}".format(param.value.x, param.value.y))
            elif param.type == DataBlockParamType.Vec3U32:
                file.write("{0}, {1}, {2}".format(param.value.x, param.value.y, param.value.z))
            elif param.type == DataBlockParamType.Vec4U32:
                file.write("{0}, {1}, {2}, {3}".format(param.value.x, param.value.y, param.value.z, param.value.w))
            elif param.type == DataBlockParamType.Vec2F32:
                file.write("{0}, {1}".format(param.value.x, param.value.y))
            elif param.type == DataBlockParamType.Vec3F32:
                file.write("{0}, {1}, {2}".format(param.value.x, param.value.y, param.value.z))
            elif param.type == DataBlockParamType.Vec4F32:
                file.write("{0}, {1}, {2}, {3}".format(param.value.x, param.value.y, param.value.z, param.value.w))
            elif param.type == DataBlockParamType.Vec2B:
                file.write("{0}, {1}".format(
                    _format_bool(param.value.x), _format_bool(param.value.y)))
            elif param.type == DataBlockParamType.Vec3B:
                file.write("{0}, {1}, {2}".format(
                    _format_bool(param.value.x), _format_bool(param.value.y),
                    _format_bool(param.value.z)))
            elif param.type == DataBlockParamType.Vec4B:
                file.write("{0}, {1}, {2}, {3}".format(
                    _format_bool(param.value.x), _format_bool(param.value.y),
                    _format_bool(param.value.z), _format_bool(param.value.w)))
            elif param.type == DataBlockParamType.Mat3F32:
                file.write("[[{0}, {1}, {2}] [{3}, {4}, {5}] [{6}, {7}, {8}]]".format(
                    param.value.m00, param.value.m01, param.value.m02,
                    param.value.m10, param.value.m11, param.value.m12,
                    param.value.m20, param.value.m21, param.value.m22, ))
            elif param.type == DataBlockParamType.Mat4F32:
                file.write("[[{0}, {1}, {2}, {3}] [{4}, {5}, {6}, {7}] [{8}, {9}, {10}, {11}] [{12}, {13}, {14}, {15}]]".format(
                    param.value.m00, param.value.m01, param.value.m02, param.value.m03,
                    param.value.m10, param.value.m11, param.value.m12, param.value.m13,
                    param.value.m20, param.value.m21, param.value.m22, param.value.m23,
                    param.value.m30, param.value.m31, param.value.m32, param.value.m33))
            elif param.type == DataBlockParamType.TMat:
                file.write("[[{0}, {1}, {2}] [{3}, {4}, {5}] [{6}, {7}, {8}] [{9}, {10}, {11}]]".format(
                    param.value.m00, param.value.m01, param.value.m02,
                    param.value.m10, param.value.m11, param.value.m12,
                    param.value.m20, param.value.m21, param.value.m22,
                    param.value.m30, param.value.m31, param.value.m32))
            elif param.type == DataBlockParamType.String:
                _write_complex_string(file, param.value)

            if i + 1 < len(self.params):
                next_param_name = self.params[i + 1].name
                if (len(next_param_name) >= 3
                        and next_param_name.startswith(MAZE_DATA_BLOCK_COMMENT_PREFIX)
                        and (next_param_name.startswith(MAZE_DATA_BLOCK_COMMENT_ENDLINE_C)
                             or next_param_name.startswith(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP))):
                    file.write(" ")
                    skip_next_indent = True
                    continue
            _write_eol(file)

        if len(self.params) > 0 and len(self.data_blocks) > 0:
            _write_eol(file)

        for i in range(len(self.data_blocks)):
            data_block = self.data_blocks[i]
            name = data_block.name

            name_is_simple = _is_name_is_simple(name)

            if len(name) >= 3 and name.startswith(MAZE_DATA_BLOCK_COMMENT_PREFIX):
                if level > 0:
                    _write_indent(file, level * 2)

                if name.startswith(MAZE_DATA_BLOCK_COMMENT_C):
                    file.write("/*")
                    file.write(param.value)
                    file.write("*/")
                    _write_eol(file)
                elif name.startswith(MAZE_DATA_BLOCK_COMMENT_CPP):
                    file.write("//")
                    file.write(param.value)
                    _write_eol(file)
                continue

            # Indent
            if level > 0:
                _write_indent(file, level * 2)

            if _is_name_is_simple(name):
                file.write(name)
            else:
                _write_complex_string(file, name)

            if data_block.is_empty():
                file.write("{}")
                _write_eol(file)
                continue

            # Opening bracket
            _write_eol(file)
            _write_indent(file, level * 2)
            file.write("{")
            _write_eol(file)

            if not data_block.write_to_file(file, level + 1):
                return False

            # Closing bracket
            _write_indent(file, level * 2)
            file.write("}")
            _write_eol(file)

            if i != len(self.data_blocks) - 1:
                _write_eol(file)
        return True

    @staticmethod
    def load_text_file(file_path):
        data_block_text_parser = DataBlockTextParser()
        return data_block_text_parser.parse(file_path)


class DataBlockTextParser:
    class Statement(Enum):
        NONE = 0
        PARAM = 1
        BLOCK = 2

    class PendingComment:
        def __init__(self, start_offset=0, end_offset=0, cpp_style=False):
            self.start_offset = start_offset
            self.end_offset = end_offset
            self.cpp_style = cpp_style

    def __init__(self):
        self.file = None
        self.file_size = 0
        self.current_line = 1
        self.current_line_offset = 0
        self.was_new_line_after_statement = False
        self.last_statement = DataBlockTextParser.Statement.NONE
        self.pending_comments = []
        self.data_block = None

    def parse(self, file_path):
        data_block = DataBlock()

        if os.path.exists(file_path):
            with open(file_path, "rb") as file:
                self.file = file

                current_pos = self.file.tell()
                self.file.seek(0, 2)
                self.file_size = file.tell()
                self.file.seek(current_pos)
                self._parse_data_block(data_block, True)

        return data_block

    def _parse_data_block(self, data_block, is_topmost):

        self.data_block = data_block

        while True:
            if not self._skip_white(True):
                return False

            if self._is_eof():
                if not is_topmost:
                    self._process_syntax_error("Unexpected EOF #1")
                    return False
                break

            if self._read_char_no_rewind() == '}':
                if is_topmost:
                    self._process_syntax_error("Unexpected '}' in topmost block")
                    return False
                self._rewind(1)

                self._flush_pending_comments(data_block, False)
                break

            start_offset = self.file.tell()
            name_text = self._parse_identifier()
            if name_text == "":
                self._process_syntax_error("Expected identifier")
                return False

            if not self._skip_white():
                return False

            if self._is_eof():
                self._process_syntax_error("Unexpected EOF #2")
                return False

            if self._read_char_no_rewind() == '{':
                self._rewind(1)
                self.was_new_line_after_statement = False
                self.last_statement = DataBlockTextParser.Statement.NONE
                self._flush_pending_comments(data_block, False)

                new_block = data_block.add_new_data_block(name_text)
                if not self._parse_data_block(new_block, False):
                    return False

                self.last_statement = DataBlockTextParser.Statement.NONE
            elif self._read_char_no_rewind() == ':':
                self._flush_pending_comments(data_block, True)

                self._rewind(1)
                type_name_text = self._parse_identifier()
                if type_name_text == "":
                    self._process_syntax_error("Expected type identifier")
                    return False

                param_type = data_block_param_type_from_string[type_name_text]
                if param_type is None or param_type == DataBlockParamType.NONE:
                    self._process_syntax_error("Unknown type")
                    return False

                if not self._skip_white():
                    return False

                if self._is_eof():
                    self._process_syntax_error("Unexpected EOF #3")
                    return False

                if self._read_char() != '=':
                    self._process_syntax_error("Expected '='")
                    return False

                if not self._skip_white():
                    return False

                next_ch = self._read_char_no_rewind()
                if next_ch == '\r' or next_ch == '\n':
                    self._process_syntax_error("Unexpected CR/LF")
                    return False

                if self._is_eof():
                    self._process_syntax_error("Unexpected EOF #4")
                    return False

                value_text = self._parse_value()
                if value_text == "":
                    return False

                data_block.add_param(name_text, param_type, value_text)

                self.was_new_line_after_statement = False
                self.last_statement = DataBlockTextParser.Statement.PARAM
            elif self._read_char_no_rewind() == '=':
                print('NOT IMPLEMENTED')
            else:
                self._process_syntax_error("Syntax error!")
                return False

        return True


    def _skip_white(self, allow_crlf=True, track_new_line_after_param=False):
        while True:
            if self._is_eof():
                break
            ch = self._read_char()
            if ch == ' ' or ch == '\t' or ch == '\x1A' or ch == 0:
                continue

            if allow_crlf:
                if ch == '\r':
                    next_ch = self._read_char_no_rewind()
                    if not self._is_eof() and next_ch == '\n':
                        self._rewind(1)
                        self._inc_current_line()
                        if track_new_line_after_param:
                            self.was_new_line_after_statement = True
                    continue
                elif ch == '\n':
                    self._inc_current_line()
                    if track_new_line_after_param:
                        self.was_new_line_after_statement = True
                    continue

            if ch == '/':
                if not self._is_eof():
                    next_ch = self._read_char()
                    if next_ch == '/':
                        cpp_comment_start_offset = self.file.tell()

                        while not self._is_eof():
                            next_next_ch = self._read_char()
                            if next_next_ch == '\r' or next_next_ch == '\n':
                                break

                        if self.was_new_line_after_statement or self.last_statement == DataBlockTextParser.Statement.NONE:
                            self.pending_comments.append(DataBlockTextParser.PendingComment(
                                cpp_comment_start_offset, self.file.tell() - 1, True))
                        elif self.last_statement == DataBlockTextParser.Statement.PARAM:
                            self.data_block.add_param_string(
                                MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP,
                                self._read_text(
                                    cpp_comment_start_offset,
                                    self.file.tell() - cpp_comment_start_offset - 1))
                        else:
                            self.data_block.add_new_data_block(
                                MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP).add_param_string(
                                MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP,
                                self._read_text(
                                    cpp_comment_start_offset,
                                    self.file.tell() - cpp_comment_start_offset - 1))

                        continue
                    elif next_ch == '*':
                        c_comment_start_offset = self.file.tell()
                        count = 1
                        while self._can_read_size(2):
                            next_next_chars = self._read_text_no_rewind(2)

                            if next_next_chars[0] == '/' and next_next_chars[1] == '*':
                                self._rewind(2)
                                count = count + 1
                            elif next_next_chars[0] == '*' and next_next_chars[1] == '/':
                                self._rewind(2)
                                count = count - 1
                                if count <= 0:
                                    break
                            else:
                                self._rewind(1)

                        if count > 0 and not self._can_read_size(2):
                            self._process_syntax_error(
                                "Unexpected EOF inside comment (count={0}, tell={1}, size={2} "
                                "c_comment_start_offset={3})".format(
                                count, self.file.tell(), self.file_size, c_comment_start_offset))
                            return False

                        if self.was_new_line_after_statement or self.last_statement == DataBlockTextParser.Statement.NONE:
                            self.pending_comments.append(DataBlockTextParser.PendingComment(
                                c_comment_start_offset, self.file.tell() - 2, False))
                        elif self.last_statement == DataBlockTextParser.Statement.PARAM:
                            self.data_block.add_param_string(
                                MAZE_DATA_BLOCK_COMMENT_ENDLINE_C,
                                self._read_text(
                                    c_comment_start_offset,
                                    self.file.tell() - c_comment_start_offset - 2))
                        else:
                            self.data_block.add_new_data_block(
                                MAZE_DATA_BLOCK_COMMENT_ENDLINE_C).add_param_string(
                                MAZE_DATA_BLOCK_COMMENT_ENDLINE_C,
                                self._read_text(
                                    c_comment_start_offset,
                                    self.file.tell() - c_comment_start_offset - 1))

                        continue
                    else:
                        self._rewind(-1)

                self._rewind(-1)
                break
            else:
                self._rewind(-1)
                break
        return True

    def _parse_identifier(self):
        while True:
            if not self._skip_white():
                return ""

            if self._is_eof():
                break

            ch = self._read_char_no_rewind()
            if _is_data_block_identifier_char(ch):
                identifier_offset = self.file.tell()
                self._rewind(1)
                while not self._is_eof():
                    if not _is_data_block_identifier_char(self._read_char_no_rewind()):
                        break
                    self._rewind(1)

                identifier_length = self.file.tell() - identifier_offset

                return self._read_text(identifier_offset, identifier_length)
            elif ch == '"' or ch == '\'':
                return self._parse_value()
            else:
                break
        return ""

    def _parse_value(self):
        value = ""

        multi_line_string = False

        quot_ch = 0

        if self._read_char_no_rewind() == '"' or self._read_char_no_rewind() == '\'':
            quot_ch = self._read_char()

            two_ch = self._read_text_no_rewind(2)

            # Triple quot (MultiLine value prefix like '''\n )
            if two_ch[0] == quot_ch and two_ch[1] == quot_ch:
                multi_line_string = True
                self._rewind(2)

                # Skip first \n (start) of multiline
                offs = self.file.tell()
                ch = self._read_char()
                eol_found = False
                while ch != 0:
                    if ch == '\n':
                        eol_found = True
                        break
                    elif ch != ' ' and ch != '\r' and ch != '\t':
                        break
                    ch = self._read_char()

                if not eol_found:
                    self.file.seek(offs)
        multi_line_comment_offset = -1
        rewind_to_offset = -1

        while True:
            if self._is_eof():
                break

            ch = self._read_char_no_rewind(0)

            # MultiLine comment
            if multi_line_comment_offset != -1:
                ch2 = self._read_char_no_rewind(1)
                if ch == '\r':
                    if ch2 == '\n':
                        self._rewind(2)
                        self._inc_current_line()
                    rewind_to_offset = multi_line_comment_offset
                    break
                elif ch == '\n':
                    self._rewind(1)
                    self._inc_current_line()
                    rewind_to_offset = multi_line_comment_offset
                    break
                elif ch == 0:
                    self._process_syntax_error("Unclosed comment")
                    return ""
                elif ch == '*' and ch2 == '/':
                    self._rewind(2)
                    ch = self._read_char_no_rewind()
                    if ch == '\r' or ch == '\n':
                        rewind_to_offset = multi_line_comment_offset
                    multi_line_comment_offset = -1
                else:
                    self._rewind(1)
                    continue

            if quot_ch != 0:
                if ch == quot_ch and not multi_line_string:
                    self._rewind(1)
                    if not self._skip_white():
                        return ""
                    if self._read_char_no_rewind() == ';':
                        self._rewind(1)
                    break
                elif (ch == quot_ch and multi_line_string and
                      self._read_char_no_rewind(1) == quot_ch and
                      self._read_char_no_rewind(2) == quot_ch):
                    # Crop last multiline \n (end)
                    if len(value) > 1 and value[len(value) - 1] == '\n':
                        value = value[0:len(value) - 1]
                    self._rewind(3)

                    if not self._skip_white():
                        return ""
                    if self._read_char_no_rewind() == ';':
                        self._rewind(1)
                    break
                elif ((ch == '\r' or ch == '\n') and not multi_line_string) or ch == 0:
                    self._process_syntax_error("Unclosed string")
                    return ""
                elif ch == '~':
                    self._rewind(1)

                    if self._is_eof():
                        self._process_syntax_error("Unclosed string")
                        return ""

                    ch = self._read_char_no_rewind()
                    if ch == 'r':
                        ch = '\r'
                    elif ch == 'n':
                        ch = '\n'
                    elif ch == 't':
                        ch = '\t'
                elif ch == '\r':
                    self._rewind(1)
                    continue
            else:
                if ch == ';' or ch == '\r' or ch == '\n' or ch == 0 or ch == '}':
                    if ch == ';':
                        self._rewind(1)
                    break
                elif ch == '/':
                    next_ch = self._read_char_no_rewind(1)

                    if next_ch == '/':
                        break
                    elif next_ch == '*':
                        multi_line_comment_offset = self.file.tell() - 1
                        self._rewind(2)
                        continue

            value = value + ch
            self._rewind(1)

        if multi_line_comment_offset != -1:
            while True:
                ch = self._read_char_no_rewind()
                if ch == 0:
                    self._process_syntax_error("Unclosed string")
                    return ""
                elif ch == '\r' and self._read_char_no_rewind(1) == '\n':
                    self._rewind(1)
                    self._inc_current_line()
                elif ch == '\n':
                    self._inc_current_line()
                elif ch == '*' and self._read_char_no_rewind(1) == '/':
                    self._rewind(2)
                    multi_line_comment_offset = -1

                self._rewind(1)

        if quot_ch == 0:
            i = len(value) - 1
            while i >= 0:
                if value[i] != ' ' and value[i] != '\t':
                    break
                i = i - 1
            i = i + 1
            if i < len(value):
                value = value[:i]

        if rewind_to_offset != -1:
            self.file.seek(rewind_to_offset)
        return value

    def _flush_pending_comments(self, data_block, to_params):
        for pending_comment in self.pending_comments:
            comment_key = MAZE_DATA_BLOCK_COMMENT_CPP if pending_comment.cpp_style else MAZE_DATA_BLOCK_COMMENT_C

            comment_text = self._read_text(
                        pending_comment.start_offset, pending_comment.end_offset - pending_comment.start_offset)

            if to_params:
                data_block.add_param_string(comment_key, comment_text)
            else:
                data_block.add_new_data_block(comment_key).add_param_string(comment_key, comment_text)
        self.pending_comments.clear()

    def _process_syntax_error(self, text):
        print("{0} (Line: {1})".format(text, self.current_line))
        return False

    def _inc_current_line(self):
        self.current_line = self.current_line + 1
        self.current_line_offset = self.file.tell()

    def _read_char(self):
        return str(chr(self.file.read(1)[0]))

    def _read_char_no_rewind(self, index=0):
        current_pos = self.file.tell()
        text = self.file.read(3)
        self.file.seek(current_pos)
        return str(chr(text[index]))

    def _read_text_no_rewind(self, size):
        current_pos = self.file.tell()
        text = self.file.read(size)
        self.file.seek(current_pos)
        return ''.join(chr(b) for b in text)

    def _read_text(self, from_pos, size):
        current_pos = self.file.tell()
        self.file.seek(from_pos)
        text = self.file.read(size)
        self.file.seek(current_pos)
        return ''.join(chr(b) for b in text)

    def _can_read_size(self, size):
        return (self.file_size - self.file.tell()) >= size

    def _rewind(self, shift):
        current_pos = self.file.tell()
        self.file.seek(current_pos + shift)

    def _is_eof(self):
        return self.file.tell() == self.file_size
