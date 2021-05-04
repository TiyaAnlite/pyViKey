#pragma once
#include "../pyModule.h"

static PyObject* ramdom(PyObject* self, PyObject* args) {
	WORD index;
	if (!PyArg_ParseTuple(args, "i", &index)) return NULL;
	WORD d1;
	WORD d2;
	WORD d3;
	WORD d4;
	PyObject* ramdom_data = Py_BuildValue("[]");
	DWORD err = ViKeyRandom(index, &d1, &d2, &d3, &d4);
	if (err) {
		Py_DECREF(ramdom_data);
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	/*
	else {
		PyList_Append(ramdom_data, Py_BuildValue("H", d1));
		PyList_Append(ramdom_data, Py_BuildValue("H", d2));
		PyList_Append(ramdom_data, Py_BuildValue("H", d3));
		PyList_Append(ramdom_data, Py_BuildValue("H", d4));
	}
	return Py_BuildValue("N", ramdom_data);
	*/
	return Py_BuildValue("(HHHH)", d1, d2, d3, d4);
}

static PyObject* read_data(PyObject* self, PyObject* args) {
	WORD index;
	WORD addr;
	WORD length;
	if (!PyArg_ParseTuple(args, "iii", &addr, &length, &index)) return NULL;
	BYTE* buffer = new BYTE[length]; 
	DWORD err = VikeyReadData(index, addr, length, buffer);
	if (err) {
		delete[] buffer;
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	PyObject* o = Py_BuildValue("y#", buffer, length);
	delete[] buffer;
	return o;
}

static PyObject* write_data(PyObject* self, PyObject* args) {
	WORD index;
	WORD addr;
	WORD length;
	const BYTE* buffer;
	if (!PyArg_ParseTuple(args, "iy#i", &addr, &buffer, &length, &index)) return NULL;
	DWORD err = VikeyWriteData(index, addr, length, const_cast<BYTE*>(buffer));
	if (err) {
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	return Py_BuildValue("h", length);
}