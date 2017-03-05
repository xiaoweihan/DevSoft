// DoubleEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DoubleEdit.h"


// DoubleEdit

IMPLEMENT_DYNAMIC(DoubleEdit, CEdit)

DoubleEdit::DoubleEdit()
{

}

DoubleEdit::~DoubleEdit()
{
}


BEGIN_MESSAGE_MAP(DoubleEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// DoubleEdit message handlers




void DoubleEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if((nChar >=48 && nChar <=57) || (((TCHAR)nChar) == '.') 
		|| (((TCHAR)nChar) == '-') || (nChar == 8))  
		//only Numbers, '.', '-' and backspace are allowed
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
