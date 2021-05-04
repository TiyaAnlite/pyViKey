import enum
from functools import wraps

import pyViKeyLib as _Lib


def vikey_exception_wrapper(func):
    @wraps(func)
    def exception_wrapper(self, *args, **kwargs):
        try:
            return func(self, *args, **kwargs)
        except RuntimeError as err:
            raise ViKeyError(err.args[0])

    return exception_wrapper


@enum.unique
class ViKeyIdentity(enum.Enum):
    anonymous = 0
    user = 1
    admin = 2


@enum.unique
class ViKeyType(enum.Enum):
    ViKeyAPP = 0
    ViKeySTD = 1
    ViKeyNET = 2
    ViKeyPRO = 3
    ViKeyWEB = 4
    ViKeyTIME = 5
    ViKeyMultiFunctional = 0x0A
    ViKeyMultiFunctionalTime = 0x0B
    ViKeyInvalid = 0x0C


@enum.unique
class ViKeyTypeCN(enum.Enum):
    ViKeyAPP = "实用型加密狗"
    ViKeySTD = "标准型加密狗"
    ViKeyNET = "网络型加密狗"
    ViKeyPRO = "专业型加密狗"
    ViKeyWEB = "身份认证型加密狗"
    ViKeyTIME = "时钟型加密狗，内部独立时钟"
    ViKeyMultiFunctional = "多功能加密狗  支持软件加密 支持文档加密"
    ViKeyMultiFunctionalTime = "多功能时钟加密狗"
    ViKeyInvalid = "无效类型"


@enum.unique
class ViKeyErrorType(enum.Enum):
    VIKEY_SUCCESS = 0x00000000
    VIKEY_ERROR_NO_VIKEY = 0x80000001
    VIKEY_ERROR_INVALID_PASSWORD = 0x80000002
    VIKEY_ERROR_NEED_FIND = 0x80000003
    VIKEY_ERROR_INVALID_INDEX = 0x80000004
    VIKEY_ERROR_INVALID_VALUE = 0x80000005
    VIKEY_ERROR_INVALID_KEY = 0x80000006
    VIKEY_ERROR_GET_VALUE = 0x80000007
    VIKEY_ERROR_SET_VALUE = 0x80000008
    VIKEY_ERROR_NO_CHANCE = 0x80000009
    VIKEY_ERROR_NO_TAUTHORITY = 0x8000000A
    VIKEY_ERROR_INVALID_ADDR_OR_SIZE = 0x8000000B
    VIKEY_ERROR_RANDOM = 0x8000000C
    VIKEY_ERROR_SEED = 0x8000000D
    VIKEY_ERROR_CONNECTION = 0x8000000E
    VIKEY_ERROR_CALCULATE = 0x8000000F
    VIKEY_ERROR_MODULE = 0x80000010
    VIKEY_ERROR_GENERATE_NEW_PASSWORD = 0x80000011
    VIKEY_ERROR_ENCRYPT_FAILED = 0x80000012
    VIKEY_ERROR_DECRYPT_FAILED = 0x80000013
    VIKEY_ERROR_ALREADY_LOCKED = 0x80000014
    VIKEY_ERROR_UNKNOWN_COMMAND = 0x80000015
    VIKEY_ERROR_NO_SUPPORT = 0x80000016
    VIKEY_ERROR_CATCH = 0x80000017
    VIKEY_ERROR_UNKNOWN_ERROR = 0xFFFFFFFF
    # Custom Error
    VIKEY_CHANGED = 0x90000001


