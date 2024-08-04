#include <Python.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    long long begin;
    long long current;
    long long end;
    long long step;
} RangeObject;

static PyTypeObject RangeObject_Type;

static PyMemberDef RangeObject_members[] = {
    {"begin", T_INT, offsetof(RangeObject, begin), 0, "beginning of range"},
    {"end", T_INT, offsetof(RangeObject, end), 0, "end of range"},
    {"step", T_INT, offsetof(RangeObject, step), 0, "step of increase"},
    {NULL}  /* Sentinel */
};

static RangeObject * 
newRangeObject(PyObject* args) {
    RangeObject* self;
    self = PyObject_NEW(RangeObject, &RangeObject_Type);
    return self;
}

static int
rangeObjectInit(RangeObject* self, PyObject* args)
{
    long long begin, end, step = 1;

    if(!PyArg_ParseTuple(args, "LL|L", &begin, &end, &step)) {
        PyErr_SetString(PyExc_TypeError, "parameter error");
        return -1;
    }

    self->current = begin;
    self->begin = begin;
    self->step = step;
    self->end = end;

    return 0;
}

static void
rangeObjectDealloc(RangeObject* self) {
    PyObject_Free(self);
}

static PyObject *
rangeObjectIter(PyObject* self) {
    RangeObject* range = (RangeObject*) self;
    Py_IncRef(self);
    range->current = range->begin;
    return self;
}

static PyObject *
rangeObjectIterNext(PyObject* self) {
    RangeObject* range = (RangeObject*) self;
    if (range->current >= range->end) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
    long long current = range->current;
    range->current += range->step;
    return PyLong_FromLongLong(current);
}

static PyTypeObject RangeObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "range_module.RangeObject",        /*tp_name*/
    sizeof(RangeObject),               /*tp_basicsize*/
    0,                                 /*tp_itemsize*/
    /* methods */
    (destructor)rangeObjectDealloc,    /*tp_dealloc*/
    0,                                 /*tp_vectorcall_offset*/
    (getattrfunc)0,                    /*tp_getattr*/
    (setattrfunc)0,                    /*tp_setattr*/
    0,                                 /*tp_as_async*/
    0,                                 /*tp_repr*/
    0,                                 /*tp_as_number*/
    0,                                 /*tp_as_sequence*/
    0,                                 /*tp_as_mapping*/
    0,                                 /*tp_hash*/
    0,                                 /*tp_call*/
    0,                                 /*tp_str*/
    0,                                 /*tp_getattro*/     
    0,                                 /*tp_setattro*/
    0,                                 /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,                /*tp_flags*/
    0,                                 /*tp_doc*/
    0,                                 /*tp_traverse*/
    0,                                 /*tp_clear*/
    0,                                 /*tp_richcompare*/
    0,                                 /*tp_weaklistoffset*/
    rangeObjectIter,                   /*tp_iter*/
    (iternextfunc)rangeObjectIterNext, /*tp_iternext*/
    0,                                 /*tp_methods*/
    RangeObject_members,               /*tp_members*/
    0,                                 /*tp_getset*/
    0,                                 /*tp_base*/
    0,                                 /*tp_dict*/
    0,                                 /*tp_descr_get*/
    0,                                 /*tp_descr_set*/
    0,                                 /*tp_dictoffset*/
    rangeObjectInit,                   /*tp_init*/
    0,                                 /*tp_alloc*/
    (newfunc)newRangeObject,           /*tp_new*/
    0,                                 /*tp_free*/
    0,                                 /*tp_is_gc*/
};

static struct PyModuleDef range_module = {
    PyModuleDef_HEAD_INIT,
    "range_module",
    "Custom range module",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_range_module() {
    PyObject* m;
    
    if (PyType_Ready(&RangeObject_Type) < 0) {
        return NULL;
    }

    m = PyModule_Create(&range_module);

    if (m == NULL) {
        return NULL;
    }

    Py_IncRef(&RangeObject_Type);

    if (PyModule_AddObject(m, "RangeObject",(PyObject *)&RangeObject_Type) < 0) {
        Py_DECREF(&RangeObject_Type);
        Py_DECREF(m);
        return NULL;
    }
    
    return m;
}