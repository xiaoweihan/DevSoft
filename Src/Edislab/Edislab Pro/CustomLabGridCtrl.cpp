#include "stdafx.h"
#include "GlobalDataManager.h"
#include "CustomLabGridCtrl.h"
#include "GridViewManager.h"
#include "Msg.h"
#include "Edislab Pro.h"
#include "DlgTabPanel.h"
#define GLOBAL_DATA CGlobalDataManager::CreateInstance()

CCustomLabGridCtrl::CCustomLabGridCtrl( void ):
	m_nVitrualRows(INIT_ROW_NUM)
{
	m_GridViewManager = NULL;
}

CCustomLabGridCtrl::~CCustomLabGridCtrl( void )
{

}

void CCustomLabGridCtrl::SetViewManager(CGridViewManager* pViewManager)
{
	if (pViewManager)
	{
		m_GridViewManager = pViewManager;
	}
}

BEGIN_MESSAGE_MAP(CCustomLabGridCtrl, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CCustomLabGridCtrl::InitGridStyle()
{
	EnableHeader(TRUE, BCGP_GRID_HEADER_MOVE_ITEMS | BCGP_GRID_HEADER_SORT);
	EnableRowHeader(TRUE,-1,100);
	EnableLineNumbers(TRUE);
	SetClearInplaceEditOnEnter(FALSE);
	EnableInvertSelOnCtrl (TRUE);
	SetScrollBarsStyle (CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER);
	//SetGridItemReadOnly(true);
	SetWholeRowSel(TRUE);
	EnableMarkSortedColumn(TRUE,TRUE);

	/*EnableHeader (TRUE, BCGP_GRID_HEADER_MOVE_ITEMS | BCGP_GRID_HEADER_SELECT);
	SetClearInplaceEditOnEnter (FALSE);
	EnableInvertSelOnCtrl ();
	EnableLineNumbers ();*/
}

void CCustomLabGridCtrl::UpdataGrid()
{
	//清空Grid
	
	
	m_ColumnsEx.RemoveAllHeaderItems();
	m_ColumnsEx.DeleteAllColumns();
	//RemoveAll();

	//初始化Grid式样
	InitGridStyle();

	EnableRowHeader (TRUE);

	SetGridHeader();

	m_ColumnsEx.SetHeaderLineCount (GRID_HEADER_LINES);

	//EnableVirtualMode(m_pCallBack,(LPARAM)GetParent());
	EnableVirtualMode(m_pCallBack,(LPARAM)this);
	//EnableVirtualMode (GridCallback,(LPARAM)this);

	SetVirtualRows(m_nVitrualRows);

	AdjustLayout ();
}

void CCustomLabGridCtrl::SetGridHeader()
{
	//获取全局数据
	vector<GROUPDATA> allData = GLOBAL_DATA.getAllData();
	
	int nGroupCount = static_cast<int>(allData.size());

	for (int nGroup = 0; nGroup < nGroupCount; nGroup++)
	{
		CArray<int, int> arrMergeLines;
		CArray<int, int> arrMergeColumns;
		int nColumnCount = static_cast<int>(allData[nGroup].vecColData.size());
		for (int nCol = 0; nCol < nColumnCount; nCol++)
		{
			arrMergeColumns.Add (nCol);
		}

		arrMergeLines.Add (0);

		m_ColumnsEx.AddHeaderItem (&arrMergeColumns, &arrMergeLines, -1/*1*/,allData[nGroup].strGroupName , HDF_CENTER, nColumnCount+1);

		for (int i = 0; i < nColumnCount; i++)
		{
			InsertColumn (i, allData[nGroup].vecColData[i].strColumnName, DEFAULT_COLUMN_WIDTH);
			SetHeaderAlign (i, HDF_CENTER);
			SetColumnAlign (i, HDF_CENTER);
		}

		GetColumnsInfo ().SetHeaderMultiLine (nColumnCount, TRUE);
	}


	//CArray<int, int> arrMergeColumns;
	//CArray<int, int> arrMergeLines;

	//GROUPDATA& GroupData = m_GridDataManager->GetGridGroupInfo();
	//vector <COLUMNDATA>& ColData = m_GridDataManager->GetGridColummnInfo();

	//int nCount = static_cast<int>(ColData.size());

	//for (int i = 0; i < nCount; i++)
	//{
	//	arrMergeColumns.Add (i);
	//}

	//arrMergeLines.Add (0);
	//m_ColumnsEx.AddHeaderItem (&arrMergeColumns, &arrMergeLines, -1/*1*/,GroupData.strGroupName , HDF_CENTER, nCount+1);

	//for (int i = 0; i < nCount; i++)
	//{
	//	InsertColumn (i, ColData[i].strColumnName, DEFAULT_COLUMN_WIDTH);
	//	SetHeaderAlign (i, HDF_CENTER);
	//	SetColumnAlign (i, HDF_CENTER);
	//}

	//GetColumnsInfo ().SetHeaderMultiLine (nCount, TRUE);
}

int CCustomLabGridCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	UpdataGrid();

	return 0;
}

void CCustomLabGridCtrl::OnDestroy() 
{
	CBCGPGridCtrl::OnDestroy();
}

// Custom Header

CRect CCustomLabGridCtrl::OnGetHeaderRect (CDC* pDC, const CRect& rectDraw)
{
	CRect rect = CBCGPGridCtrl::OnGetHeaderRect (pDC, rectDraw);
	rect.bottom = rect.top + rect.Height () * m_ColumnsEx.GetHeaderLineCount ();

	return rect;
}

