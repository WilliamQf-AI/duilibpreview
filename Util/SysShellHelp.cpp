#include "SysShellHelp.h"

typedef void (WINAPI *LPFN_PGNSI)(LPSYSTEM_INFO);


CSysShellHelp::CSysShellHelp(void)
{
}

CSysShellHelp::~CSysShellHelp(void)
{
}

//***************************************************
//���ƣ�ExeFile
//���ܣ�������ִ���ļ�
//������
//      strExePath - ��ִ���ļ�
//      strCmd     - ������
//����ֵ��0 ʧ�ܣ�1 - �ɹ�
//***************************************************
int CSysShellHelp::ExeFile(const string& strExePath, const string& strCmd, int nTimeout)
{
	PROCESS_INFORMATION  pi;
	STARTUPINFOA StartInfo;
	ZeroMemory(&StartInfo, sizeof(STARTUPINFOA));
	StartInfo.cb = sizeof(STARTUPINFOA);
	StartInfo.dwX = 20;
	StartInfo.dwY = 20;
	StartInfo.dwXSize = 200;
	StartInfo.dwYSize = 200;
	StartInfo.lpDesktop = NULL;
	StartInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartInfo.wShowWindow = SW_SHOW;//SW_HIDE;

	string strExe(strExePath);

	if (!strExe.empty())
	{
		strExe += " ";
	}

	strExe += strCmd;
	if (!CreateProcessA(NULL, //strExe.c_str(),// strExePath.c_str(),
		const_cast<char*>(strExe.c_str()),//NULL,//const_cast<char*>(strCmd.c_str()),
		NULL, 
		NULL,
		FALSE, 
		0,
		NULL,
		NULL,
		&StartInfo,
		&pi))
	{
		DWORD dwErr = GetLastError();

		return -1;
	}
	else   
	{
		//WaitForSingleObject(pi.hProcess,nTimeout);   
		CloseHandle(pi.hProcess);   
		CloseHandle(pi.hThread);   
	} 

	return 1;
}

int  CSysShellHelp::Source2File(const char* sFname, HMODULE hModule, DWORD dwID, const char* sType)
{
	HRSRC res=FindResourceA(hModule, MAKEINTRESOURCEA(dwID), sType);

	if (!res)
	{
		return -1;
	}
	{
		DWORD size = SizeofResource(hModule,res);
		HGLOBAL gl = LoadResource(hModule, res);
		LPVOID  lp = LockResource(gl);
		{
			FILE* pfile = fopen(sFname, "wb");
			if(pfile)
			{
				fwrite(lp, size, 1, pfile);
				fclose(pfile);
			}
		}
		UnlockResource(gl);
		FreeResource(gl);
	}

	return 0;
}

BOOL  CSysShellHelp::Is64Bit_OS()
{
	BOOL bRetVal = FALSE;
	SYSTEM_INFO si = { 0 };
	LPFN_PGNSI pGNSI = (LPFN_PGNSI) GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetNativeSystemInfo");
	if (pGNSI == NULL)
	{
		return FALSE;
	}
	pGNSI(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || 
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
	{
		bRetVal = TRUE;
	}
	else
	{
		// 32 λ����ϵͳ
		//_tprintf(_T("is 32 bit OS\r\n"));
	}
	return bRetVal;
}

BOOL CSysShellHelp::CheckIEVer(int &nVerNum)
{
	CRegKey regkey;//����ע��������
	LONG lResult = regkey.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Internet Explorer"),KEY_QUERY_VALUE); //�� ע���� 
	if( lResult != ERROR_SUCCESS ) 
		return FALSE;

	TCHAR szIEVer[50];
	DWORD dwSize = 50; 
	
	regkey.QueryValue(szIEVer,_T("Version"),&dwSize);//��ȡVersion�ֶ�ֵ
	regkey.Close();//�ر�ע���

	USES_CONVERSION;

	string strIEVer = T2A(szIEVer);
	int nPos = strIEVer.find_first_of(".");
	nVerNum = atoi(strIEVer.substr(0,nPos).c_str());
	return true;
}

//��������������������
//
void CSysShellHelp::RunCmd(string& strCmdText)
{
	//system(strCmdText.c_str());
	WinExec(strCmdText.c_str(), SW_HIDE);
}

BOOL CSysShellHelp::EnableDebugPriv()
{
	HANDLE hToken; 
	LUID sedebugnameValue; 
	TOKEN_PRIVILEGES tkp; 

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) { 
		return FALSE;
	} 
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue)) { 
		CloseHandle(hToken); 
		return FALSE;
	} 
	tkp.PrivilegeCount = 1; 
	tkp.Privileges[0].Luid = sedebugnameValue; 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) { 
		CloseHandle(hToken);
		return FALSE;
	} 
	return TRUE;
}

