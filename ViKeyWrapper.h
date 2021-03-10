#pragma once
#define PY_SSIZE_T_CLEAN
//#define Py_LIMITED_API
#include <Python.h>

//#include <stdio.h>
//#include <tchar.h>
//#include <windows.h>

#include "lib/Vikey.h"

/*
#ifdef _WIN32
#ifdef _M_IX86
#pragma comment (lib, "lib/ViKey.lib")
#endif // _M_IX86  

#ifdef _M_X64
#pragma comment (lib, "lib/ViKey_x86.lib")
#endif  // _M_X64
#endif  //_WIN32

#ifdef linux
#pragma comment (lib, "lib/ViKey.a")
#endif  //linux
*/

static PyObject* find(PyObject* self, PyObject* args);
static PyObject* get_HID(PyObject* self, PyObject* args);
static PyObject* get_SoftID(PyObject* self, PyObject* args);
static PyObject* ramdom(PyObject* self, PyObject* args);
static PyObject* md5(PyObject* self, PyObject* args);
static PyObject* sha1(PyObject* self, PyObject* args);
static PyObject* hmac_md5(PyObject* self, PyObject* args);
static PyObject* hmac_sha1(PyObject* self, PyObject* args);