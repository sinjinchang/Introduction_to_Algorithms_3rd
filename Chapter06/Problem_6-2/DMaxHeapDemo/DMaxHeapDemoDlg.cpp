
// DMaxHeapDemoDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "DMaxHeapDemo.h"
#include "DMaxHeapDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDMaxHeapDemoDlg 对话框



CDMaxHeapDemoDlg::CDMaxHeapDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DMAXHEAPDEMO_DIALOG, pParent)
	, m_InsertElements(_T(""))
	, m_IncreaseElemIndex(0)
	, m_IncreaseToElem(0)
	, m_DAry(3)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDMaxHeapDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INSERT_ELEM_EDIT, m_InsertElements);
	DDX_Text(pDX, IDC_INCREASE_ELEM_INDEX_EDIT, m_IncreaseElemIndex);
	DDV_MinMaxInt(pDX, m_IncreaseElemIndex, 0, INT_MAX);
	DDX_Text(pDX, IDC_INCREASE_TO_ELEM_EDIT, m_IncreaseToElem);
	DDX_Text(pDX, IDC_D_ARY_EDIT, m_DAry);
	DDV_MinMaxInt(pDX, m_DAry, 2, INT_MAX);
}

BEGIN_MESSAGE_MAP(CDMaxHeapDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_INSERT, &CDMaxHeapDemoDlg::OnInsert)
	ON_BN_CLICKED(IDC_EXTRACT_MAX, &CDMaxHeapDemoDlg::OnExtractMax)
	ON_BN_CLICKED(IDC_INCREASE_KEY, &CDMaxHeapDemoDlg::OnIncreaseKey)
	ON_BN_CLICKED(IDC_CLEAR, &CDMaxHeapDemoDlg::OnClear)
	ON_BN_CLICKED(IDC_SET_D_ARY, &CDMaxHeapDemoDlg::OnSetDAry)
END_MESSAGE_MAP()


// CDMaxHeapDemoDlg 消息处理程序

BOOL CDMaxHeapDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_DMaxHeapFigure.SubclassDlgItem(IDC_D_MAX_HEAP_FIGURE, this);

	int initDHeapData[] = { 16, 14, 10, 8, 7, 9, 3, 2, 4, 1 };
	m_DMaxHeap.set_branches(3);
	for (int i = 0; i < sizeof(initDHeapData)/sizeof(int); ++i)
		m_DMaxHeap.insert(initDHeapData[i]);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDMaxHeapDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDMaxHeapDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CDMaxHeapDemoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDMaxHeapDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;

	if (m_DMaxHeapFigure.m_hWnd)
	{
		m_DMaxHeapFigure.GetWindowRect(rect);
		ScreenToClient(rect);
		rect.bottom = cy-12;
		rect.right = cx-13;
		m_DMaxHeapFigure.MoveWindow(rect);
	}
}

void CDMaxHeapDemoDlg::OnInsert()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	istringstream is(m_InsertElements.GetBuffer());
	m_InsertElements.ReleaseBuffer();

	int elem;
	while (is >> elem)
		m_DMaxHeap.insert(elem);

	m_DMaxHeapFigure.ResetScrollRange();
	Invalidate();
}

void CDMaxHeapDemoDlg::OnExtractMax()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		m_DMaxHeap.extract_max();
	}
	catch (runtime_error)
	{
		MessageBox("Empty D-Heap Error!");
	}

	m_DMaxHeapFigure.ResetScrollRange();
	Invalidate();
}

void CDMaxHeapDemoDlg::OnIncreaseKey()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_DMaxHeap.increase_key(m_IncreaseElemIndex, m_IncreaseToElem);
	m_DMaxHeapFigure.ResetScrollRange();
	Invalidate();
}

void CDMaxHeapDemoDlg::OnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_DMaxHeap.clear();
	m_DMaxHeapFigure.ResetScrollRange();
	Invalidate();
}

void CDMaxHeapDemoDlg::OnSetDAry()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_DMaxHeap.set_branches(m_DAry);
	m_DMaxHeapFigure.ResetScrollRange();
	Invalidate();
}
