def VikeyFind() -> int: ...


def VikeyGetHID(index: int) -> int: ...


def VikeyGetSoftIDString(index: int) -> str: ...


def VikeyGetType(index: int) -> int: ...


def VikeyGetLevel(index: int) -> int: ...


def VikeyGetPtroductName(index: int) -> str: ...


def ViKeyRandom(index: int) -> list: ...


def VikeyMD5(data: bytes, length: int, index: int) -> bytes: ...


def VikeySHA1(data: bytes, length: int, index: int) -> bytes: ...


def VikeyHmacMD5(data: bytes, length: int, index: int) -> bytes: ...


def VikeyHmacSHA1(data: bytes, length: int, index: int) -> bytes: ...


def VikeyUserLogin(password: str, index: int) -> int: ...


def VikeyAdminLogin(password: str, index: int) -> int: ...


def VikeyLogout(index: int) -> int: ...
