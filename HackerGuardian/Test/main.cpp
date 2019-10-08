
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif


void printDict(PyObject* obj) {
	if (!PyDict_Check(obj))
		return;
	PyObject *k, *keys;
	keys = PyDict_Keys(obj);
	for (int i = 0; i < PyList_GET_SIZE(keys); i++) {
		k = PyList_GET_ITEM(keys, i);
		char* c_name = PyString_AsString(k);
		printf("%s/n", c_name);
	}
}

int main() {
// #define _TEST__
// #undef _TEST__
// #ifdef _TEST__
// 	printf("enter macro\n");
// #else
// 	printf("no macro\n");
	// #endif
	Py_Initialize();
	if (!Py_IsInitialized())
		return -1;

 	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	PyRun_SimpleString("sys.path.append('E:/python_workspace/HelloWorld/src/')");

	PyObject*	pModule = NULL;
	PyObject*	first = NULL;
	PyObject*	second = NULL;

	pModule = PyImport_ImportModule("hello");
	// test func with parameter
	PyObject* pfnMix = PyObject_GetAttrString(pModule, "mix");
	if (pfnMix && PyCallable_Check(pfnMix))  
	{  
		PyObject * pyParams = PyTuple_New(2);  
		PyTuple_SetItem(pyParams, 0, Py_BuildValue("i", 5));  
		PyTuple_SetItem(pyParams, 1, Py_BuildValue("i", 2));  

		// ok, call the function  
		int r1 = 0, r2 = 0;  
		PyObject * pyValue = PyObject_CallObject(pfnMix, pyParams);           
		PyArg_ParseTuple(pyValue, "i|i", &r1, &r2);  
		if (pyValue)  
		{  
			printf("%d,%d\n", r1, r2); //output is 7,3  
		}         
	}  

	// no argument
	second = PyObject_GetAttrString(pModule, "show");
	PyEval_CallObject(second, NULL);
	//Py_DECREF(second);
	//Py_CLEAR(second);

	// no argument
	second = PyObject_GetAttrString(pModule, "GetString");
	PyObject * pyString = PyObject_CallObject(second, NULL);
	printf("return string = %s", PyString_AsString(pyString));
	Py_DECREF(second);

	// have argument
	first = PyObject_GetAttrString(pModule, "add_item");

	PyObject* pArgs = NULL;
	PyObject* pList = NULL;
	//
	pList = PyList_New(2);
 	PyList_SetItem(pList, 0, Py_BuildValue("s", "x_name"));
 	PyList_SetItem(pList, 1, Py_BuildValue("s", "y_name"));
// 	PyList_SetItem(pList, Py_BuildValue("s", "the third"));
// 	PyList_SetItem(pList,0,PyInt_FromLong(1L));
//	PyList_SetItem(pList,1,PyInt_FromLong(2L));
//	PyList_SetItem(list,2,PyInt_FromLong(3L));
	//
	PyObject* pDict = PyDict_New();
	PyDict_SetItemString(pDict, "first", Py_BuildValue("i", 1));
	PyDict_SetItemString(pDict, "second", Py_BuildValue("i", 1));
	//
	pArgs = PyTuple_New(2);
	PyTuple_SetItem(pArgs, 0, pList);
	PyTuple_SetItem(pArgs, 1, pDict);

	PyEval_CallObject(first, pArgs);


// 	//导入模块
// 	PyObject* pModule = PyImport_ImportModule("testpy");
// 	if (!pModule) {
// 		printf("Cant open python file!/n");
// 		return -1;
// 	}
// 	//模块的字典列表
// 	PyObject* pDict = PyModule_GetDict(pModule);
// 	if (!pDict) {
// 		printf("Cant find dictionary./n");
// 		return -1;
// 	}
// 	//打印出来看一下
// 	printDict(pDict);
// 	//演示函数调用
// 	PyObject* pFunHi = PyDict_GetItemString(pDict, "sayhi");
// 	PyObject_CallFunction(pFunHi, "s", "lhb");
// 	Py_DECREF(pFunHi);
// 	//演示构造一个Python对象，并调用Class的方法
// 	//获取Second类
// 	PyObject* pClassSecond = PyDict_GetItemString(pDict, "Second");
// 	if (!pClassSecond) {
// 		printf("Cant find second class./n");
// 		return -1;
// 	}
// 	//获取Person类
// 	PyObject* pClassPerson = PyDict_GetItemString(pDict, "Person");
// 	if (!pClassPerson) {
// 		printf("Cant find person class./n");
// 		return -1;
// 	}
// 	//构造Second的实例
// 	PyObject* pInstanceSecond = PyInstance_New(pClassSecond, NULL, NULL);
// 	if (!pInstanceSecond) {
// 		printf("Cant create second instance./n");
// 		return -1;
// 	}
// 	//构造Person的实例
// 	PyObject* pInstancePerson = PyInstance_New(pClassPerson, NULL, NULL);
// 	if (!pInstancePerson) {
// 		printf("Cant find person instance./n");
// 		return -1;
// 	}
// 	//把person实例传入second的invoke方法
// 	PyObject_CallMethod(pInstanceSecond, "invoke", "O", pInstancePerson);
// 	//释放
// 	Py_DECREF(pInstanceSecond);
// 	Py_DECREF(pInstancePerson);
// 	Py_DECREF(pClassSecond);
// 	Py_DECREF(pClassPerson);
// 	Py_DECREF(pModule);
	Py_Finalize();

	system("pause");
	return 0;
}