@enum.unique
class ViKeyErrorTypeCN(enum.Enum):
    VIKEY_SUCCESS = "成功"
    VIKEY_ERROR_NO_VIKEY = "没有找到ViKey加密锁"
    VIKEY_ERROR_INVALID_PASSWORD = "密码错误"
    VIKEY_ERROR_NEED_FIND = "请先查找加密锁"
    VIKEY_ERROR_INVALID_INDEX = "无效的句柄"
    VIKEY_ERROR_INVALID_VALUE = "数值错误"
    VIKEY_ERROR_INVALID_KEY = "秘钥无效"
    VIKEY_ERROR_GET_VALUE = "读取信息错误"
    VIKEY_ERROR_SET_VALUE = "设置信息错误"
    VIKEY_ERROR_NO_CHANCE = "没有机会"
    VIKEY_ERROR_NO_TAUTHORITY = "权限不足"
    VIKEY_ERROR_INVALID_ADDR_OR_SIZE = "地址或长度错误"
    VIKEY_ERROR_RANDOM = "获取随机数错误"
    VIKEY_ERROR_SEED = "获取种子错误"
    VIKEY_ERROR_CONNECTION = "通信错误"
    VIKEY_ERROR_CALCULATE = "算法或计算错误"
    VIKEY_ERROR_MODULE = "产生密码错误"
    VIKEY_ERROR_ENCRYPT_FAILED = "加密数据错误"
    VIKEY_ERROR_DECRYPT_FAILED = "解密数据错误"
    VIKEY_ERROR_ALREADY_LOCKED = "ViKey加密锁已经被锁定"
    VIKEY_ERROR_UNKNOWN_COMMAND = "无效的命令"
    VIKEY_ERROR_NO_SUPPORT = "当前ViKey加密锁不支持此功能"
    VIKEY_ERROR_CATCH = "发生异常"
    VIKEY_ERROR_UNKNOWN_ERROR = "未知错误"
    VIKEY_CHANGED = "当前ViKey已被改变"


class ViKeyError(RuntimeError):

    def __init__(self, errcode):
        try:
            t = ViKeyErrorType(errcode)
        except ValueError:
            errcode = 0xFFFFFFFF
            t = ViKeyErrorType(errcode)
        self.err_code = errcode
        self.err_type = t
        self.err_msg = ViKeyErrorTypeCN[self.err_type.name]

    def __str__(self):
        return f"[0x{hex(self.err_code).upper()[2:]}]{self.err_msg.name}: {self.err_msg.value}"


class PyViKeyPlus:

    def __init__(self, index: int = 0):
        find()
        self.index = index
        self.HID = 0
        self.softHID = ""
        self.type = None
        self.level = ViKeyIdentity.anonymous
        self.__info_init()

    def __info_init(self):
        try:
            self.HID = _Lib.VikeyGetHID(self.index)
            self.softHID = _Lib.VikeyGetSoftIDString(self.index)
            t = ViKeyType(_Lib.VikeyGetType(self.index))
            self.type = (t.value, t.name, ViKeyTypeCN[t.name].value)
            self.level = ViKeyIdentity(_Lib.VikeyGetLevel(self.index))
        except RuntimeError as err:
            raise ViKeyError(err.args[0])

    def __del__(self):
        try:
            self.__logout()
        except:
            pass

    def __check(self):
        if self.HID != _Lib.VikeyGetHID(self.index):
            raise RuntimeError(0x90000001)

    def __get_level(self):
        self.__check()
        self.level = ViKeyIdentity(_Lib.VikeyGetLevel(self.index))
        return self.level

    def __login_user(self, password: str):
        self.__check()
        self.level = ViKeyIdentity(_Lib.VikeyUserLogin(password, self.index))
        return self.level

    def __login_admin(self, password: str):
        self.__check()
        self.level = ViKeyIdentity(_Lib.VikeyAdminLogin(password, self.index))
        return self.level

    def __logout(self):
        self.__check()
        self.level = ViKeyIdentity(_Lib.VikeyLogout(self.index))
        return self.level

    @vikey_exception_wrapper
    def get_level(self):
        return self.__get_level()

    @vikey_exception_wrapper
    def login(self, identity: ViKeyIdentity = ViKeyIdentity.user, password: str = None):
        if not password:
            password = self.softHID
        self.__check()
        if identity == ViKeyIdentity.anonymous:
            self.level = ViKeyIdentity(self.__logout())
        elif identity == ViKeyIdentity.user:
            self.level = ViKeyIdentity(self.__login_user(password))
        elif identity == ViKeyIdentity.admin:
            self.level = ViKeyIdentity(self.__login_admin(password))
        return self.level

    @vikey_exception_wrapper
    def logout(self):
        return self.__logout()

    @vikey_exception_wrapper
    def random_number(self):
        self.__check()
        return _Lib.ViKeyRandom(self.index)

    @vikey_exception_wrapper
    def random_number_generator(self):
        while True:
            self.__check()
            for n in _Lib.ViKeyRandom(self.index):
                yield n


def find() -> int:
    """
    查找可用的ViKey\n
    :return: 可用的ViKey数量
    """
    try:
        return _Lib.VikeyFind()
    except RuntimeError as err:
        raise ViKeyError(err.args[0])
