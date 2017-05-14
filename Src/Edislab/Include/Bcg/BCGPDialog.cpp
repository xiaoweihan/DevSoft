//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2016 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
 //*******************************************************************************

#include "stdafx.h"

#include "BCGPDialog.h"

#ifndef _BCGSUITE_
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPPngImage.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPFrameWnd.h"
#include "BCGPRibbonBar.h"
#include "BCGPTooltipManager.h"
#endif

#include "BCGPVisualManager.h"
#include "BCGPLocalResource.h"
#include "BCGPGestureManager.h"
#include "BCGPDrawManager.h"
#include "BCGPGlobalUtils.h"
#include "BCGPEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EXPAND_MARGIN_VERT globalUtils.ScaleByDPI(8)

/////////////////////////////////////////////////////////////////////////////
// CBCGPDialog dialog

IMPLEMENT_DYNAMIC(CBCGPDialog, CDialog)

#pragma warning (disable : 4355)

CBCGPDialog::CBCGPDialog() :
	m_Impl (*this)
{
	CommonConstruct ();
}
//*************************************************************************************
CBCGPDialog::CBCGPDialog (UINT nIDTemplate, CWnd *pParent/*= NULL*/) : 
				CDialog (nIDTemplate, pParent),
				m_Impl (*this)
{
	CommonConstruct ();
}
//*************************************************************************************
CBCGPDialog::CBCGPDialog (LPCTSTR lpszTemplateName, CWnd *pParentWnd/*= NULL*/) : 
				CDialog(lpszTemplateName, pParentWnd),
				m_Impl (*this)
{
	CommonConstruct ();
}

#pragma warning (default : 4355)

//*************************************************************************************
void CBCGPDialog::CommonConstruct ()
{
	m_bDisableShadows = FALSE;
	m_bIsLocal = FALSE;
	m_pLocaRes = NULL;
	m_bWasMaximized = FALSE;
	m_rectBackstageWatermark.SetRectEmpty();
	m_pBackstageWatermark = NULL;
	m_pParentEdit = NULL;
	m_bParentClosePopupDlgNotified = FALSE;
	m_nExpandedAreaHeight = -1;
	m_nExpandedCheckBoxHeight = -1;
	m_hwndExpandCheckBoxCtrl = NULL;
	m_bExpandAreaSpecialBackground = TRUE;
	m_bIsRibbonStartPage = FALSE;
	m_bIsExpanded = TRUE;
	m_bCancelModeCapturedWindow = FALSE;
}
//*************************************************************************************
void CBCGPDialog::SetControlInfoTip(UINT nCtrlID, LPCTSTR lpszInfoTip, DWORD dwVertAlign, BOOL bRedrawInfoTip, CBCGPControlInfoTip::BCGPControlInfoTipStyle style, BOOL bIsClickable, const CPoint& ptOffset)
{
	m_Impl.SetControlInfoTip(nCtrlID, lpszInfoTip, dwVertAlign, bRedrawInfoTip, style, bIsClickable, ptOffset);
}
//*************************************************************************************
void CBCGPDialog::SetControlInfoTip(CWnd* pWndCtrl, LPCTSTR lpszInfoTip, DWORD dwVertAlign, BOOL bRedrawInfoTip, CBCGPControlInfoTip::BCGPControlInfoTipStyle style, BOOL bIsClickable, const CPoint& ptOffset)
{
	m_Impl.SetControlInfoTip(pWndCtrl, lpszInfoTip, dwVertAlign, bRedrawInfoTip, style, bIsClickable, ptOffset);
}
//*************************************************************************************
void CBCGPDialog::SetBackgroundColor (COLORREF color, BOOL bRepaint)
{
	if (m_brBkgr.GetSafeHandle () != NULL)
	{
		m_brBkgr.DeleteObject ();
	}

	if (color != (COLORREF)-1)
	{
		m_brBkgr.CreateSolidBrush (color);
	}

	if (bRepaint && GetSafeHwnd () != NULL)
	{
		Invalidate ();
		UpdateWindow ();
	}
}
//*************************************************************************************
void CBCGPDialog::SetBackgroundImage (HBITMAP hBitmap, 
								BackgroundLocation location,
								BOOL bAutoDestroy,
								BOOL bRepaint)
{
	m_Impl.m_Background.Clear();

	if (hBitmap != NULL)
	{
		m_Impl.m_Background.SetMapTo3DColors(FALSE);
		m_Impl.m_Background.AddImage(hBitmap, TRUE);

#ifndef _BCGSUITE_
		m_Impl.m_Background.SetSingleImage(FALSE);
#else
		m_Impl.m_Background.SetSingleImage();
#endif
		if (bAutoDestroy)
		{
			::DeleteObject(hBitmap);
		}
	}

	m_Impl.m_BkgrLocation = (int)location;

	if (bRepaint && GetSafeHwnd () != NULL)
	{
		RedrawWindow();
	}
}
//*************************************************************************************
BOOL CBCGPDialog::SetBackgroundImage (UINT uiBmpResId,
									BackgroundLocation location,
									BOOL bRepaint)
{
	BOOL bRes = TRUE;

	m_Impl.m_Background.Clear();
	
	if (uiBmpResId != 0)
	{
		m_Impl.m_Background.SetMapTo3DColors(FALSE);
		bRes = m_Impl.m_Background.Load(uiBmpResId);

#ifndef _BCGSUITE_
		m_Impl.m_Background.SetSingleImage(FALSE);
#else
		m_Impl.m_Background.SetSingleImage();
#endif
	}
	
	m_Impl.m_BkgrLocation = (int)location;
	
	if (bRepaint && GetSafeHwnd () != NULL)
	{
		RedrawWindow();
	}

	return bRes;
}

