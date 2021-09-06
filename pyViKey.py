import enum
from functools import wraps

from . import pyViKeyLib as _Lib


def vikey_exception_wrapper(func):
    @wraps(func)
    def exception_wrapper(self, *args, **kwargs):
        try:
            return func(self, *args, **kwargs)
        except RuntimeError as err:
            raise ViKeyError(err.args[0])

    return exception_wrapper


class Identity(enum.Enum):
    """身份状态信息，默认为anonymous权限，部分操作对权限有最低要求\n
    ViKeyIdentity.anonymous: 匿名权限\n
    ViKeyIdentity.user: 用户权限\n
    ViKeyIdentity.admin: 管理员权限"""
    anonymous = 0
    user = 1
    admin = 2


class Type(enum.Enum):
    """标识硬件类型"""
    ViKeyAPP = 0
    ViKeySTD = 1
    ViKeyNET = 2
    ViKeyPRO = 3
    ViKeyWEB = 4
    ViKeyTIME = 5
    ViKeyMultiFunctional = 0x0A
    ViKeyMultiFunctionalTime = 0x0B
    ViKeyInvalid = 0x0C


class TypeCN(enum.Enum):
    """标识硬件类型(中文)"""
    ViKeyAPP = "实用型加密狗"
    ViKeySTD = "标准型加密狗"
    ViKeyNET = "网络型加密狗"
    ViKeyPRO = "专业型加密狗"
    ViKeyWEB = "身份认证型加密狗"
    ViKeyTIME = "时钟型加密狗，内部独立时钟"
    ViKeyMultiFunctional = "多功能加密狗  支持软件加密 支持文档加密"
    ViKeyMultiFunctionalTime = "多功能时钟加密狗"
    ViKeyInvalid = "无效类型"


class ErrorType(enum.Enum):
    """底层库错误码标识"""
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
    VIKEY_CROSS_REGIONAL_WRITE = 0x90000002


class ErrorTypeCN(enum.Enum):
    """底层库错误码标识(中文)"""
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
    # Custom Error
    VIKEY_CHANGED = "当前ViKey已被改变"
    VIKEY_CROSS_REGIONAL_WRITE = "跨用户区写入数据"


class StorageInfo(enum.Enum):
    """ViKey存储空间信息帮助类(单位为字节)"""
    ViKeyAPP = 128
    ViKeySTD = 2048
    ViKeyNET = 2048
    ViKeyPRO = 4096
    ViKeyWEB = 128
    ViKeyTIME = 256


class ViKeyError(RuntimeError):
    """底层库错误的高级封装，提供友好状态信息显示"""

    def __init__(self, errcode):
        try:
            t = ErrorType(errcode)
        except ValueError:
            errcode = 0xFFFFFFFF
            t = ErrorType(errcode)
        self.err_code = errcode
        self.err_type = t
        self.err_msg = ErrorTypeCN[self.err_type.name]

    def __str__(self):
        return f"[0x{hex(self.err_code).upper()[2:]}]{self.err_msg.name}: {self.err_msg.value}"


