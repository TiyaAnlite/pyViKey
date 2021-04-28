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
	{"VikeyReadData", read_data, METH_VARARGS, "Read data from hardware"},
	{"VikeyWriteData", write_data, METH_VARARGS, "Write data to hardware"},
	{"VikeyMD5", md5, METH_VARARGS, "MD5"},
	{"VikeySHA1", sha1, METH_VARARGS, "SHA1"},
	{"VikeyHmacMD5", hmac_md5, METH_VARARGS, "SHA1"},
	{"VikeyHmacSHA1", hmac_sha1, METH_VARARGS, "HMAC using SHA1"},
	{"VikeyDesEncrypt", des_encrypt, METH_VARARGS, "DES encrypt"},
	{"VikeyDesDecrypt", des_decrypt, METH_VARARGS, "DES decrypt"},
	{"Vikey3DesEncrypt", des3_encrypt, METH_VARARGS, "3DES decrypt"},
	{"Vikey3DesDecrypt", des3_decrypt, METH_VARARGS, "3DES decrypt"},
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