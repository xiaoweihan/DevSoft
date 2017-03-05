/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:BaseTabWnd.cpp
Description:TabWnd基类
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#include "stdafx.h"
#include "BaseTabWnd.h"

IMPLEMENT_DYNCREATE(CBaseTabWnd,CBCGPTabWnd)
CBaseTabWnd::CBaseTabWnd( CWnd* pParenWnd ):
m_pParentWnd(pParenWnd)
{

}

CBaseTabWnd::~CBaseTabWnd( void )
{

}

/*****************************************************************************
@FunctionName : 设置Tab父窗口指针
@FunctionDescription : 设置Tab父窗口指针
@inparam  : pParentWnd:父窗口指针
@outparam :  
@return : 
*****************************************************************************/
void CBaseTabWnd::SetTabParentWnd( CWnd* pParentWnd )
{
	m_pParentWnd = pParentWnd;
}

/*****************************************************************************
@FunctionName : 获取Tab父窗口指针
@FunctionDescription : 获取Tab父窗口指针
@inparam  : 
@outparam :  
@return : Tab父窗口指针
*****************************************************************************/
CWnd* CBaseTabWnd::GetTabParentWnd( void ) const
{
	return m_pParentWnd;
}

unsigned int CBaseTabWnd::s_nTabBaseID = 100;
