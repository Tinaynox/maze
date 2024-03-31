from enum import Enum
import os

from typing import Tuple

S32 = int
S64 = int
U32 = int
U64 = int
F32 = float
F64 = float
Bool = bool
Vec4S8 = Tuple[int, int, int, int]
Vec4U8 = Tuple[int, int, int, int]
Vec2S32 = Tuple[int, int]
Vec3S32 = Tuple[int, int, int]
Vec4S32 = Tuple[int, int, int, int]
Vec2U32 = Tuple[int, int]
Vec3U32 = Tuple[int, int, int]
Vec4U32 = Tuple[int, int, int, int]
Vec2F32 = Tuple[float, float]
Vec3F32 = Tuple[float, float, float]
Vec4F32 = Tuple[float, float, float, float]
Vec2B = Tuple[bool, bool]
Vec3B = Tuple[bool, bool, bool]
Vec4B = Tuple[bool, bool, bool, bool]
Mat3F32 = Tuple[float, float, float,
                float, float, float,
                float, float, float]
Mat4F32 = Tuple[float, float, float, float,
                float, float, float, float,
                float, float, float, float,
                float, float, float, float]
String = str


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
    String = 24


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


def _is_identifier_char(ch):
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
        result = _is_identifier_char(ch)
        if not result:
            break
    return result


def _write_eof(file):
    file.write("\n")


def _write_indent(file, size):
    for i in size:
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


class DataBlock:
    def __init__(self, data_block_name=""):
        assert isinstance(data_block_name, str), "data_block_name must be an str"
        self.name = data_block_name
        self.params = []
        self.data_blocks = []

    # Params
    def add_param(self, param_name, param_type, param_value):
        self.params.append(DataBlockParam(param_name, param_type, param_value))

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

    def create_data_block(self, data_block_name):
        new_data_block = DataBlock(data_block_name)
        self.add_data_block(new_data_block)
        return new_data_block

    def get_data_block(self, data_block_name, default_value=None):
        if default_value is None:
            default_value = DataBlock()
        return next((x for x in self.data_blocks if x.name == data_block_name), default_value)

    # Serialization
    def save_text_file(self, file_path):
        with open(file_path, "w") as file:
            self._save_text_file(file, 0)

    def _save_text_file(self, file, level):
        skip_next_indent = False

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
                    _write_eof(file)
                elif name.startswith(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP) or name.startswith(MAZE_DATA_BLOCK_COMMENT_CPP):
                    file.write("//")
                    file.write(param.value)
                    _write_eof(file)
                continue

            if _is_name_is_simple(name):
                file.write(name)
            else:
                _write_complex_string(file, name)

            file.write(":")
            file.write(data_block_param_type_info[param.type].name)
            file.write(" = ")
            file.write(param.value)

            if i + 1 < len(self.params):
                next_param_name = self.params[i + 1].name
                if (len(next_param_name) >= 3
                    and next_param_name.startswith(MAZE_DATA_BLOCK_COMMENT_PREFIX)
                    and (next_param_name.startswith(MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_C)
                         or next_param_name.startswith(MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_CPP))):
                    file.write(" ")
                    skip_next_indent = True
                    continue
                _write_eof(file)

            if len(self.params) > 0 and len(self.data_blocks) > 0:
                _write_eof(file)
