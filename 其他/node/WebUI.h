#pragma once

#include <Windows.h>
#include "wke.h"
#include <string>
#include <map>
using namespace std;

#include "../util/KCritSec.h"

#include "../duiLib/include/UIlib.h"
using namespace DuiLib;
#pragma comment(lib, "../duiLib/lib/Duilib_uStatic.lib")


#define DUI_CTR_WEBUI			(_T("WebUI"))

/////������Ⱦ�ļ��ִ���ʽ,�������ο�miniblink��api�ĵ������ؼ��������̣�
/////        wkeOnPaintUpdated�ص�֪ͨ�и���=���ؼ�����������Ч����=������duilib��ˢ��=��ˢ�µ��ÿؼ�ʱ,ͨ��wkeGetViewDC��ȡwebview��DC,�Լ�������Ӧλ�ü���.
/////1.�����õ�����Buffer��������,��Ҫ�Լ�ת��Ϊλͼ,����DC�ϡ����api��wkeOnPaintBitUpdated
/////2.����Ⱦ�õ�DCֱ�Ӵ�����,�Լ�ֱ��bitblt����Ҫ��λ�ü��ɡ����api��wkeOnPaintUpdated
/////3.ʹ��wkePaint�ӿ�,Ȼ���Լ�ת��Ϊλͼ,�Լ��������api��wkePaint
/////4.ʹ��wkePaint2�ӿ�,Ȼ���Լ�ת��Ϊλͼ,�Լ��������api��wkePaint2
/////5.ʹ��wkeGetViewDC��ȡwebview��DC,�Լ�ֱ��bitblt����Ҫ��λ�ü��ɡ����api��wkeGetViewDC

class CWebUI;

//�����µ�ҳ��ʱ�Ļص�,����˵������ window.open ���ߵ���� <a target="_blank" .../>
typedef wkeWebView (*PCreateViewCallback)(wkeWebView webView,CWebUI *pWeb,wkeNavigationType navType, const wchar_t* pszUrl,const wkeWindowFeatures* features);
//ҳ��������ʱ�Ļص����ɹ���ȡ����ʧ�ܣ�
typedef void (*PLoadFinishCallback)(CWebUI *pWeb,const wchar_t* pszUrl, wkeLoadingResult result,const wchar_t* pszFailedReason);
//ҳ�浼���ص�
typedef bool (*PNavigationCallback)(wkeWebView webView, CWebUI* pWeb, wkeNavigationType navType, const wchar_t* pszUrl);
//urlLoad֮ǰ�Ļص�:����true��ʾ��������������󣨲��ᷢ���������󣩡�����false��ʾ��Ȼ�ᷢ����������
typedef bool (*PLoadUrlBeginCallback)(wkeWebView webView, CWebUI* pWeb, const char* url, void *job);

//js�ص�c++�ĺ���ָ������
typedef jsValue (WKE_CALL_TYPE *PJsCallback)(jsExecState es,void* param);

//js�ص�c++�ĺ���ָ�����͡�ע�⣺���ר����window.external.xxx()
typedef jsValue (WKE_CALL_TYPE *PJsExternalCallback)(jsExecState es, jsValue object, jsValue* args, int argCount);

class CWebUI :	public CControlUI
{
public:
	CWebUI();
	~CWebUI(void);

	virtual LPCTSTR	GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
	virtual void DoInit();
	virtual void SetPos(RECT rc, bool bNeedInvalidate/* = true*/);
	virtual void DoEvent(DuiLib::TEventUI& event);
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

	//��ʼ������mb���˾����������mb apiǰ���ȵ��á���������mb api����͵���wkeInit���߳�Ϊͬ���߳�
	static void InitGlobal();
	static void UnInitGlobal();

	//ȫ�ֵ�js������c++����,�����ڶ��󴴽�֮ǰ���ð�
	//����:pszJSFuncName-c++������Ӧ����js�е���ʱ�ĺ����� param-�����Զ�������,�ڴ���ʱ���������ָ��
	static void JSBindFunction(const char* pszJSFuncName,PJsCallback pFunc,void* param, const int& nParamCount);

	//���ô����µ�ҳ��ʱ�Ļص�
	void SetCreateViewCallback(PCreateViewCallback pCallback);
	//���ü�����ɵĻص�
	void SetLoadFinishCallback(PLoadFinishCallback pCallback);
	//���õ����ص�
	void SetNavigationCallback(PNavigationCallback pCallback);
	//����urlLoadBegin�Ļص�
	void SetLoadUrlBeginCallback(PLoadUrlBeginCallback pCallback);
	//����window.external.xxx��js�󶨣�ע�ⲻҪ��ͬ����js����
	void SetExternalJSBindFunc(const string& strJSFuncName,PJsExternalCallback pFunc);
	//����window.xxx��js�󶨣�ע�ⲻҪ��ͬ����js����
	void SetWindowJSBindFunc(const string& strJSFuncName,PJsExternalCallback pFunc);

	//�ṩһ��SetURL�Ľӿ�,��֮ǰ�Ĵ���ʹ��
	void SetURL(const wstring& strUrl);
	//����html����
	void LoadHtml(const wstring& strHtml);
	//ҳ����������ˡ�ǰ����ֹͣ��ˢ��
	void Back();
	void Forward();
	void Stop();
	void Reload();

	//��������header Ҫ��PLoadUrlBeginCallback�ص��е��òſ���
	void SetDynamicHeader(void* jobPtr,wchar_t* pszKey,wchar_t* value);

	//�ṩһ��RunJS�Ľӿ�,��֮ǰ�Ĵ���ʹ�� c++ֱ�ӵ���js����
	jsValue RunJS(const wstring& strValue);

	inline wkeWebView GetWebView() {return m_WebView;};
private:
	//�����µ�ҳ��ʱ�Ļص�,����˵������ window.open ���ߵ���� <a target="_blank" .../>
	static wkeWebView OnCreateViewCallback(wkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures);
	//ҳ�������ɻص��������ɹ���ȡ����ʧ�ܣ�
	static void WKE_CALL_TYPE OnLoadFinish(wkeWebView webView,void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason);
	//��ת�����ص�
	static bool OnNavigationCallback(wkeWebView webView, void* param, wkeNavigationType navType, const wkeString urlStr);
	//webview��Ҫˢ�µĻص�
	static void OnPaintUpdatedCallback(wkeWebView webView, void* param, const HDC hdc, int x, int y, int cx, int cy);
	//LoadurlBegin�Ļص�
	static bool OnLoadUrlBeginCallback(wkeWebView webView, void* param, const char* url, void *job);
	//js�����Ĵ���ʱ�Ļص�
	static void WKE_CALL_TYPE OnCreateScriptContextCallback(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* context, int extensionGroup, int worldId);
	static void ReleaseExternalFunction(jsData* data);


private:
	static bool m_static_bInit_webui;

	wkeWebView m_WebView;		//������Ǹ����,���߽���ָ��
	
	PCreateViewCallback m_pCreateViewCallback;
	PLoadFinishCallback m_pLoadFinishCallback;
	PNavigationCallback m_pNavigationCallback;
	PLoadUrlBeginCallback m_pLoadUrlBeginCallback;

	CKCritSec m_lockExternalJs;
	map<string,PJsExternalCallback> m_mapExternalJs;
	CKCritSec m_lockWindowJs;
	map<string,PJsExternalCallback> m_mapWindowJs;
};