BEGIN_MESSAGE_MAP(CBCGPDialog, CDialog)
	//{{AFX_MSG_MAP(CBCGPDialog)
	ON_WM_ACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_ENABLE()	
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_GETMINMAXINFO()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SYSCOMMAND()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_WM_STYLECHANGED()
	ON_MESSAGE(WM_DWMCOMPOSITIONCHANGED, OnDWMCompositionChanged)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_POWERBROADCAST, OnPowerBroadcast)
	ON_REGISTERED_MESSAGE(BCGM_UPDATETOOLTIPS, OnBCGUpdateToolTips)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnNeedTipText)
	ON_MESSAGE(WM_DPICHANGED, OnDPIChanged)
	ON_MESSAGE(WM_THEMECHANGED, OnThemeChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPDialog message handlers

void CBCGPDialog::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	CDialog::OnStyleChanged(nStyleType, lpStyleStruct);
	
	if (nStyleType == GWL_EXSTYLE)
	{
		if (((lpStyleStruct->styleOld & WS_EX_LAYOUTRTL) != 0 && 
			(lpStyleStruct->styleNew & WS_EX_LAYOUTRTL) == 0 ||
			(lpStyleStruct->styleOld & WS_EX_LAYOUTRTL) == 0 && 
			(lpStyleStruct->styleNew & WS_EX_LAYOUTRTL) != 0))
		{
			OnRTLChanged ((lpStyleStruct->styleNew & WS_EX_LAYOUTRTL) != 0);
		}
	}
}
//*****************************************************************************
void CBCGPDialog::OnRTLChanged (BOOL bIsRTL)
{
	if (AfxGetMainWnd() == this)
	{
		globalData.m_bIsRTL = bIsRTL;
		CBCGPToolBarImages::EnableRTL(globalData.m_bIsRTL);
	}
}
//**************************************************************************
void CBCGPDialog::OnActivate(UINT nState, CWnd *pWndOther, BOOL /*bMinimized*/) 
{
	Default();
	m_Impl.OnActivate (nState, pWndOther);
}
//*************************************************************************************
BOOL CBCGPDialog::OnNcActivate(BOOL bActive) 
{
	//-----------------------------------------------------------
	// Do not call the base class because it will call Default()
	// and we may have changed bActive.
	//-----------------------------------------------------------
	BOOL bRes = (BOOL)DefWindowProc (WM_NCACTIVATE, bActive, 0L);

	if (bRes)
	{
		m_Impl.OnNcActivate (bActive);
	}

	return bRes;
}
//**************************************************************************************
void CBCGPDialog::OnEnable(BOOL bEnable) 
{
	CDialog::OnEnable (bEnable);
	m_Impl.OnNcActivate (bEnable);
}
//**************************************************************************************
BOOL CBCGPDialog::OnEraseBkgnd(CDC* pDC) 
{
	BOOL bRes = TRUE;

	CRect rectBottom;
	rectBottom.SetRectEmpty();

	CRect rectClient;
	GetClientRect (rectClient);

	if (m_nExpandedAreaHeight > 0 && m_nExpandedCheckBoxHeight > 0 && m_bExpandAreaSpecialBackground)
	{
		int nBottomHeight = m_nExpandedCheckBoxHeight + 2 * EXPAND_MARGIN_VERT;
		if (IsExpanded())
		{
			nBottomHeight += m_nExpandedAreaHeight;
		}
		
		rectBottom = rectClient;
		rectBottom.top = rectBottom.bottom - nBottomHeight;
	}

	if (m_brBkgr.GetSafeHandle () == NULL && !m_Impl.m_Background.IsValid() && !IsVisualManagerStyle ())
	{
		bRes = CDialog::OnEraseBkgnd (pDC);

		if (!rectBottom.IsRectEmpty() && !globalData.IsHighContastMode())
		{
			COLORREF clrLine = CBCGPDrawManager::ColorMakeDarker(::GetSysColor(COLOR_BTNFACE), .1);
			COLORREF clrLine1 = CBCGPDrawManager::ColorMakeLighter(::GetSysColor(COLOR_BTNFACE), .1);

			CBCGPDrawManager dm(*pDC);
			dm.DrawLine(rectBottom.left, rectBottom.top, rectBottom.right, rectBottom.top, clrLine);
			dm.DrawLine(rectBottom.left, rectBottom.top + 1, rectBottom.right, rectBottom.top + 1, clrLine1);
		}
	}
	else
	{
		ASSERT_VALID (pDC);

		if (m_Impl.m_BkgrLocation != 0 /* BACKGR_TILE */ || !m_Impl.m_Background.IsValid())
		{
			if (m_brBkgr.GetSafeHandle () != NULL)
			{
				pDC->FillRect (rectClient, &m_brBkgr);
			}
			else if (IsVisualManagerStyle ())
			{
#ifndef _BCGSUITE_
				if (IsBackstageMode())
				{
					CBCGPMemDC memDC(*pDC, this);
					CDC* pDCMem = &memDC.GetDC();

					CBCGPVisualManager::GetInstance ()->OnFillRibbonBackstageForm(pDCMem, this, rectClient);

#ifndef BCGP_EXCLUDE_RIBBON
					if (m_bIsRibbonStartPage)
					{
						// Syncronize the ribbon background image with the dialog background:
						OnDrawRibbonBackgroundImage(pDCMem, rectClient);

						int nLeftPaneWidth = GetRibbonStartPageLeftPaneWidth();
						if (nLeftPaneWidth > 0)
						{
							CRect rectLeftPane = rectClient;
							rectLeftPane.right = rectLeftPane.left + nLeftPaneWidth;

							CBCGPVisualManager::GetInstance()->OnFillRibbonBackstageLeftPane(pDCMem, rectLeftPane);
						}
					}
#endif
					if (!m_rectBackstageWatermark.IsRectEmpty() && !globalData.IsHighContastMode())
					{
						if (m_pBackstageWatermark != NULL)
						{
							ASSERT_VALID(m_pBackstageWatermark);
							m_pBackstageWatermark->DrawEx(pDCMem, m_rectBackstageWatermark, 0);
						}
						else
						{
							OnDrawBackstageWatermark(pDCMem, m_rectBackstageWatermark);
						}
					}

					bRes = TRUE;
				}
				else
#endif
				{
					if (IsWhiteBackground() && !globalData.IsHighContastMode() && !CBCGPVisualManager::GetInstance ()->IsDarkTheme())
					{
						pDC->FillSolidRect(rectClient, RGB(255, 255, 255));
					}
					else if (!CBCGPVisualManager::GetInstance ()->OnFillDialog (pDC, this, rectClient))
					{
						CDialog::OnEraseBkgnd (pDC);
					}
				}
			}
			else
			{
				CDialog::OnEraseBkgnd (pDC);
			}
		}

		if (!rectBottom.IsRectEmpty())
		{
			CBCGPVisualManager::GetInstance()->OnDrawDlgExpandedArea(pDC, this, rectBottom);
		}

		m_Impl.DrawBackgroundImage(pDC, rectClient);
	}

	m_Impl.DrawResizeBox(pDC);
	m_Impl.ClearAeroAreas (pDC);
	m_Impl.DrawControlInfoTips(pDC);

	return bRes;
}
//**********************************************************************************
void CBCGPDialog::OnDestroy() 
{
	if (m_pParentEdit != NULL)
	{
		if (!m_bParentClosePopupDlgNotified)
		{
			m_pParentEdit->ClosePopupDlg(NULL, FALSE);
			m_bParentClosePopupDlgNotified = TRUE;
		}

		delete m_pParentEdit;
		m_pParentEdit = NULL;
	}

	m_Impl.OnDestroy ();
	CDialog::OnDestroy();
}
//***************************************************************************************
HBRUSH CBCGPDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (m_brBkgr.GetSafeHandle () != NULL || m_Impl.m_Background.IsValid() ||
		IsVisualManagerStyle ())
	{
		HBRUSH hbr = m_Impl.OnCtlColor (pDC, pWnd, nCtlColor);
		if (hbr != NULL)
		{
			return hbr;
		}
	}	

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
//**************************************************************************************
BOOL CBCGPDialog::PreTranslateMessage(MSG* pMsg) 
{
	if (m_Impl.PreTranslateMessage (pMsg))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
//**************************************************************************************
void CBCGPDialog::SetActiveMenu (CBCGPPopupMenu* pMenu)
{
	m_Impl.SetActiveMenu (pMenu);
}
//*************************************************************************************
BOOL CBCGPDialog::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (m_hwndExpandCheckBoxCtrl != NULL && lParam == (LPARAM)m_hwndExpandCheckBoxCtrl && HIWORD(wParam) == BN_CLICKED)
	{
		Expand(!IsExpanded());
		return TRUE;
	}

	if (m_Impl.OnCommand (wParam, lParam))
	{
		return TRUE;
	}

	return CDialog::OnCommand(wParam, lParam);
}
//*************************************************************************************
INT_PTR CBCGPDialog::DoModal() 
{
	if (m_bIsLocal && m_pLocaRes == NULL)
	{
		m_pLocaRes = new CBCGPLocalResource ();
	}

	return CDialog::DoModal();
}
//*************************************************************************************
void CBCGPDialog::PreInitDialog()
{
	if (m_pLocaRes != NULL)
	{
		delete m_pLocaRes;
		m_pLocaRes = NULL;
	}
}
//*************************************************************************************
void CBCGPDialog::OnSysColorChange() 
{
	CDialog::OnSysColorChange();
	
	if (AfxGetMainWnd() == this)
	{
		globalData.UpdateSysColors ();
		CBCGPVisualManager::GetInstance ()->OnUpdateSystemColors ();
	}
}
//*************************************************************************************
void CBCGPDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CDialog::OnSettingChange(uFlags, lpszSection);
	
	if (AfxGetMainWnd() == this)
	{
		globalData.OnSettingChange ();
	}
}
//*************************************************************************************
void CBCGPDialog::EnableVisualManagerStyle (BOOL bEnable, BOOL bNCArea, const CList<UINT,UINT>* plstNonSubclassedItems)
{
	ASSERT_VALID (this);

	m_Impl.EnableVisualManagerStyle (bEnable, bEnable && bNCArea, plstNonSubclassedItems);

	if (bEnable && bNCArea)
	{
		m_Impl.OnChangeVisualManager ();
	}
}
//*************************************************************************************
BOOL CBCGPDialog::OnInitDialog() 
{
#ifndef _BCGSUITE_
	CBCGPPopupMenu* pActivePopupMenu = CBCGPPopupMenu::GetSafeActivePopupMenu();
	if (pActivePopupMenu != NULL)
	{
		ASSERT_VALID(pActivePopupMenu);
		
		if (pActivePopupMenu->IsFloaty())
		{
			pActivePopupMenu->SendMessage(WM_CLOSE);
		}
	}
#endif

	if (AfxGetMainWnd() == this)
	{
		globalData.m_bIsRTL = (GetExStyle() & WS_EX_LAYOUTRTL);
		CBCGPToolBarImages::EnableRTL(globalData.m_bIsRTL);
	}

	if (m_bCancelModeCapturedWindow)
	{
		HWND hWndCapture = ::GetCapture();
		if (hWndCapture != NULL && ::IsWindow(hWndCapture))
		{
			::SendMessage(hWndCapture, WM_CANCELMODE, 0, 0);
		}
	}

	CDialog::OnInitDialog();

	m_Impl.m_bHasBorder = (GetStyle () & WS_BORDER) == WS_BORDER;
	m_Impl.m_bHasCaption = (GetStyle() & WS_CAPTION) == WS_CAPTION;

	if (IsVisualManagerStyle ())
	{
		m_Impl.EnableVisualManagerStyle (TRUE, IsVisualManagerNCArea ());
	}

	if (m_Impl.HasAeroMargins ())
	{
		m_Impl.EnableAero (m_Impl.m_AeroMargins);
	}

	if (IsBackstageMode())
	{
		m_Impl.EnableBackstageMode();
	}

	if (m_Impl.IsOwnerDrawCaption() && (!m_Impl.m_bHasBorder || !m_Impl.m_bHasCaption))
	{
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	m_Impl.CreateTooltipInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//*************************************************************************************
BOOL CBCGPDialog::EnableAero (BCGPMARGINS& margins)
{
	return m_Impl.EnableAero (margins);
}
//*************************************************************************************
void CBCGPDialog::GetAeroMargins (BCGPMARGINS& margins) const
{
	m_Impl.GetAeroMargins (margins);
}
//***************************************************************************
LRESULT CBCGPDialog::OnDWMCompositionChanged(WPARAM,LPARAM)
{
	m_Impl.OnDWMCompositionChanged ();
	return 0;
}
//***************************************************************************
void CBCGPDialog::OnSize(UINT nType, int cx, int cy) 
{
	BOOL bIsMinimized = (nType == SIZE_MINIMIZED);

	if (m_Impl.IsOwnerDrawCaption ())
	{
		CRect rectWindow;
		GetWindowRect (rectWindow);

		WINDOWPOS wndpos;
		wndpos.flags = SWP_FRAMECHANGED;
		wndpos.x     = rectWindow.left;
		wndpos.y     = rectWindow.top;
		wndpos.cx    = rectWindow.Width ();
		wndpos.cy    = rectWindow.Height ();

		m_Impl.OnWindowPosChanged (&wndpos);
	}

	m_Impl.UpdateCaption ();
	m_Impl.UpdateToolTipsRect();

	if (!bIsMinimized && nType != SIZE_MAXIMIZED && !m_bWasMaximized)
	{
		AdjustControlsLayout();
		return;
	}

	CDialog::OnSize(nType, cx, cy);

	m_bWasMaximized = (nType == SIZE_MAXIMIZED);

	AdjustControlsLayout();
}
//**************************************************************************
void CBCGPDialog::OnNcPaint() 
{
	if (!m_Impl.OnNcPaint ())
	{
		Default ();
	}
}
//**************************************************************************
void CBCGPDialog::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	m_Impl.OnNcMouseMove (nHitTest, point);
	CDialog::OnNcMouseMove(nHitTest, point);
}
//**************************************************************************
void CBCGPDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_Impl.OnLButtonUp (point);
	CDialog::OnLButtonUp(nFlags, point);
}
//**************************************************************************
void CBCGPDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_Impl.OnLButtonDown (point);
	CDialog::OnLButtonDown(nFlags, point);
}
//**************************************************************************
BCGNcHitTestType CBCGPDialog::OnNcHitTest(CPoint point) 
{
	BCGNcHitTestType nHit = m_Impl.OnNcHitTest (point);
	if (nHit != HTNOWHERE)
	{
		return nHit;
	}

	return CDialog::OnNcHitTest(point);
}
//**************************************************************************
void CBCGPDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (!m_Impl.OnNcCalcSize (bCalcValidRects, lpncsp))
	{
		CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}
