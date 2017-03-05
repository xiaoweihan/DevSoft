#pragma once


// DoubleEdit

class DoubleEdit : public CEdit
{
	DECLARE_DYNAMIC(DoubleEdit)

public:
	DoubleEdit();
	virtual ~DoubleEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


