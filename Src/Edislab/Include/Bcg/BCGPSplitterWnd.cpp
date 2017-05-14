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
//
// BCGPSplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPVisualManager.h"
#include "BCGPSplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBCGPSplitterWnd, CSplitterWnd)

/////////////////////////////////////////////////////////////////////////////
// CBCGPSplitterWnd

CBCGPSplitterWnd::CBCGPSplitterWnd()
{
	m_bCyclicPaneActivation = TRUE;
}

CBCGPSplitterWnd::~CBCGPSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CBCGPSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CBCGPSplitterWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPSplitterWnd message handlers

void CBCGPSplitterWnd::OnDrawSplitter (CDC* pDC, ESplitType nType, 
									   const CRect& rectArg)
{
	// if pDC == NULL, then just invalidate
	if (pDC == NULL)
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
		return;
	}

	CRect rect = rectArg;

	switch (nType)
	{
	case splitBorder:
		CBCGPVisualManager::GetInstance ()->OnDrawSplitterBorder (pDC, this, rect);
		return;

	case splitBox:
		CBCGPVisualManager::GetInstance ()->OnDrawSplitterBox (pDC, this, rect);
		break;

	case splitIntersection:
	case splitBar:
		break;

	default:
		ASSERT(FALSE);  // unknown splitter type
	}

	// fill the middle
	CBCGPVisualManager::GetInstance ()->OnFillSplitterBackground (pDC, this, rect);
}

void CBCGPSplitterWnd::RecalcLayout()
{
	for (int col = 0; col < m_nCols; col++)
	{
		for (int row = 0; row < m_nRows; row++)
		{
			if (GetDlgItem(IdFromRowCol(row, col)) == NULL)
			{
				// Not created yet, do nothing
				return;
			}
		}
	}

	CSplitterWnd::RecalcLayout();
}

BOOL CBCGPSplitterWnd::CanActivateNext(BOOL bPrev)
{
	ASSERT_VALID(this);

	if (!m_bCyclicPaneActivation)
	{
		int row = 0;
		int col = 0;

		if (GetActivePane(&row, &col) == NULL)
		{
			TRACE0("Warning: Cannot go to next pane - there is no current view.\n");
			return FALSE;
		}

		if (bPrev)
		{
			if (row == 0 && col == 0)
			{
				return FALSE;
			}
		}
		else
		{
			if (col == m_nCols - 1 && row == m_nRows - 1)
			{
				return FALSE;
			}
		}
	}

	return CSplitterWnd::CanActivateNext(bPrev);
}

BOOL CBCGPSplitterWnd::ActivateFirstPane()
{
	if (GetActivePane() == NULL)
	{
		TRACE0("Warning: Can't go to next pane - there is no current pane.\n");
		return FALSE;
	}

	SetActivePane(0, 0);
	return TRUE;
}

BOOL CBCGPSplitterWnd::ActivateLastPane()
{
	if (GetActivePane() == NULL)
	{
		TRACE0("Warning: Can't go to next pane - there is no current pane.\n");
		return FALSE;
	}

	SetActivePane(m_nRows - 1, m_nCols - 1);
	return TRUE;
}

