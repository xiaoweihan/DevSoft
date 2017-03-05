#pragma once



// CDeviceView 窗体视图

class CDeviceView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CDeviceView)

protected:
	CDeviceView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDeviceView();

public:
	enum { IDD = IDD_DLG_DEVICE };
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


