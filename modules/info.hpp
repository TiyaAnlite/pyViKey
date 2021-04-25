#include "../pyModule.h"
static PyObject* get_HID(PyObject* self, PyObject* args) {
	DWORD hid;
	WORD index;
	int ret = PyArg_ParseTuple(args, "i", &index);
	if (!ret) return NULL;
	DWORD err = VikeyGetHID(index, &hid);
	return Py_BuildValue("kk", err, hid);
}

static PyObject* get_SoftID(PyObject* self, PyObject* args) {
	char sid[8];  //SoftID:8bit
	WORD index;
	int ret = PyArg_ParseTuple(args, "i", &index);
	if (!ret) return NULL;
	DWORD err = VikeyGetSoftIDString(index, sid);
	return Py_BuildValue("ks#", err, sid, 8);
}

static PyObject* get_type(PyObject* self, PyObject* args) {
	VikeyType type = VikeyType::ViKeyInvalid;
	WORD index;
	int ret = PyArg_ParseTuple(args, "i", &index);
	if (!ret) return NULL;
	DWORD err = VikeyGetType(index, &type);
	return Py_BuildValue("ki", err, type);
}

static PyObject* get_level(PyObject* self, PyObject* args) {
	BYTE user_level = 0;
	WORD index;
	int ret = PyArg_ParseTuple(args, "i", &index);
	if (!ret) return NULL;
	DWORD err = VikeyGetLevel(index, &user_level);
	return Py_BuildValue("kB", err, user_level);
}

static PyObject* get_name(PyObject* self, PyObject* args) {
	WCHAR szName[17];  //8char or 4word = 16bit
	WORD index;
	int ret = PyArg_ParseTuple(args, "i", &index);
	if (!ret) return NULL;
	DWORD err = VikeyGetPtroductName(index, szName);
	return Py_BuildValue("ku", err, szName);
}