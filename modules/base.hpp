#include "../pyModule.h"
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