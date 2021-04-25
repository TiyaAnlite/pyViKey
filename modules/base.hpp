#pragma once
#include "../pyModule.h"

static PyObject* find(PyObject* self, PyObject* args) {
	DWORD count;
	DWORD err = VikeyFind(&count);
	if (err) {
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	return Py_BuildValue("i", count);
}

static PyObject* login_user(PyObject* self, PyObject* args) {
	WORD index;
	BYTE user_level = 0;
	const char* password;
	int ret = PyArg_ParseTuple(args, "si", &password, &index);
	if (!ret) return NULL;
	DWORD err = VikeyUserLogin(index, const_cast<char*>(password));
	if (err) {
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	else {
		err = VikeyGetLevel(index, &user_level);
		if (err) {
			PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
			return NULL;
		}
	}
	return Py_BuildValue("B", user_level);
}

static PyObject* login_admin(PyObject* self, PyObject* args) {
	WORD index;
	BYTE user_level = 0;
	const char* password;
	int ret = PyArg_ParseTuple(args, "si", &password, &index);
	if (!ret) return NULL;
	DWORD err = VikeyAdminLogin(index, const_cast<char*>(password));
	if (err) {
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	else {
		err = VikeyGetLevel(index, &user_level);
		if (err) {
			PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
			return NULL;
		}
	}
	return Py_BuildValue("B", user_level);
}

static PyObject* logout(PyObject* self, PyObject* args) {
	WORD index;
	BYTE user_level = 0;
	int ret = PyArg_ParseTuple(args, "i", &index);
	if (!ret) return NULL;
	DWORD err = VikeyLogoff(index);
	if (err) {
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	else {
		err = VikeyGetLevel(index, &user_level);
		if (err) {
			PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
			return NULL;
		}
	}
	return Py_BuildValue("B", user_level);
}