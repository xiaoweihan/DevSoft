#pragma once
#include <vector>
#include "Type.h"
class CWidgetLayout;
class CAbstractSplitAlgorithm
{
public:
	CAbstractSplitAlgorithm(void);
	virtual ~CAbstractSplitAlgorithm(void);

	void SetWidgetArray(const WidgetArray& Array);

	void SetWndFront(bool bWndFront);

	void SetLayout(CWidgetLayout* pLayout);

	//设置宿主窗口指针
	void SetHostWnd(CWnd* pHostWnd);

	//重新计算布局
	void RecalculateLayout(int nWidth,int nHeight);

	//改变布局
	void ChangeLayout(int nWidth,int nHeight);

	//左键按下
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//左键弹起
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//鼠标移动
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
protected:
	//分隔条把宿主窗口分割的区域列表
	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray) {}
	//调整布局
	virtual void AdjustLayout(int nWidth,int nHeight) {}
	//根据坐标位置查询状态
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
	//获取指定索引的前一个索引
	int GetPrexHorSplitBarIndex(int nCurrentIndex);
	//获取垂直分隔条的前一个索引
	int GetPrexVerSplitBarIndex(int nCurrentIndex);
	//隐藏水平分隔条
	void ShowHorSplitBar(bool bShow);
	//隐藏垂直分隔条
	void ShowVerSplitBar(bool bShow);
protected:
	//子窗口的个数
	WidgetArray m_WidgetArray;
	//是否前置
	bool m_bWndFront;
	//宿主窗口的指针
	CWnd* m_pHostWnd;
	//记录鼠标选中的分隔条
	SPLIT_BAR_INFO m_SelectSplitBarInfo;
	//鼠标点击点
	CPoint m_StartPoint;
	//鼠标左键点击光标
	LPCTSTR m_CursorState;
	//窗口局部奇
	CWidgetLayout* m_pLayout;
};