class PyViKeyPlus:
    """
    ViKey Python库高级封装，提供面向对象支持与错误包装，状态码帮助
    """

    def __init__(self, index: int = None, select=False, auto_select=True):
        """
        :param index: ViKey序号
        :param select: 交互提示选择(index指定时忽略)
        :param auto_select: 在启用交互选择的情况下，自动选择唯一一个硬件
        """
        count = find()
        self.index = index
        if not self.index and select:
            self.__select(count, auto_select)
        if not self.index:
            self.index = 0
        self.HID = 0
        self.softHID = ""
        self.type = None
        self.level = Identity.anonymous
        self.__info_init()

    def __info_init(self):
        try:
            self.HID = _Lib.VikeyGetHID(self.index)
            self.softHID = _Lib.VikeyGetSoftIDString(self.index)
            t = Type(_Lib.VikeyGetType(self.index))
            self.type = (t.value, t.name, TypeCN[t.name].value)  # (typeId, typeName, typeNameCN)
            self.level = Identity(_Lib.VikeyGetLevel(self.index))
        except RuntimeError as err:
            raise ViKeyError(err.args[0])

    def __del__(self):
        try:
            self.__logout()
        except:
            pass

    def __select(self, count: int, auto_select=True):
        """交互选择模式下，选择多个硬件"""
        hardware = []
        for i in range(count):
            info = f"[ViKeyPlus]ViKey[{i}] - "
            try:
                HID = _Lib.VikeyGetHID(i)
            except RuntimeError as err:
                info += f"err: {ErrorType(err.args[0]).name}"
            else:
                info += str(HID)
            hardware.append(info)
        [print(i) for i in hardware]
        if auto_select and len(hardware) == 1:
            self.index = 0
            return
        while True:
            s = input("Select>>> ")
            if s.isdigit() and int(s) < len(hardware):
                self.index = int(s)
                return

    def __check(self):
        if self.HID != _Lib.VikeyGetHID(self.index):
            raise RuntimeError(0x90000001)

    def __get_level(self) -> Identity:
        self.__check()
        self.level = Identity(_Lib.VikeyGetLevel(self.index))
        return self.level

    def __get_name(self) -> str:
        self.__check()
        if Identity.user.value > self.__get_level().value:  # 权限检查
            self.__login_user(self.softHID)
        return _Lib.VikeyGetPtroductName(self.index)

    def __login_user(self, password: str) -> Identity:
        self.__check()
        self.level = Identity(_Lib.VikeyUserLogin(password, self.index))
        return self.level

    def __login_admin(self, password: str) -> Identity:
        self.__check()
        self.level = Identity(_Lib.VikeyAdminLogin(password, self.index))
        return self.level

    def __logout(self) -> Identity:
        self.__check()
        self.level = Identity(_Lib.VikeyLogout(self.index))
        return self.level

    @vikey_exception_wrapper
    def get_level(self) -> Identity:
        """获取当前登录权限"""
        return self.__get_level()

    @vikey_exception_wrapper
    def get_name(self) -> str:
        """获取硬件名"""
        return self.__get_name()

    @vikey_exception_wrapper
    def login(self, identity: Identity = Identity.user, password: str = None):
        """
        登录硬件为指定权限\n
        :param identity: ViKeyIdentity权限类
        :param password: 密码
        """
        if not password:
            password = self.softHID
        self.__check()
        if identity == Identity.anonymous:
            self.level = Identity(self.__logout())
        elif identity == Identity.user:
            self.level = Identity(self.__login_user(password))
        elif identity == Identity.admin:
            self.level = Identity(self.__login_admin(password))
        return self.level

    @vikey_exception_wrapper
    def logout(self):
        """登出硬件，返回匿名状态"""
        return self.__logout()

    @vikey_exception_wrapper
    def get_random_number(self) -> tuple:
        """返回一组随机数(4个)"""
        self.__check()
        return _Lib.ViKeyRandom(self.index)

    @vikey_exception_wrapper
    def random_number_generator(self):
        """随机数生成器"""
        while True:
            self.__check()
            for n in _Lib.ViKeyRandom(self.index):
                yield n

    @vikey_exception_wrapper
    def md5(self, data: bytes) -> bytes:
        """
        MD5哈希算法\n
        :param data: 输入数据
        :return: 输出数据
        """
        self.__check()
        return _Lib.VikeyMD5(data, self.index)

    @vikey_exception_wrapper
    def sha1(self, data: bytes) -> bytes:
        """
        SHA1哈希算法\n
        :param data: 输入数据
        :return: 输出数据
        """
        self.__check()
        return _Lib.VikeySHA1(data, self.index)

    @vikey_exception_wrapper
    def hmac_md5(self, data: bytes) -> bytes:
        """
        HMAC哈希验证算法(MD5)\n
        :param data: 输入数据
        :return: 输出数据
        """
        self.__check()
        return _Lib.VikeyHmacMD5(data, self.index)

    @vikey_exception_wrapper
    def hmac_sha1(self, data: bytes) -> bytes:
        """
        HMAC哈希验证算法(SHA1)\n
        :param data: 输入数据
        :return: 输出数据
        """
        self.__check()
        return _Lib.VikeyHmacSHA1(data, self.index)

    @vikey_exception_wrapper
    def encrypt_des(self, data: bytes) -> bytes:
        """
        DES对称加密\n
        :param data: 输入数据
        :return: 输出数据
        """
        self.__check()
        return _Lib.VikeyDesEncrypt(data, self.index)

    @vikey_exception_wrapper
    def decrypt_des(self, data: bytes) -> bytes:
        """
        DES对称解密\n
        :param data: 输入数据
        :return: 输出数据
        """
        self.__check()
        return _Lib.VikeyDesDecrypt(data, self.index)

    @vikey_exception_wrapper
    def encrypt_3des(self, data: bytes) -> bytes:
        """
        3DES对称加密\n
        :param data: 输入数据
        :return: 输出数据
        """
        self.__check()
        return _Lib.Vikey3DesEncrypt(data, self.index)

    @vikey_exception_wrapper
    def decrypt_3des(self, data: bytes) -> bytes:
        """
        3DES对称解密\n
        :param data: 输入数据
        :return: 输出数据
        """
        self.__check()
        return _Lib.Vikey3DesDecrypt(data, self.index)

    @vikey_exception_wrapper
    def read_data(self, address: int, length: int) -> bytes:
        """
        读取存储区数据\n
        :param address: 起始地址
        :param length: 长度
        :return: 返回数据
        """
        self.__check()
        return _Lib.VikeyReadData(address, length, self.index)

    @vikey_exception_wrapper
    def write_data(self, address: int, data: bytes) -> int:
        """
        写入存储区数据\n
        当ViKey的为前五个支持类型时，会先进行读写区越界检查\n
        否则将尝试直接写入\n
        :param address: 起始地址
        :param data: 写入数据
        :return: 写入的数据长度
        """
        if self.type[0] <= Type.ViKeyTIME.value:
            # 边界检查区域
            user_capacity = getattr(StorageInfo, self.type[1]).value / 2
            if address < user_capacity <= address + len(data) - 1:
                # 写入数据超出用户区
                raise RuntimeError(0x90000002)
        self.__check()
        return _Lib.VikeyWriteData(address, data, self.index)


def find() -> int:
    """
    查找可用的ViKey\n
    :return: 可用的ViKey数量
    """
    try:
        return _Lib.VikeyFind()
    except RuntimeError as err:
        raise ViKeyError(err.args[0])
