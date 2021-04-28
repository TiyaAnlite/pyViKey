#pragma once
#include "../pyModule.h"
//#include <iostream>

static WORD padding(const BYTE* src, WORD srcLen, BYTE* &buffer)
{
    // PKCS#5
    BYTE paddNum = 8 - srcLen % 8;
    WORD bufferLen = srcLen + paddNum;

    //extend memory
    //std::cout << "Input: " << srcLen << std::endl;
    //std::cout << "Padding: " << (WORD)paddNum << std::endl;
    //std::cout << "new Buffer: " << bufferLen << std::endl;
    buffer = new BYTE[bufferLen];
    memcpy(buffer, src, srcLen);

    for (int i = 0; i < paddNum; ++i) {
        buffer[srcLen + i] = paddNum;
    }
    return bufferLen;
}

static WORD padd_num(WORD srcLen) {
    // PKCS#5
    return 8 - srcLen % 8;
}

static PyObject* des_encrypt(PyObject* self, PyObject* args) {
    WORD index;
    const BYTE* src;
    WORD srcLen;
    WORD outLen;  //not used
    if (!PyArg_ParseTuple(args, "y#i", &src, &srcLen, &index)) return NULL;
    WORD bufferLen = srcLen + padd_num(srcLen);
    BYTE* buffer = new BYTE[bufferLen];
    DWORD err = VikeyDesEncrypt(index, srcLen, const_cast<BYTE*>(src), buffer, &outLen);
    if (err) {
        delete[] buffer;
        PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
        return NULL;
    }
    PyObject* o = Py_BuildValue("y#", buffer, bufferLen);
    delete[] buffer;
    return o;
}

static PyObject* des_decrypt(PyObject* self, PyObject* args) {
    WORD index;
    WORD length;
    const BYTE* src;
    WORD outLen;
    if (!PyArg_ParseTuple(args, "y#i", &src, &length, &index)) return NULL;
    BYTE* buffer = new BYTE[length];
    DWORD err = VikeyDesDecrypt(index, length, const_cast<BYTE*>(src), buffer, &outLen);
    if (err) {
        delete[] buffer;
        PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
        return NULL;
    }
    PyObject* o = Py_BuildValue("y#", buffer, outLen);  //de-padding
    delete[] buffer;
    return o;
}

static PyObject* des3_encrypt(PyObject* self, PyObject* args) {
    WORD index;
    const BYTE* src;
    WORD srcLen;
    if (!PyArg_ParseTuple(args, "y#i", &src, &srcLen, &index)) return NULL;
    BYTE* inBuffer;
    WORD bufferLen = padding(src, srcLen, inBuffer);
    BYTE* outBuffer = new BYTE[bufferLen];
    DWORD err = Vikey3DesEncrypt(index, bufferLen, inBuffer, outBuffer);
    if (err) {
        delete[] inBuffer;
        delete[] outBuffer;
        PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
        return NULL;
    }
    PyObject* o = Py_BuildValue("y#", outBuffer, bufferLen);
    delete[] inBuffer;
    delete[] outBuffer;
    return o;
}

static PyObject* des3_decrypt(PyObject* self, PyObject* args) {
    WORD index;
    WORD length;
    const BYTE* src;
    if (!PyArg_ParseTuple(args, "y#i", &src, &length, &index)) return NULL;
    BYTE* buffer = new BYTE[length];
    DWORD err = Vikey3DesDecrypt(index, length, const_cast<BYTE*>(src), buffer);
    if (err) {
        delete[] buffer;
        PyErr_SetObject(PyExc_RuntimeError, Py_BuildValue("k", err));
        return NULL;
    }
    //std::cout << "Pad-flag: " << (WORD)buffer[length - 1] << std::endl;
    WORD outLen = length - buffer[length - 1];
    //std::cout << "outLen: " << outLen << std::endl;
    PyObject* o = Py_BuildValue("y#", buffer, outLen);  //de-padding
    delete[] buffer;
    return o;
}