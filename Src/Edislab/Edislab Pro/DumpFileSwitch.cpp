/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:DumpFileSwitch.cpp
Description:Dump文件开关器
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#include "stdafx.h"
#include "DumpFileSwitch.h"
#include <Windows.h>
#include <ImageHlp.h>
#include <string>
#pragma warning(push)
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma comment(lib,"dbghelp.lib")
static std::string s_DumpFilePath = "";
static bool IsDataSectionNeeded(const wchar_t* szModuleName);
static void CreateMiniDump(EXCEPTION_POINTERS* pep,const TCHAR* szFilename);
static BOOL CALLBACK MiniDumpCallback(PVOID pParam,const PMINIDUMP_CALLBACK_INPUT pInput,PMINIDUMP_CALLBACK_OUTPUT pOutput);
static LONG __stdcall CustomUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
/*****************************************************************************
* @函数名称 : CreateInstance
* @功能描述 : 创建单实例
* @author : xiaowei.han
* @date : 2016/6/29 13:33
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
CDumpFileSwitch& CDumpFileSwitch::CreateInstance( void )
{
 
    return s_obj;
}

/*****************************************************************************
* @函数名称 : CDumpFileSwitch
* @功能描述 : 构造函数
* @author : xiaowei.han
* @date : 2016/6/29 13:33
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
CDumpFileSwitch::CDumpFileSwitch( void )
{

}

/*****************************************************************************
* @函数名称 : CDumpFileSwitch
* @功能描述 : 析构函数
* @author : xiaowei.han
* @date : 2016/6/29 13:33
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
CDumpFileSwitch::~CDumpFileSwitch( void )
{

}

void CDumpFileSwitch::SetDumpFilePath( const char* szDumpFilePath )
{
    if (NULL == szDumpFilePath)
    {
        return;
    }

    s_DumpFilePath = szDumpFilePath;
}

void CDumpFileSwitch::OpenSwitch( void )
{
    SetUnhandledExceptionFilter(CustomUnhandledExceptionFilter);
}


void CDumpFileSwitch::OpenSwitchWithReg( int nType,const char* szExeName )
{
    if (NULL == szExeName || nType < 1 || nType > 2)
    {
        return;
    }

    HKEY hReg = NULL;
    CString strSubKey = CString(_T("SOFTWARE\\Microsoft\\Windows\\Windows Error Reporting\\LocalDumps")) + CString(_T("\\")) + CString(szExeName);
    LONG lResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE,strSubKey,0
                                  ,NULL,0,KEY_WRITE,NULL,&hReg,NULL);

    if (ERROR_SUCCESS != lResult)
    {
        return;
    }

    lResult = RegSetValueEx(hReg,_T("DumpFolder"),0,REG_EXPAND_SZ,(const BYTE*)s_DumpFilePath.c_str(),s_DumpFilePath.length());

    if (ERROR_SUCCESS != lResult)
    {
        return;
    }

    int nMaxDumpFileNum = 10;
    lResult = RegSetValueEx(hReg,_T("DumpCount"),0,REG_DWORD,(const BYTE*)&nMaxDumpFileNum,sizeof(nMaxDumpFileNum));
    if (ERROR_SUCCESS != lResult)
    {
        return;
    }
    int nCustomDumpFlags = 0;
    lResult = RegSetValueEx(hReg,_T("CustomDumpFlags"),0,REG_DWORD,(const BYTE*)&nCustomDumpFlags,sizeof(nCustomDumpFlags));
    if (ERROR_SUCCESS != lResult)
    {
        return;
    }

    lResult = RegSetValueEx(hReg,_T("DumpType"),0,REG_DWORD,(const BYTE*)&nType,sizeof(nType));
    if (ERROR_SUCCESS != lResult)
    {
        return;
    }
}

bool IsDataSectionNeeded( const wchar_t* szModuleName )
{
    if (NULL == szModuleName)
    {
        return false;
    }

    wchar_t szFileName[_MAX_FNAME] = L"";
    _wsplitpath(szModuleName,NULL,NULL,szFileName,NULL);
    if(wcsicmp(szFileName,L"ntdll") == 0)
    {
        return true;
    }

    return false;
}

void CreateMiniDump( EXCEPTION_POINTERS* pep,const TCHAR* szFilename )
{
    HANDLE hFile = CreateFile(szFilename,GENERIC_READ | GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) )
    {
        MINIDUMP_EXCEPTION_INFORMATION mdei;
        mdei.ThreadId           = GetCurrentThreadId();
        mdei.ExceptionPointers  = pep;
        mdei.ClientPointers     = FALSE;
        MINIDUMP_CALLBACK_INFORMATION mci;
        mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;
        mci.CallbackParam       = 0;
        MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)(MiniDumpWithPrivateReadWriteMemory |
            MiniDumpWithDataSegs |
            MiniDumpWithHandleData |
            0x00000800 |
            0x00001000 |
            MiniDumpWithUnloadedModules );
        MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
            hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci );
        CloseHandle( hFile );
        hFile = NULL;
    }

}

BOOL CALLBACK MiniDumpCallback( PVOID pParam,const PMINIDUMP_CALLBACK_INPUT pInput,PMINIDUMP_CALLBACK_OUTPUT pOutput )
{
    if( pInput == NULL || pOutput == NULL)
    {
        return FALSE;
    }
    switch(pInput->CallbackType)
    {
    case ModuleCallback:
        if( pOutput->ModuleWriteFlags & ModuleWriteDataSeg )
        {
            if( !IsDataSectionNeeded( pInput->Module.FullPath ) )
            {
                pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
            }
        }
    case IncludeModuleCallback:
    case IncludeThreadCallback:
    case ThreadCallback:
    case ThreadExCallback:
        return TRUE;
    default:
        break;
    }
    return FALSE;
}

LONG __stdcall CustomUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	USES_CONVERSION;
    CreateMiniDump(pExceptionInfo,A2T(s_DumpFilePath.c_str()));
    return EXCEPTION_EXECUTE_HANDLER;
}
#pragma warning(pop)

CDumpFileSwitch CDumpFileSwitch::s_obj;