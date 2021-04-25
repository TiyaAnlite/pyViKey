#pragma once
#include "../pyModule.h"

static PyObject* md5(PyObject* self, PyObject* args) {
	const unsigned char* data;
	BYTE pResult[16];  //MD5:16bytes
	WORD index;
	WORD length;
	int ret = PyArg_ParseTuple(args, "y#i", &data, &length, &index);
	if (!ret) return NULL;
	DWORD err = VikeyMD5(index, length, const_cast<unsigned char*>(data), pResult);
	if (err) {
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	return Py_BuildValue("y#", pResult, 16);
}

static PyObject* sha1(PyObject* self, PyObject* args) {
	const unsigned char* data;
	BYTE pResult[20];  //SHA1:20bytes
	WORD index;
	WORD length;
	int ret = PyArg_ParseTuple(args, "y#i", &data, &length, &index);
	if (!ret) return NULL;
	DWORD err = VikeySHA1(index, length, const_cast<unsigned char*>(data), pResult);
	if (err) {
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	return Py_BuildValue("y#", pResult, 20);
}

static PyObject* hmac_md5(PyObject* self, PyObject* args) {
	const unsigned char* data;
	BYTE pResult[16];  //MD5:16bytes
	WORD index;
	WORD length;
	int ret = PyArg_ParseTuple(args, "y#i", &data, &length, &index);
	if (!ret) return NULL;
	DWORD err = VikeyHmacMD5(index, length, const_cast<unsigned char*>(data), pResult);
	if (err) {
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	return Py_BuildValue("y#", pResult, 16);
}

static PyObject* hmac_sha1(PyObject* self, PyObject* args) {
	const unsigned char* data;
	BYTE pResult[20];  //SHA1:20bytes
	WORD index;
	WORD length;
	int ret = PyArg_ParseTuple(args, "y#i", &data, &length, &index);
	if (!ret) return NULL;
	DWORD err = VikeyHmacSHA1(index, length, const_cast<unsigned char*>(data), pResult);
	if (err) {
		PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
		return NULL;
	}
	return Py_BuildValue("y#", pResult, 20);
}