void CCustomLabGridCtrl::OnDrawHeader (CDC* pDC)
{
	m_ColumnsEx.PrepareDrawHeader ();

	CBCGPGridCtrl::OnDrawHeader (pDC);
}

void CCustomLabGridCtrl::OnPrintHeader(CDC* pDC, CPrintInfo* pInfo)
{
	m_ColumnsEx.PreparePrintHeader ();

	CBCGPGridCtrl::OnPrintHeader(pDC, pInfo);
}

void CCustomLabGridCtrl::OnPosSizeChanged ()
{
	CBCGPGridCtrl::OnPosSizeChanged ();

	m_ColumnsEx.ReposHeaderItems ();
}

void CCustomLabGridCtrl::SetDisplayVirtualRows( int nVirtualRows )
{
	m_nVitrualRows = nVirtualRows;
}

int CCustomLabGridCtrl::GetVirtualRows( void ) const
{
	return m_nVirtualRows;
}

void CCustomLabGridCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{

	CWnd* pWnd = AfxGetMainWnd();
	if (NULL != pWnd)
	{
		pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,0,0);
	}
	CBCGPGridCtrl::OnLButtonDown(nFlags, point);
}

void CCustomLabGridCtrl::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	//是否选中
	CBCGPGridRow* pRow = HitTestRowHeader(point);

	if (NULL != pRow)
	{
		if (TRUE == pRow->IsSelected())
		{
			//
		}
		CBCGPGridCtrl::OnLButtonUp(nFlags, point);
		return;
	}
	
	CBCGPGridItemID ItemID;
	ItemID = HitTestVirtual(point,NULL);

	CBCGPGridCtrl::OnLButtonDblClk(nFlags, point);
}

void CCustomLabGridCtrl::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);

	int nColumnNum = GetColumnCount();

	//列宽
	for (int i = 0; i < nColumnNum; ++i)
	{
		//SetColumnWidth(i,(cx - GetRowHeaderWidth()) / nColumnNum);
		SetColumnWidth(i,cx / nColumnNum);
	}
	AdjustLayout();
}

void CCustomLabGridCtrl::OnLButtonUp( UINT nFlags, CPoint point )
{
	CBCGPGridCtrl::OnLButtonUp(nFlags, point);
}

void CCustomLabGridCtrl::OnItemChanged (CBCGPGridItem* pItem, int nRow, int nColumn)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);

	CBCGPGridRow* pRow = pItem->GetParentRow ();
	if (pRow == NULL)
	{
		return;
	}

	ASSERT_VALID (pRow);

	vector<GROUPDATA> allData = GLOBAL_DATA.getAllData();
	
	int nGroupCount = static_cast<int>(allData.size());

	int nFind = 0;
	int nColumnID = 0;

	for (int nGroup = 0; nGroup < nGroupCount; nGroup++)
	{
		int nColumnCount = static_cast<int>(allData[nGroup].vecColData.size());
		for (int nCol = 0; nCol < nColumnCount; nCol++)
		{
			if(nFind == nColumn)
			{
				nColumnID = allData[nGroup].vecColData[nCol].nColumnID;
				break;
			}
			nFind++;
		}
	}
	//获取列ID

	//if(nColumn < )

	GLOBAL_DATA.ModifyData(nColumnID,nRow,(LPCTSTR) (_bstr_t) pItem->GetValue ());
	//	m_GridDataManager->GetGridData()[nColumn][nRow] = (LPCTSTR) (_bstr_t) pItem->GetValue ();
	GLOBAL_DATA.SetChanged();
	GLOBAL_DATA.Notify();
	if(m_GridViewManager)
		m_GridViewManager->RefreshAll(this);
}

void CCustomLabGridCtrl::SetCallBack( BCGPGRID_CALLBACK pCallBack )
{
	m_pCallBack = pCallBack;
}

void CCustomLabGridCtrl::Refresh( void )
{
	if (TRUE == IsVirtualMode())
	{
		m_CachedItems.CleanUpCache();
	}
	AdjustLayout();
}

void CCustomLabGridCtrl::Sort( int nColumn, BOOL bAscending /*= TRUE*/, BOOL bAdd /*= FALSE*/ )
{
	CBCGPGridCtrl::Sort(nColumn,bAscending,bAdd);
}

void CCustomLabGridCtrl::OnFilterBarChanged( int nColumn, const CString& strNewFilter )
{
	
}

BOOL CCustomLabGridCtrl::IsRowFilteredByFilterBar( const CBCGPGridRow* pRow )
{
	return FALSE;
}

BOOL CCustomLabGridCtrl::IsItemFilteredByFilterBar( CBCGPGridItem* pItem, int nColumn, const CString& strFilter )
{
	return FALSE;
}

BOOL CCustomLabGridCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_LBUTTONDOWN == pMsg->message||WM_RBUTTONDOWN==pMsg->message)
	{
		CWnd* parent = GetParent();
		if(parent)
		{
			parent = parent->GetParent();
			CDlgTabPanel* pTabPanel = dynamic_cast<CDlgTabPanel*>(parent);
			if(pTabPanel)
			{
				pTabPanel->SetActiveDlg(this);
			}
		}
	}
	return CBCGPGridCtrl::PreTranslateMessage(pMsg);
}
