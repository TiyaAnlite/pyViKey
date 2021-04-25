#include "../pyModule.h"
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