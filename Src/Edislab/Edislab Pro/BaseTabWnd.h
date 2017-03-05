/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:BaseTabWnd.h
Description:TabWnd基类
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#ifndef BASE_TAB_WND_H
#define BASE_TAB_WND_H

class CBaseTabWnd : public CBCGPTabWnd
{
	DECLARE_DYNCREATE(CBaseTabWnd)
public:
	CBaseTabWnd(CWnd* pParenWnd = NULL);
	~CBaseTabWnd(void);

	//设置Tab的父窗口指针
	void SetTabParentWnd(CWnd* pParentWnd);

	//获取Tab的父窗口指针
	CWnd* GetTabParentWnd(void) const;

protected:
	//父窗口指针
	CWnd* m_pParentWnd;
public:
	//创建TabWnd的ID基地址
	static unsigned int s_nTabBaseID;
};


#endif