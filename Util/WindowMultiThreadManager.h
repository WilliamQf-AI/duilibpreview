#pragma once

/*
���ڹ����봰����ص��߳�,���翪�߳�����ȡ���ݲ��ڻ�ȡ�ɹ�����ʾ�ڴ�����,��ȡ�����д�������رգ��ڴ������������������RemoveAllThread,
���߳̿��Է���ı����յ�;
*/

#include "KCritSec.h"
#include <map>
using namespace std;

typedef void (*PThreadFuncCallback)(ULONG_PTR pParam);

class CWindowMultiThreadManager
{
public:
	CWindowMultiThreadManager(void);
	virtual ~CWindowMultiThreadManager(void);

	//�����߳�ID
	DWORD BeginThread(PThreadFuncCallback pFun,ULONG_PTR pParam);
	//����ָ���߳�id���߳�
	void RemoveThread(const DWORD& dwThreadId,bool bTerminate = false);
	//����ȫ���߳�
	void RemoveAllThread();


public:	//���ڲ�ʹ��,�ⲿ���ù�
	PThreadFuncCallback GetThreadFunc(const DWORD& dwThreadId);
	ULONG_PTR GetThreadParam(const DWORD& dwThreadId);

protected:
	CKCritSec m_lockThreadHandleMap;
	map<DWORD,HANDLE> m_mapThreadHandle;

	CKCritSec m_lockThreadFuncMap;
	map<DWORD,PThreadFuncCallback> m_mapThreadFunc;

	CKCritSec m_lockThreadParamMap;
	map<DWORD,ULONG_PTR> m_mapThreadParam;
};
