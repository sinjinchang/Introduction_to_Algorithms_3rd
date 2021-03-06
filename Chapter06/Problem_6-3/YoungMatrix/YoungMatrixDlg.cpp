
// YoungMatrixDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "YoungMatrix.h"
#include "YoungMatrixDlg.h"
#include "afxdialogex.h"

#include <sstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYoungMatrixDlg 对话框



CYoungMatrixDlg::CYoungMatrixDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_YOUNGMATRIX_DIALOG, pParent)
	, m_MatrixHeight(4)
	, m_MatrixWidth(4)
	, m_InsertElements(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_YoungMatrix.set_infinity(INT_MAX);
	m_YoungMatrix.set_size(m_MatrixWidth, m_MatrixHeight);

	int initMatrixData[] = { 9, 16, 3, 2, 4, 8, 5, 14, 12 };
	for (int i = 0; i < sizeof(initMatrixData)/sizeof(int); ++i)
		m_YoungMatrix.insert(initMatrixData[i]);

}

void CYoungMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MATRIX_HEIGHT_EDIT, m_MatrixHeight);
	DDV_MinMaxInt(pDX, m_MatrixHeight, 0, INT_MAX);
	DDX_Text(pDX, IDC_MATRIX_WIDTH_EDIT, m_MatrixWidth);
	DDV_MinMaxInt(pDX, m_MatrixWidth, 0, INT_MAX);
	DDX_Text(pDX, IDC_INSERT_ELEMS_EDIT, m_InsertElements);
}

BEGIN_MESSAGE_MAP(CYoungMatrixDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_SET_SIZE, &CYoungMatrixDlg::OnSetSize)
	ON_BN_CLICKED(IDC_INSERT, &CYoungMatrixDlg::OnInsert)
	ON_BN_CLICKED(IDC_EXTRACT_MIN, &CYoungMatrixDlg::OnExtractMin)
	ON_BN_CLICKED(IDC_Clear, &CYoungMatrixDlg::OnClear)
END_MESSAGE_MAP()


// CYoungMatrixDlg 消息处理程序

BOOL CYoungMatrixDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_YoungMatrixFigure.SubclassDlgItem(IDC_MATRIX_FIGURE, this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYoungMatrixDlg::OnPaint()
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
HCURSOR CYoungMatrixDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CYoungMatrixDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CYoungMatrixDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;

	if (m_YoungMatrixFigure.m_hWnd)
	{
		m_YoungMatrixFigure.GetWindowRect(rect);
		ScreenToClient(rect);
		rect.bottom = cy - 12;
		rect.right = cx - 13;
		m_YoungMatrixFigure.MoveWindow(rect);
	}
}

void CYoungMatrixDlg::OnSetSize()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_YoungMatrix.set_size(m_MatrixWidth, m_MatrixHeight);
	m_YoungMatrixFigure.ResetScrollRange();
	Invalidate();
}

void CYoungMatrixDlg::OnInsert()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	istringstream is(m_InsertElements.GetBuffer());
	m_InsertElements.ReleaseBuffer();

	int elem;
	while (is >> elem)
		m_YoungMatrix.insert(elem);

	Invalidate();
}

void CYoungMatrixDlg::OnExtractMin()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		m_YoungMatrix.extract_min();
	}
	catch (runtime_error)
	{
		MessageBox("Empty Young Matrix Error!");
	}

	Invalidate();
}

void CYoungMatrixDlg::OnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_YoungMatrix.clear();
	m_YoungMatrixFigure.ResetScrollRange();
	Invalidate();
}
