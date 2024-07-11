S32 = int
S64 = int
U32 = int
U64 = int
F32 = float
F64 = float
Bool = bool


class Vec2:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y


class Vec3:
    def __init__(self, x=0, y=0, z=0):
        self.x = x
        self.y = y
        self.z = z


class Vec4:
    def __init__(self, x=0, y=0, z=0, w=0):
        self.x = x
        self.y = y
        self.z = z
        self.w = w


class Vec4S8(Vec4):
    def __init__(self, x=0, y=0, z=0, w=0):
        super().__init__(x, y, z, w)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 4:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y, z, w = map(int, parts)
        return cls(x, y, z, w)


class Vec4U8(Vec4):
    def __init__(self, x=0, y=0, z=0, w=0):
        super().__init__(x, y, z, w)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 4:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y, z, w = map(int, parts)
        return cls(x, y, z, w)


class Vec2S32(Vec2):
    def __init__(self, x=0, y=0):
        super().__init__(x, y)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 2:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y = map(int, parts)
        return cls(x, y)


class Vec3S32(Vec3):
    def __init__(self, x=0, y=0, z=0):
        super().__init__(x, y, z)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 3:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y, z = map(int, parts)
        return cls(x, y, z)


class Vec4S32(Vec4):
    def __init__(self, x=0, y=0, z=0, w=0):
        super().__init__(x, y, z, w)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 4:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y, z, w = map(int, parts)
        return cls(x, y, z, w)


class Vec2U32(Vec2):
    def __init__(self, x=0, y=0):
        super().__init__(x, y)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 2:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y = map(int, parts)
        return cls(x, y)


class Vec3U32(Vec3):
    def __init__(self, x=0, y=0, z=0):
        super().__init__(x, y, z)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 3:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y, z = map(int, parts)
        return cls(x, y, z)


class Vec4U32(Vec4):
    def __init__(self, x=0, y=0, z=0, w=0):
        super().__init__(x, y, z, w)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 4:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y, z, w = map(int, parts)
        return cls(x, y, z, w)


class Vec2F32(Vec2):
    def __init__(self, x=0.0, y=0.0):
        super().__init__(x, y)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 2:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y = map(float, parts)
        return cls(x, y)


class Vec3F32(Vec3):
    def __init__(self, x=0.0, y=0.0, z=0.0):
        super().__init__(x, y, z)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 3:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y, z = map(float, parts)
        return cls(x, y, z)


class Vec4F32(Vec4):
    def __init__(self, x=0.0, y=0.0, z=0.0, w=0.0):
        super().__init__(x, y, z, w)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 4:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y, z, w = map(float, parts)
        return cls(x, y, z, w)


class Vec2B(Vec2):
    def __init__(self, x=False, y=False):
        super().__init__(x, y)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 2:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y = map(Bool, parts)
        return cls(x, y)


class Vec3B(Vec3):
    def __init__(self, x=True, y=True, z=True):
        super().__init__(x, y, z)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 3:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y, z = map(Bool, parts)
        return cls(x, y, z)


class Vec4B(Vec4):
    def __init__(self, x=True, y=True, z=True, w=True):
        super().__init__(x, y, z, w)

    @classmethod
    def from_str(cls, s):
        parts = s.split(',')
        if len(parts) != 4:
            raise ValueError("Failed to parse vector - {0}".format(s))
        x, y, z, w = map(Bool, parts)
        return cls(x, y, z, w)


class Mat3F32:
    def __init__(self,
                 m00=0.0, m01=0.0, m02=0.0,
                 m10=0.0, m11=0.0, m12=0.0,
                 m20=0.0, m21=0.0, m22=0.0):
        self.m00 = m00
        self.m01 = m01
        self.m02 = m02
        self.m10 = m10
        self.m11 = m11
        self.m12 = m12
        self.m20 = m20
        self.m21 = m21
        self.m22 = m22

    @classmethod
    def from_str(cls, s):
        rows = s.strip()[1:-1].replace(' ', '').split('][')
        elements = [float(num) for row in rows for num in row.strip('[]').split(',')]

        if len(elements) != 9:
            raise ValueError("Failed to parse vector - {0}".format(s))

        return cls(*elements)


class Mat4F32:
    def __init__(self,
                 m00=0.0, m01=0.0, m02=0.0, m03=0.0,
                 m10=0.0, m11=0.0, m12=0.0, m13=0.0,
                 m20=0.0, m21=0.0, m22=0.0, m23=0.0,
                 m30=0.0, m31=0.0, m32=0.0, m33=0.0):
        self.m00 = m00
        self.m01 = m01
        self.m02 = m02
        self.m03 = m03
        self.m10 = m10
        self.m11 = m11
        self.m12 = m12
        self.m13 = m13
        self.m20 = m20
        self.m21 = m21
        self.m22 = m22
        self.m23 = m23
        self.m30 = m30
        self.m31 = m31
        self.m32 = m32
        self.m33 = m33

    @classmethod
    def from_str(cls, s):
        rows = s.strip(' ')[1:-1].replace(' ', '').split('][')
        elements = [float(num) for row in rows for num in row.strip('[]').split(',')]

        if len(elements) != 16:
            raise ValueError("Failed to parse vector - {0}".format(s))

        return cls(*elements)


class TMat:
    def __init__(self,
                 m00=0.0, m01=0.0, m02=0.0,
                 m10=0.0, m11=0.0, m12=0.0,
                 m20=0.0, m21=0.0, m22=0.0,
                 m30=0.0, m31=0.0, m32=0.0):
        self.m00 = m00
        self.m01 = m01
        self.m02 = m02
        self.m10 = m10
        self.m11 = m11
        self.m12 = m12
        self.m20 = m20
        self.m21 = m21
        self.m22 = m22
        self.m30 = m30
        self.m31 = m31
        self.m32 = m32

    @classmethod
    def from_str(cls, s):
        rows = s.strip(' ')[1:-1].replace(' ', '').split('][')
        elements = [float(num) for row in rows for num in row.strip('[]').split(',')]

        if len(elements) != 12:
            raise ValueError("Failed to parse vector - {0}".format(s))

        return cls(*elements)


String = str
