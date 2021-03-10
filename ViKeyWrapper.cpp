#include "ViKeyWrapper.h"

static PyObject* find(PyObject* self, PyObject* args) {
	DWORD count;
	DWORD err = VikeyFind(&count);
	if (err == 0) {
		return Py_BuildValue("(ii)", 1, count);
	}
	else {
		return Py_BuildValue("(ii)", 0, 0);
	}
}

static PyObject* get_HID(PyObject* self, PyObject* args) {
	DWORD hid;
	WORD index;
	int ret = PyArg_ParseTuple(args, "i", &index);
	if (!ret) return NULL;
	DWORD err = VikeyGetHID(index, &hid);
	return Py_BuildValue("kk", err, hid);
}

static PyObject* get_SoftID(PyObject* self, PyObject* args) {
	char sid[9] = { 0 };
	WORD index;
	int ret = PyArg_ParseTuple(args, "i", &index);
	if (!ret) return NULL;
	DWORD err = VikeyGetSoftIDString(index, sid);
	return Py_BuildValue("ks", err, sid);
}

static PyObject* ramdom(PyObject* self, PyObject* args) {
	WORD index;
	int ret = PyArg_ParseTuple(args, "i", &index);
	if (!ret) return NULL;
	WORD d1;
	WORD d2;
	WORD d3;
	WORD d4;
	PyObject* ramdom_data = Py_BuildValue("[]");
	DWORD err = ViKeyRandom(index, &d1, &d2, &d3, &d4);
	if (err == 0) {
		PyList_Append(ramdom_data, Py_BuildValue("H", d1));
		PyList_Append(ramdom_data, Py_BuildValue("H", d2));
		PyList_Append(ramdom_data, Py_BuildValue("H", d3));
		PyList_Append(ramdom_data, Py_BuildValue("H", d4));
	}
	return Py_BuildValue("(kN)", err, ramdom_data);
}

static PyObject* md5(PyObject* self, PyObject* args) {
	const unsigned char* data;
	BYTE pResult[17] = { 0 };  //MD5:16bytes
	WORD index;
	WORD length;
	int ret = PyArg_ParseTuple(args, "yii", &data, &length, &index);
	if (!ret) return NULL;
	DWORD err = VikeyMD5(index, length, const_cast<unsigned char*>(data), pResult);
	return Py_BuildValue("ky", err, pResult);
}

static PyObject* sha1(PyObject* self, PyObject* args) {
	const unsigned char* data;
	BYTE pResult[21] = { 0 };  //SHA1:20bytes
	WORD index;
	WORD length;
	int ret = PyArg_ParseTuple(args, "yii", &data, &length, &index);
	if (!ret) return NULL;
	DWORD err = VikeySHA1(index, length, const_cast<unsigned char*>(data), pResult);
	return Py_BuildValue("ky", err, pResult);
}

static PyObject* hmac_md5(PyObject* self, PyObject* args) {
	const unsigned char* data;
	BYTE pResult[17] = { 0 };  //MD5:16bytes
	WORD index;
	WORD length;
	int ret = PyArg_ParseTuple(args, "yii", &data, &length, &index);
	if (!ret) return NULL;
	DWORD err = VikeyHmacMD5(index, length, const_cast<unsigned char*>(data), pResult);
	return Py_BuildValue("ky", err, pResult);
}

static PyObject* hmac_sha1(PyObject* self, PyObject* args) {
	const unsigned char* data;
	BYTE pResult[21] = { 0 };  //SHA1:20bytes
	WORD index;
	WORD length;
	int ret = PyArg_ParseTuple(args, "yii", &data, &length, &index);
	if (!ret) return NULL;
	DWORD err = VikeyHmacSHA1(index, length, const_cast<unsigned char*>(data), pResult);
	return Py_BuildValue("ky", err, pResult);
}

static PyMethodDef pyViKeyMethods[] = {
	{"VikeyFind", find, METH_VARARGS, "Find available ViKey"},
	{"VikeyGetHID", get_HID, METH_VARARGS, "Get hardware ID"},
	{"VikeyGetSoftIDString", get_SoftID, METH_VARARGS, "Get soft ID"},
	{"ViKeyRandom", ramdom, METH_VARARGS, "Hardware ramdom data"},
	{"VikeyMD5", md5, METH_VARARGS, "MD5"},
	{"VikeySHA1", sha1, METH_VARARGS, "SHA1"},
	{"VikeyHmacMD5", hmac_md5, METH_VARARGS, "SHA1"},
	{"VikeyHmacSHA1", hmac_sha1, METH_VARARGS, "HMAC using SHA1"},
	NULL
};

static struct PyModuleDef pyViKey = {
	PyModuleDef_HEAD_INIT,
	"pyViKey",
	"pyViKey Lib",
	-1,
	pyViKeyMethods
};

PyMODINIT_FUNC
PyInit_pyViKey(void)
{
	return PyModule_Create(&pyViKey);
}