//**************************************************************************
void CBCGPDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_Impl.OnMouseMove (point);

	CDialog::OnMouseMove(nFlags, point);
}
//**************************************************************************
void CBCGPDialog::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	if ((lpwndpos->flags & SWP_FRAMECHANGED) == SWP_FRAMECHANGED)
	{
		m_Impl.OnWindowPosChanged (lpwndpos);
	}

	CDialog::OnWindowPosChanged(lpwndpos);

#ifndef _BCGSUITE_
	if (m_Impl.m_pShadow != NULL)
	{
		m_Impl.m_pShadow->Repos();
	}
#endif
}
//**************************************************************************
LRESULT CBCGPDialog::OnChangeVisualManager (WPARAM, LPARAM)
{
	m_Impl.OnChangeVisualManager ();
	return 0;
}
//**************************************************************************
void CBCGPDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	m_Impl.OnGetMinMaxInfo (lpMMI);
	CDialog::OnGetMinMaxInfo(lpMMI);
}
//****************************************************************************
LRESULT CBCGPDialog::OnSetText(WPARAM, LPARAM) 
{
	LRESULT	lRes = Default();

	if (lRes && IsVisualManagerStyle () && IsVisualManagerNCArea ())
	{
		RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
	}

	return lRes;
}
//****************************************************************************
int CBCGPDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_Impl.m_bDisableShadows = m_bDisableShadows;

	if (CDialog::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
    return m_Impl.OnCreate();
}
//****************************************************************************
void CBCGPDialog::EnableDragClientArea(BOOL bEnable)
{
	m_Impl.EnableDragClientArea(bEnable);
}
//****************************************************************************
void CBCGPDialog::EnableLayout(BOOL bEnable, CRuntimeClass* pRTC, BOOL bResizeBox)
{
	m_Impl.EnableLayout(bEnable, pRTC, bResizeBox);
}
//************************************************************************
void CBCGPDialog::SetWhiteBackground(BOOL bSet)
{
	ASSERT_VALID (this);

	m_Impl.m_bIsWhiteBackground = bSet && !globalData.IsHighContastMode();
}
//****************************************************************************
void CBCGPDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_Impl.LoadPlacement();
	CDialog::OnShowWindow(bShow, nStatus);
}
//****************************************************************************
BOOL CBCGPDialog::OnSetPlacement(WINDOWPLACEMENT& wp)
{
	return m_Impl.SetPlacement(wp);
}
//***************************************************************************
LRESULT CBCGPDialog::OnPowerBroadcast(WPARAM wp, LPARAM)
{
	LRESULT lres = Default ();

	if (wp == PBT_APMRESUMESUSPEND && AfxGetMainWnd()->GetSafeHwnd() == GetSafeHwnd())
	{
		globalData.Resume ();
#ifdef _BCGSUITE_
		bcgpGestureManager.Resume();
#endif
	}

	return lres;
}
//***************************************************************************
BOOL CBCGPDialog::Create(UINT nIDTemplate, CWnd* pParentWnd) 
{
#if _MSC_VER < 1400
	return Create(MAKEINTRESOURCE(nIDTemplate), pParentWnd);
#else
	if (m_bIsLocal && m_pLocaRes == NULL)
	{
		m_pLocaRes = new CBCGPLocalResource ();
	}

	return CDialog::Create(MAKEINTRESOURCE(nIDTemplate), pParentWnd);
#endif
}
//***************************************************************************
BOOL CBCGPDialog::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd) 
{
	if (m_bIsLocal && m_pLocaRes == NULL)
	{
		m_pLocaRes = new CBCGPLocalResource ();
	}
	
	return CDialog::Create(lpszTemplateName, pParentWnd);
}
//***************************************************************************
void CBCGPDialog::OnOK()
{
	if (m_pParentEdit != NULL && !m_bParentClosePopupDlgNotified)
	{
		m_pParentEdit->ClosePopupDlg(NULL, TRUE);
		m_bParentClosePopupDlgNotified = TRUE;
		return;
	}

	if (!IsBackstageMode())
	{
		CDialog::OnOK();
	}
}
//***************************************************************************
void CBCGPDialog::OnCancel()
{
	if (m_Impl.m_pToolTipInfo->GetSafeHwnd () != NULL)
	{
		m_Impl.m_pToolTipInfo->Pop();
	}

	if (m_pParentEdit != NULL && !m_bParentClosePopupDlgNotified)
	{
		m_pParentEdit->ClosePopupDlg(NULL, FALSE);
		m_bParentClosePopupDlgNotified = TRUE;
		return;
	}

	if (!IsBackstageMode())
	{
		CDialog::OnCancel();
	}
	else
	{
		CWnd* pParent = GetParent();
		if (pParent->GetSafeHwnd() != NULL)
		{
			pParent->SendMessage(WM_CLOSE);
		}
	}
}
//***************************************************************************
void CBCGPDialog::ClosePopupDlg(LPCTSTR lpszEditValue, BOOL bOK, DWORD_PTR dwUserData)
{
	if (m_pParentEdit == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	if (!m_bParentClosePopupDlgNotified)
	{
		m_pParentEdit->ClosePopupDlg(lpszEditValue, bOK, dwUserData);
		m_bParentClosePopupDlgNotified = TRUE;
	}
}
//***************************************************************************
void CBCGPDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	m_Impl.OnSysCommand(nID, lParam);
	CDialog::OnSysCommand(nID, lParam);
}
//***************************************************************************
void CBCGPDialog::EnableExpand(UINT nExpandCheckBoxCtrlID, LPCTSTR lpszExpandLabel, LPCTSTR lszCollapseLabel)
{
	EnableExpand(GetDlgItem(nExpandCheckBoxCtrlID)->GetSafeHwnd(), lpszExpandLabel, lszCollapseLabel);
}
//***************************************************************************
void CBCGPDialog::EnableExpand(HWND hwndExpandCheckBoxCtrl, LPCTSTR lpszExpandLabel, LPCTSTR lszCollapseLabel)
{
	ASSERT(GetSafeHwnd() != NULL);

	if (GetLayout() != NULL)
	{
		ASSERT(FALSE);
		return;
	}

	if (!m_bIsExpanded)
	{
		Expand();
	}

	m_strExpandLabel = lpszExpandLabel == NULL ? _T("") : lpszExpandLabel;
	m_strCollapseLabel = lszCollapseLabel == NULL ? _T("") : lszCollapseLabel;

	m_lstCtrlsInCollapseArea.RemoveAll();
	m_nExpandedAreaHeight = -1;
	m_nExpandedCheckBoxHeight = -1;

	m_hwndExpandCheckBoxCtrl = hwndExpandCheckBoxCtrl;

	if (hwndExpandCheckBoxCtrl == NULL)
	{
		return;
	}

	BOOL bIsCollapsedArea = FALSE;

	for (CWnd* pWndChild = GetWindow(GW_CHILD); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		HWND hwndChild = pWndChild->GetSafeHwnd();

		if (bIsCollapsedArea)
		{
			m_lstCtrlsInCollapseArea.AddTail(hwndChild);
		}
		else if (hwndChild == hwndExpandCheckBoxCtrl)
		{
			CBCGPButton* pButton = DYNAMIC_DOWNCAST(CBCGPButton, FromHandlePermanent(hwndExpandCheckBoxCtrl));
			if (pButton->GetSafeHwnd() == NULL)
			{
				m_btnExpandCheckBox.SubclassWindow(hwndExpandCheckBoxCtrl);
				pButton = &m_btnExpandCheckBox;
			}

			if (pButton->GetSafeHwnd() != NULL)
			{
				if (!m_strCollapseLabel.IsEmpty())
				{
					pButton->SetWindowText(m_strCollapseLabel);
				}

				pButton->m_bCheckBoxExpandCollapseMode = TRUE;
				pButton->SetMouseCursorHand();
				pButton->SizeToContent();
				pButton->SetCheck(1);

				CRect rectExpand;
				pButton->GetWindowRect(rectExpand);
				ScreenToClient(rectExpand);
				
				CRect rectClient;
				GetClientRect(rectClient);
				
				m_nExpandedAreaHeight = rectClient.bottom - rectExpand.bottom - EXPAND_MARGIN_VERT;
				m_nExpandedCheckBoxHeight = rectExpand.Height();
			}

			bIsCollapsedArea = TRUE;
		}
	}
}
//***************************************************************************
void CBCGPDialog::SetExpandAreaSpecialBackground(BOOL bSet, BOOL bRedraw)
{
	m_bExpandAreaSpecialBackground = bSet;

	if (bRedraw && GetSafeHwnd() != NULL)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}
}
//***************************************************************************
BOOL CBCGPDialog::Expand(BOOL bExpand)
{
	ASSERT(GetSafeHwnd() != NULL);

	if (m_lstCtrlsInCollapseArea.IsEmpty() || m_nExpandedAreaHeight <= 0)
	{
		return FALSE;
	}

	if (m_bIsExpanded == bExpand)
	{
		return TRUE;
	}

	OnBeforeExpand();

	CRect rectWindow;
	GetWindowRect(rectWindow);

	int dy = bExpand ? m_nExpandedAreaHeight : -m_nExpandedAreaHeight;

	SetWindowPos(NULL, -1, -1, rectWindow.Width(), rectWindow.Height() + dy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	for (POSITION pos = m_lstCtrlsInCollapseArea.GetHeadPosition(); pos != NULL;)
	{
		CWnd* pCtrl = CWnd::FromHandle(m_lstCtrlsInCollapseArea.GetNext(pos));

		if (!bExpand && GetFocus()->GetSafeHwnd() == pCtrl->GetSafeHwnd() && m_hwndExpandCheckBoxCtrl != NULL)
		{
			::SetFocus(m_hwndExpandCheckBoxCtrl);
		}

		pCtrl->ShowWindow(bExpand ? SW_SHOWNOACTIVATE : SW_HIDE);
	}

	m_bIsExpanded = bExpand;

	CString strLabel = m_bIsExpanded ? m_strCollapseLabel : m_strExpandLabel;

	CBCGPButton* pButton = DYNAMIC_DOWNCAST(CBCGPButton, CWnd::FromHandlePermanent(m_hwndExpandCheckBoxCtrl));
	ASSERT_VALID(pButton);

	pButton->SetCheck(m_bIsExpanded);

	if (!strLabel.IsEmpty())
	{
		pButton->SetWindowText(strLabel);
		pButton->SizeToContent();
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);

	OnAfterExpand();
	return TRUE;
}

#if (!defined _BCGSUITE_) && (!defined BCGP_EXCLUDE_RIBBON)

void CBCGPDialog::OnDrawRibbonBackgroundImage(CDC* pDC, CRect rect)
{
	CFrameWnd* pTopFrame = GetTopLevelFrame();
	CBCGPRibbonBar* pRibbonBar = NULL;

	CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pTopFrame);
	if (pMainFrame != NULL)
	{
		pRibbonBar = pMainFrame->GetRibbonBar();
	}
	else	// Maybe, SDI frame...
	{
		CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pTopFrame);
		if (pFrame != NULL)
		{
			pRibbonBar = pFrame->GetRibbonBar();
		}
	}

	if (pRibbonBar != NULL)
	{
		ASSERT_VALID(pRibbonBar);

		const CBCGPToolBarImages& image = pRibbonBar->GetBackgroundImage();

		if (!globalData.IsHighContastMode() && CBCGPVisualManager::GetInstance ()->IsRibbonBackgroundImage() && image.GetCount() > 0)
		{
			CRect rectRibbon;
			pRibbonBar->GetClientRect(rectRibbon);
			pRibbonBar->MapWindowPoints(this, &rectRibbon);
			
			rectRibbon.bottom = rectRibbon.top + rect.Height();

			((CBCGPToolBarImages&)image).DrawEx(pDC, rectRibbon, 0, CBCGPToolBarImages::ImageAlignHorzRight, CBCGPToolBarImages::ImageAlignVertTop);
		}
	}
}

#endif

BOOL CBCGPDialog::OnNeedTipText(UINT id, NMHDR* pNMH, LRESULT* pResult)
{
	return m_Impl.OnNeedTipText(id, pNMH, pResult);
}
//**************************************************************************
LRESULT CBCGPDialog::OnBCGUpdateToolTips (WPARAM wp, LPARAM)
{
	UINT nTypes = (UINT) wp;

	if (nTypes & BCGP_TOOLTIP_TYPE_DEFAULT)
	{
		m_Impl.CreateTooltipInfo();
	}

	return 0;
}
//**************************************************************************
BOOL CBCGPDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_Impl.OnSetCursor())
	{
		return TRUE;
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
//*****************************************************************************************
LRESULT CBCGPDialog::OnDPIChanged(WPARAM, LPARAM)
{
	LRESULT lRes = Default();
	
	OnChangeVisualManager(0, 0);
	return lRes;
}
//*****************************************************************************************
LRESULT CBCGPDialog::OnThemeChanged(WPARAM, LPARAM)
{
	LRESULT lRes = Default();
	
	CBCGPVisualManager::GetInstance()->OnUpdateSystemColors();
	OnChangeVisualManager(0, 0);
	return lRes;
}
