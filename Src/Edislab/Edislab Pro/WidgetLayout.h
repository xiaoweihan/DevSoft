#pragma once
#include "Type.h"
class CAbstractSplitAlgorithm;
class CWidgetLayout
{
public:
	//分隔条个数
	static const int SPLIT_BAR_NUM = 2;
	CWidgetLayout(CWnd* pHostWnd,bool bWndFront = false);
	virtual ~CWidgetLayout(void);
	//添加组件
	void AddWidget(CWnd* pWidget);
	//删除组件
	void DelWidget(CWnd* pWidget);
	//调整布局
	void AdjustLayout(int nWidth,int nHeight);
	//左键按下
	void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//左键弹起
	void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//鼠标移动
	void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//获取水平移动分隔条
	CWnd* GetMoveHorSplitBar(void);

	//获取垂直移动风格条
	CWnd* GetMoveVerSplitBar(void);

	void InitLayout(CWnd* pWnd);

	//获取widget个数
	int GetWidgetNum(void) const;
private:
	//创建分隔条
	void CreateSplitBar(CWnd* pWnd);

	//销毁分隔条
	void DestroySplitBar(void);

	void InitSplitBar(void);

	void UnInitSplitBar(void);
private:
	//布局依附的窗口指针
	CWnd* m_pHostWnd;
	//布局窗口列表
	WidgetArray m_WidgetArray;
	//余下的窗口在前还是在后的标志
	bool m_bRemainingWndFront;
	//内部分隔条
	SplitBarArray m_SplitBarArray;
	//分割算法
	CAbstractSplitAlgorithm* m_pSplitAlgorithm;
	//一个分割条
	CSplitBar* m_SplitArray[SPLIT_BAR_NUM];
};

