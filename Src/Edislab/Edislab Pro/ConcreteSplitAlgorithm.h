#pragma once
#include "Abstractsplitalgorithm.h"

//处理一个窗口的布局分割算法
class COneWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	COneWidgetSplitAlgorithm(void);
	~COneWidgetSplitAlgorithm(void);


public:
	//左键按下
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight) {}
	//左键弹起
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight) {}
	//鼠标移动
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight) {}
private:
	//处理水平移动
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight) {}
	//处理垂直移动
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight) {}
	//处理垂直和水平移动
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight) {}
	//水平移动之后计算坐标
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight) {}
	//垂直移动之后计算坐标
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight) {}
	//垂直和水平移动之后计算坐标
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight) {}
	//重新计算交叉点位置
	void ReCalculateIntersetRect(void) {}

protected:
	virtual void AdjustLayout(int nWidth,int nHeight);

	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
};

//处理2个窗口的布局分割算法
class CTwoWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CTwoWidgetSplitAlgorithm(void);
	~CTwoWidgetSplitAlgorithm(void);

	//左键按下
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//左键弹起
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//鼠标移动
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
protected:
	//处理水平移动
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直移动
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight) {}
	//处理垂直和水平移动
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight) {}
	//水平移动之后计算坐标
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直移动之后计算坐标
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight) {}
	//垂直和水平移动之后计算坐标
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight) {}
	//重新计算交叉点位置
	void ReCalculateIntersetRect(void) {}
protected:
	//根据坐标位置查询状态
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);

	virtual void AdjustLayout(int nWidth,int nHeight);

	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
protected:
	//水平分隔条
	CRect m_HorSplitBarRect[1];
};

//处理3个窗口的布局分割算法
class CThreeWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CThreeWidgetSplitAlgorithm(void);
	~CThreeWidgetSplitAlgorithm(void);
public:
	//左键按下
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//左键弹起
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//鼠标移动
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
protected:
	//处理水平移动
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直移动
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
protected:
	//根据坐标位置查询状态
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);

	virtual void AdjustLayout(int nWidth,int nHeight);

	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
private:
	//处理多窗口在前面的布局
	void AdjustLayoutFront(int nWidth,int nHeight);
	//处理多窗口在后面的布局
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//2个分割条
	CRect m_HorSplitBarRect[1];
	CRect m_VerSplitBarRect[1];
};

//处理4个窗口的布局分割算法
class CFourWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CFourWidgetSplitAlgorithm(void);
	~CFourWidgetSplitAlgorithm(void);
public:
	//左键按下
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//左键弹起
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//鼠标移动
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
protected:
	//处理水平移动
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
protected:
	//根据坐标位置查询状态
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);

	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
private:
	//处理多窗口在前面的布局
	void AdjustLayoutFront(int nWidth,int nHeight);
	//处理多窗口在后面的布局
	void AdjustLayoutBack(int nWidth,int nHeight);
	//重新计算选中后的分隔栏的位置
	void CalculateNextSplitBar(int nCurrentIndex,int nXMove,int nWidth,int nHeight);
private:
	CRect m_HorSplitBarRect[3];
};

//处理5个窗口的布局分割算法
class CFiveWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CFiveWidgetSplitAlgorithm(void);
	~CFiveWidgetSplitAlgorithm(void);
public:
	//左键按下
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//左键弹起
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//鼠标移动
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

protected:
	//处理水平移动
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直移动
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直和水平移动
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight);
	//水平移动之后计算坐标
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直移动之后计算坐标
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直和水平移动之后计算坐标
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//重新计算交叉点位置
	void ReCalculateIntersetRect(void);
protected:
	//根据坐标位置查询状态
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);

	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
private:
	//处理多窗口在前面的布局
	void AdjustLayoutFront(int nWidth,int nHeight);
	//处理多窗口在后面的布局
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//3个分割条
	CRect m_HorSplitBarRect[2];
	CRect m_VerSplitBarRect[1];
	//1个分隔条的交点
	SPLIT_INTERSECT_RECT m_IntersectRect[1];
};

//处理6个窗口的布局分割算法
class CSixWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CSixWidgetSplitAlgorithm(void);
	~CSixWidgetSplitAlgorithm(void);
public:
	//左键按下
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//左键弹起
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//鼠标移动
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

private:
	//处理水平移动
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直移动
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直和水平移动
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight);
	//水平移动之后计算坐标
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直移动之后计算坐标
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直和水平移动之后计算坐标
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//重新计算交叉点位置
	void ReCalculateIntersetRect(void);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);
	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
	//根据坐标位置查询状态
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
private:
	//处理多窗口在前面的布局
	void AdjustLayoutFront(int nWidth,int nHeight);
	//处理多窗口在后面的布局
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//3个分割条
	CRect m_HorSplitBarRect[2];
	CRect m_VerSplitBarRect[1];
	//2个分隔条的交点
	SPLIT_INTERSECT_RECT m_IntersectRect[2];
};

//处理7个窗口的布局分割算法
class CSevenWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CSevenWidgetSplitAlgorithm(void);
	~CSevenWidgetSplitAlgorithm(void);
public:
	//左键按下
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//左键弹起
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//鼠标移动
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
private:
	//处理水平移动
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直移动
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直和水平移动
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight);
	//水平移动之后计算坐标
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直移动之后计算坐标
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直和水平移动之后计算坐标
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//重新计算交叉点位置
	void ReCalculateIntersetRect(void);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);
	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
	//根据坐标位置查询状态
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
private:
	//处理多窗口在前面的布局
	void AdjustLayoutFront(int nWidth,int nHeight);
	//处理多窗口在后面的布局
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//4个分割条
	CRect m_HorSplitBarRect[3];
	CRect m_VerSplitBarRect[1];
	//2个分隔条的交点
	SPLIT_INTERSECT_RECT m_IntersectRect[2];
};

//处理8个窗口的布局分割算法
class CEightWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CEightWidgetSplitAlgorithm(void);
	~CEightWidgetSplitAlgorithm(void);
public:
	//左键按下
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//左键弹起
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//鼠标移动
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
private:
	//处理水平移动
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直移动
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直和水平移动
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight);
	//水平移动之后计算坐标
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直移动之后计算坐标
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直和水平移动之后计算坐标
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//重新计算交叉点位置
	void ReCalculateIntersetRect(void);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);
	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
	//根据坐标位置查询状态
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
private:
	//处理多窗口在前面的布局
	void AdjustLayoutFront(int nWidth,int nHeight);
	//处理多窗口在后面的布局
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//4个分割条
	CRect m_HorSplitBarRect[3];
	CRect m_VerSplitBarRect[1];
	//2个分隔条的交点
	SPLIT_INTERSECT_RECT m_IntersectRect[3];
};

//处理9个窗口的布局分割算法
class CNineWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CNineWidgetSplitAlgorithm(void);
	~CNineWidgetSplitAlgorithm(void);
public:
	//左键按下
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//左键弹起
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//鼠标移动
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
private:
	//处理水平移动
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直移动
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//处理垂直和水平移动
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight);
	//水平移动之后计算坐标
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直移动之后计算坐标
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//垂直和水平移动之后计算坐标
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//重新计算交叉点位置
	void ReCalculateIntersetRect(void);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);
	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
	//根据坐标位置查询状态
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
private:
	//处理多窗口在前面的布局
	void AdjustLayoutFront(int nWidth,int nHeight);
	//处理多窗口在后面的布局
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//4个分割条
	CRect m_HorSplitBarRect[2];
	CRect m_VerSplitBarRect[2];
	//2个分隔条的交点
	SPLIT_INTERSECT_RECT m_IntersectRect[4];
};