#pragma once



// CGridView 窗体视图

class CGridView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CGridView)

protected:
	CGridView();           // 动态创建所使用的受保护的构造函数
	virtual ~CGridView();

public:
	enum { IDD = IDD_DLG_GRID };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


