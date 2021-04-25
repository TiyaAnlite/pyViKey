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

static PyObject* login_user(PyObject* self, PyObject* args) {
	WORD index;
	BYTE user_level = 0;
	const char* password;
	int ret = PyArg_ParseTuple(args, "yi", &password, &index);
	if (!ret) return NULL;
	DWORD err = VikeyUserLogin(index, const_cast<char*>(password));
	if (!err) {
		DWORD err = VikeyGetLevel(index, &user_level);
	}
	return Py_BuildValue("kB", err, user_level);
}

static PyObject* login_admin(PyObject* self, PyObject* args) {
	WORD index;
	BYTE user_level = 0;
	const char* password;
	int ret = PyArg_ParseTuple(args, "yi", &password, &index);
	if (!ret) return NULL;
	DWORD err = VikeyAdminLogin(index, const_cast<char*>(password));
	if (!err) {
		DWORD err = VikeyGetLevel(index, &user_level);
	}
	return Py_BuildValue("kB", err, user_level);
}

static PyObject* logout(PyObject* self, PyObject* args) {
	WORD index;
	BYTE user_level = 0;
	int ret = PyArg_ParseTuple(args, "i", &index);
	if (!ret) return NULL;
	DWORD err = VikeyLogoff(index);
	if (!err) {
		DWORD err = VikeyGetLevel(index, &user_level);
	}
	return Py_BuildValue("kB", err, user_level);
}