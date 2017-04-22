/************************************************************************************
Copyright(C):
FileName:Utility.cpp
Description:公共接口
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
************************************************************************************/
#include "stdafx.h"
#include "Utility.h"
#include "ConverEncode.h"
/*****************************************************************************
FunctionName:GetExeDirecory
FunctionDescription:获取可执行文件所在的目录
@param
@param
@Return Value:可执行文件所在的目录
******************************************************************************/
std::string Utility::GetExeDirecory( void )
{
	std::string strPath;
	char szPath[MAX_PATH] = {0};

	if (0 != GetModuleFileNameA(NULL,szPath,sizeof(szPath)))
	{
		strPath = szPath;
		int nPos = (int)strPath.rfind('\\');
		if (std::string::npos != nPos)
		{
			strPath = strPath.substr(0,nPos);
		}
	}

	return strPath;
}

/*****************************************************************************
* @函数名称 : MultiByte2WideChar
* @功能描述 : 多字节向宽字节转换
* @author : xiaowei.han
* @date : 2016/7/1 12:34
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
std::wstring Utility::MultiByte2WideChar( const char* szSrc )
{
    std::wstring strResult;
    if (NULL != szSrc)
    {
        int nNum = MultiByteToWideChar(CP_ACP,0,szSrc,-1,NULL,0);   
        wchar_t *pwText = new wchar_t[nNum];
        if(NULL != pwText)
        {
            MultiByteToWideChar(CP_ACP,0,szSrc,-1,pwText,nNum);

            strResult = pwText;

            delete[] pwText;

            pwText = NULL;
        }     
    }

    return strResult;
}

/*****************************************************************************
* @函数名称 : WideChar2MultiByte
* @功能描述 : 宽字节向多字节转换
* @author : xiaowei.han
* @date : 2016/7/1 12:34
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
std::string Utility::WideChar2MultiByte( const wchar_t* szSrc )
{
    std::string strResult;
    if (NULL != szSrc)
    {
        int nNum = WideCharToMultiByte(CP_ACP,NULL,szSrc,-1,NULL,0,NULL,FALSE);
        char *psText = new char[nNum];
        if(NULL != psText)
        {
            WideCharToMultiByte(CP_ACP,NULL,szSrc,-1,psText,nNum,NULL,FALSE);

            strResult = psText;
            delete []psText;
            psText = NULL;
        }
        
    }
    
    return strResult;
   
}

HBITMAP Utility::LoadBitmapFromFile( const char* szBitmapFile )
{

	if (NULL == szBitmapFile)
	{
		return NULL;
	}

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL,Utility::MultiByte2WideChar(szBitmapFile).c_str(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	return hBitmap;
}

int Utility::AfxBCGPMessageBox( const CString& strText,UINT nType )
{

	return BCGPMessageBox(strText,nType);
}

BYTE Utility::CalCRC8( BYTE* pBuf,int nsize )
{
	BYTE crc = 0;
	while(nsize--)
	{
		crc ^= *pBuf;
		pBuf++;	
	}	
	return crc;
}

std::string Utility::ConverUTF8ToGB2312( const std::string& strContent )
{
	char szContent[MAX_PATH] = {0};
	memcpy(szContent,strContent.c_str(),strContent.length());
	std::string strGB2312Content;
	ConverEncode::UTF_8ToGB2312(strGB2312Content,szContent,strContent.length());
	return strGB2312Content;
}