//������ȡ��Win7 64bit���г���ǰ��ȫ����(.bat;.exe;.reg;.vbs)
//      ע�������ԱȨ�ޣ���д��
void CSysShellHelp::DisableSafetyWarn()
{
	HKEY hKey;
	LRESULT lRet = 0;
	xstring strValue = _T(".exe");

	lRet= RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Associations"), 0, KEY_WRITE | KEY_WOW64_64KEY, &hKey);
	if(lRet == ERROR_SUCCESS) 
	{     
		//���������޸ļ�ֵ
		RegSetValueEx(hKey, _T("ModRiskFileTypes"),0,REG_SZ,(LPBYTE)strValue.c_str(), strValue.length() * 2);
	}
	else
	{
		DWORD dwDisposition;
		lRet = RegCreateKeyEx(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Associations"), 0,NULL, REG_OPTION_NON_VOLATILE, NULL,NULL, &hKey, &dwDisposition);
		if (lRet == ERROR_SUCCESS)
		{
			lRet = RegSetValueEx(hKey, _T("ModRiskFileTypes"),0,REG_SZ,(LPBYTE)strValue.c_str(), strValue.length() * 2);
		}
	}
	RegCloseKey(hKey);
	RunCmd(string("gpupdate /force"));
}

//������
//    ö��ϵͳ���̣�����ĳ�����̣���ɱ��
//������
//    strAppExeName - Ҫɱ���Ľ��̵�Ӧ�ó�����ļ���
//    bIsAll        - �Ƿ����еģ���ΪFALSE���ҵ�һ�����˳�
BOOL CSysShellHelp::KillProcess(const xstring &strAppExeName, BOOL bIsAll/* = TRUE*/)
{
	try
	{
		HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if( hProcess == INVALID_HANDLE_VALUE )
			return FALSE;

		TCHAR szShortPath[MAX_PATH] = { 0 };
		PROCESSENTRY32 pinfo; 
		MODULEENTRY32 minfo;

		pinfo.dwSize = sizeof(PROCESSENTRY32);
		minfo.dwSize = sizeof( MODULEENTRY32);

		BOOL report = Process32First(hProcess, &pinfo); 
		while(report) 
		{ 
			
			if (strAppExeName == pinfo.szExeFile)
			{
				HANDLE hExitProcess = OpenProcess( PROCESS_TERMINATE, FALSE, pinfo.th32ProcessID );
				if( hExitProcess)
				{
					::TerminateProcess(hExitProcess, 0);
					::CloseHandle(hExitProcess);

					if (!bIsAll)
						break;
				}
			}

			report = Process32Next(hProcess, &pinfo);
		}

		CloseHandle( hProcess );
	}
	catch(...)
	{}

	return TRUE;
}

long CSysShellHelp::KillProcessDos(const xstring& strExeName)
{
	//taskkill /f /im steamwebhelper.exe /t
	TCHAR szWindows[1024] = {0};
	GetWindowsDirectory(szWindows,1023);
	xstring strParam = szWindows;
	strParam += L"\\System32\\taskkill.exe /f /im ";
	strParam += strExeName;
	strParam += _T(" /t");

	xstring strCmd = _T("/c \"");
	strCmd += strParam;
	strCmd += _T("\"");

	TCHAR szSystem[512] = {0};
	GetSystemDirectory(szSystem, sizeof(szSystem));
	xstring strCmdExe = szSystem;
	strCmdExe += _T("\\cmd.exe");

	//OutputDebugStringW(strCmd.c_str());
	DWORD dwRet = (long)ShellExecute(NULL,_T("open"),strCmdExe.c_str(),strCmd.c_str(),_T(""),SW_HIDE);
	return dwRet;
}

BOOL SystemShutDown()
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

	// Get a token for this process. 

	if (!OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return( FALSE ); 

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
		&tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
		(PTOKEN_PRIVILEGES)NULL, 0); 

	if (GetLastError() != ERROR_SUCCESS) 
		return FALSE; 

	// Shut down the system and force all applications to close. 

	if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0)) 
		return FALSE; 

	return TRUE;

}
BOOL SystemReboot()
{
	
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

	// Get a token for this process. 

	if (!OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return( FALSE ); 

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
		&tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
		(PTOKEN_PRIVILEGES)NULL, 0); 

	if (GetLastError() != ERROR_SUCCESS) 
		return FALSE; 

	// Shut down the system and force all applications to close. 

	if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0)) 
		return FALSE; 

	return TRUE;
}

void CSysShellHelp::StartExeByCmd(const char* szCmd)
{
	string opt("cmd.exe /c \"");

	opt += szCmd;
	opt += "\"";

	::WinExec(opt.c_str(), SW_HIDE);
}
