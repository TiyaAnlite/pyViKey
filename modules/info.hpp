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