#include "pyModule.h"
#include "modules/module.hpp"

static PyMethodDef pyViKeyLibMethods[] = {
	{"VikeyFind", find, METH_VARARGS, "Find available ViKey"},
	{"VikeyGetHID", get_HID, METH_VARARGS, "Get hardware ID"},
	{"VikeyGetSoftIDString", get_SoftID, METH_VARARGS, "Get soft ID"},
	{"VikeyGetType", get_type, METH_VARARGS, "Get hardware type"},
	{"VikeyGetLevel", get_level, METH_VARARGS, "Get user level"},
	{"VikeyGetPtroductName", get_name, METH_VARARGS, "Get ptroduct name"},
	{"ViKeyRandom", ramdom, METH_VARARGS, "Hardware ramdom data"},
	{"VikeyMD5", md5, METH_VARARGS, "MD5"},
	{"VikeySHA1", sha1, METH_VARARGS, "SHA1"},
	{"VikeyHmacMD5", hmac_md5, METH_VARARGS, "SHA1"},
	{"VikeyHmacSHA1", hmac_sha1, METH_VARARGS, "HMAC using SHA1"},
	{"VikeyUserLogin", login_user, METH_VARARGS, "Login ViKey by user"},
	{"VikeyAdminLogin", login_admin, METH_VARARGS, "Login ViKey by admin"},
	{"VikeyLogout", logout, METH_VARARGS, "Logout ViKey"},
	NULL
};

static struct PyModuleDef pyViKeyLib = {
	PyModuleDef_HEAD_INIT,
	"pyViKeyLib",
	"pyViKey low-level interface",
	-1,
	pyViKeyLibMethods
};

PyMODINIT_FUNC
PyInit_pyViKeyLib(void)
{
	return PyModule_Create(&pyViKeyLib);
}