/**********************************************************
*版权所有:xiaowei.han
*文件名称:CustomGrid.h
*功能描述:自定义的Grid控件
*作者:xiaowei.han
*日期:2017/04/22
**********************************************************/
#ifndef CUSTOM_GRID_H
#define CUSTOM_GRID_H
#include "GridGroupType.h"
class CCustomGrid : public CBCGPGridCtrl
{
public:
	CCustomGrid();
	virtual ~CCustomGrid();
	DECLARE_DYNCREATE(CCustomGrid)

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnTrackHeader ()
	{
		return FALSE; // do not resize columns inside client area by mouse, only at header
	}

	// Merged header items:
public:
	virtual void OnPosSizeChanged();

protected:
	virtual CRect OnGetHeaderRect (CDC* pDC, const CRect& rectDraw);
	virtual void OnDrawHeader (CDC* pDC);
	virtual void OnPrintHeader(CDC* pDC, CPrintInfo* pInfo);
public:
	virtual CBCGPGridColumnsInfo& GetColumnsInfo();
	virtual const CBCGPGridColumnsInfo& GetColumnsInfo() const;
	void DrawHeaderItem (CDC* pDC, CRect rect, CBCGPHeaderItem* pHeaderItem);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	// other header implementation
	CBCGPGridColumnsInfoEx m_ColumnsEx;	
public:
	//创建的grid的ID
	static unsigned int s_GridID;

public:
	//设置行头信息(在创建Grid之前调用)
	void SetHeaderInfoArray(const std::vector<COLUMN_GROUP_INFO>& HeaderInfoArray);

	//动态设置grid的行头信息
	void DynamicSetHeaderInfoArray(const std::vector<COLUMN_GROUP_INFO>& HeaderInfoArray);

	//添加行头信息
	void AddHeaderInfo(const COLUMN_GROUP_INFO& HeaderInfo);

	//删除行头信息
	void RemoveHeaderInfo(const CString& strHeaderName);

	//删除某一列
	void RemoveColumn(const CString& strColumnName);

	//增加某一列
	void AddColumnInfo(const CString& strHeaderName,const CString& strColumnName);

	//查询表头名称是否已经存在
	bool IsHeaderNameExist(const CString& strHeaderName);

	//判断指定表头列名是否已经存在
	bool IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName);

	//获取表头信息
	void GetHeaderInfo(std::vector<COLUMN_GROUP_INFO>& HeaderInfoArray);

	//设置回调函数
	void SetCallBack(BCGPGRID_CALLBACK pCallBack)
	{
		m_pCallBack = pCallBack;
	}
	//设置初始显示的行数
	void SetInitDisplayRow(int nRows)
	{
		m_nDisplayRows = nRows;
	}

	//设置行数
	void SetDisplayRow(int nRow);
	//刷新Grid显示
	void Refresh(void);
	//获取列分组的显示信息
	void GetColumnGroupDisplayInfo(std::vector<SHOW_COLUMN_GROUP_INFO>& DisplayArray);
	//设置分组的显示信息
	void SetColumnGroupDisplayInfo(const std::vector<SHOW_COLUMN_GROUP_INFO>& DisplayArray);
	//设置数据(用于非虚表模式下)
	void SetColumnData(int nRowIndex,int nColumnIndex,float fData);
	//插入一行
	void InsertRow(int nRow,bool bBefore = true);
	//是否是虚表模式
	bool IsVirtualGrid(void);
private:
	//创建列信息
	void CreateHeaderInfo(void);
	//创建列信息
	void CreateColumnInfo(void);
	//创建所有行信息
	void CreateRowInfo(void);
protected:
	std::vector<COLUMN_GROUP_INFO> m_HeaderInfoArray;
	BCGPGRID_CALLBACK m_pCallBack;
	int m_nDisplayRows;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

#endif