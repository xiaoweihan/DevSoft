// DlgGridOpt.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgGridOpt.h"
#include <boost/foreach.hpp>
// CDlgGridOpt 对话框
//树的ID
static int s_nTreeID = 10000;

IMPLEMENT_DYNAMIC(CDlgGridOpt, CBaseDialog)

CDlgGridOpt::CDlgGridOpt(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgGridOpt::IDD, pParent)
{
	m_GridDisplayArray.clear();
}

CDlgGridOpt::~CDlgGridOpt()
{
}

void CDlgGridOpt::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_LOCATION, m_DisplayItemTreeLocation);
}


BEGIN_MESSAGE_MAP(CDlgGridOpt, CBaseDialog)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgGridOpt::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgGridOpt::OnBnClickedBtnCancel)
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(BCGM_GRID_HEADERCHECKBOX_CLICK,&CDlgGridOpt::OnHeaderCheckClick)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ROW_CHECKBOX_CLICK, &CDlgGridOpt::OnCheckClick)
END_MESSAGE_MAP()


// CDlgGridOpt 消息处理程序


BOOL CDlgGridOpt::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/*******************************************************************
*函数名称:InitDisplay
*功能描述:初始化对话框显示
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/04 15:04:56
*******************************************************************/
void CDlgGridOpt::InitDisplay(void)
{
	if (NULL == m_DisplayItemTreeLocation.GetSafeHwnd())
	{
		return;
	}
	CRect rectTree;
	m_DisplayItemTreeLocation.GetClientRect (&rectTree);
	m_DisplayItemTreeLocation.MapWindowPoints(this, &rectTree);

	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | TVS_LINESATROOT |
		TVS_HASLINES | TVS_HASBUTTONS | TVS_CHECKBOXES;
	m_GridDisplayItemTree.Create(nTreeStyles,rectTree,this,s_nTreeID++);

	//填充树结构
	BOOST_FOREACH(auto& V,m_GridDisplayArray)
	{
		HTREEITEM hGroupRoot = m_GridDisplayItemTree.InsertItem(V.strGroupName);


		if (NULL != hGroupRoot)
		{
			BOOST_FOREACH(auto& ColumnElement,V.ColumnArray)
			{
				HTREEITEM hSubItem = m_GridDisplayItemTree.InsertItem(ColumnElement.strColumnName,hGroupRoot);
				if (NULL != hSubItem)
				{
					if (ColumnElement.bShow)
					{
						m_GridDisplayItemTree.SetCheck(hSubItem,TRUE);
					}
					else
					{
						m_GridDisplayItemTree.SetCheck(hSubItem,FALSE);
					}
				}

			}
		}
	}

	for (int i = 0; i < m_GridDisplayItemTree.GetRowCount(); ++i)
	{
		CBCGPGridRow* pRow = m_GridDisplayItemTree.GetRow(i);

		if (NULL != pRow)
		{
			//判断是否是否为叶子节点
			if (pRow->GetSubItemsCount() == 0)
			{
				pRow->UpdateParentCheckbox();
			}
		}
	}
	m_GridDisplayItemTree.SetVisualManagerColorTheme(TRUE);
	m_GridDisplayItemTree.AdjustLayout ();
}


void CDlgGridOpt::OnBnClickedBtnApply()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void CDlgGridOpt::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CDlgGridOpt::SetDisplayInfo(const std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray)
{
	m_GridDisplayArray = GridDisplayArray;
}

void CDlgGridOpt::GetDisplayInfo(std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray)
{
	GridDisplayArray.clear();

	GridDisplayArray = m_GridDisplayArray;
}


void CDlgGridOpt::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);
}

LRESULT CDlgGridOpt::OnHeaderCheckClick( WPARAM wp, LPARAM lp )
{
	BOOL bChecked = m_GridDisplayItemTree.GetColumnsInfo().GetCheckBoxState();

	m_GridDisplayItemTree.GetColumnsInfo().SetCheckBoxState(!bChecked);
	m_GridDisplayItemTree.CheckAll(!bChecked);

	return TRUE; // disable the default implementation
}

LRESULT CDlgGridOpt::OnCheckClick( WPARAM wp, LPARAM lp )
{
	CBCGPGridRow* pRow = (CBCGPGridRow*)lp;
	if (nullptr == pRow)
	{
		return 0;
	}

	ASSERT_VALID(pRow);

	if (pRow->HasCheckBox())
	{
		BOOL bChecked = pRow->GetCheck();

		// check the row
		pRow->SetCheck(!bChecked);

		// check the subitems
		pRow->CheckSubItems(!bChecked);

		// update parent items
		pRow->UpdateParentCheckbox(TRUE/* use 3 state checkboxes */);

		// update the header control
		m_GridDisplayItemTree.UpdateHeaderCheckbox();
	}

	return TRUE; // disable the default implementation
}
