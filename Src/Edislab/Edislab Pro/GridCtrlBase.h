/*******************************************************************************/
/*描述:Grid控件重写-基类
/*作者:
/*日期:2015.8
/*******************************************************************************/
#ifndef GRID_CTRL_BASE_H
#define GRID_CTRL_BASE_H


class CGridCtrlBase : public CBCGPGridCtrl
{
	friend class CGridButtonItem;
public:
	CGridCtrlBase(void);
	~CGridCtrlBase(void);

public:
	CBCGPGridRow* CreateNewRow();

	//控件的ID
	static int s_nGridID;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

public:
	
	void SetGridItemReadOnly(bool bReadOnly);
	void AddColumnTitle(const CString& strColumTitle,int nIndex = -1);

private:
	virtual void OnItemChanged(CBCGPGridItem*pItem,int nRow,int nColumn);

protected:
	CStringArray m_ColumnTitleArray;

};

#endif