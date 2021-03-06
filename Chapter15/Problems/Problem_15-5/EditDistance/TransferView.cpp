// TransferView.cpp: 实现文件
//

#include "stdafx.h"
#include "EditDistance.h"
#include "TransferView.h"
#include "EditDistanceDlg.h"
#include "EditDistanceAlgo.h"


// CTransferView

IMPLEMENT_DYNAMIC(CTransferView, CWnd)

CTransferView::CTransferView()
{

}

CTransferView::~CTransferView()
{
}


BEGIN_MESSAGE_MAP(CTransferView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


void CTransferView::ResetScrollRange()
{
	int totalWidth, totalHeight;
	CEditDistanceDlg *pDlg = (CEditDistanceDlg*)GetParent();

	if (pDlg == NULL)
	{
		totalWidth = 0;
		totalHeight = 0;
	}
	else
	{
		string str1 = pDlg->GetEditDistance().GetOriginalString();
		string str2 = pDlg->GetEditDistance().GetTargetString();

		CPaintDC dc(this);
		CFont font;
		font.CreateFont(16, 0, 000, 000, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
						OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
						VARIABLE_PITCH | FF_ROMAN, CString("Courier New"));
		CFont *pOldFont = dc.SelectObject(&font);

		CSize singleStrSize = dc.GetTextExtent(CString(" "));
		CSize strSize1 = dc.GetTextExtent(CString("initial strings"));
		CSize strSize2 = dc.GetTextExtent(CString(str1.c_str()));
		CSize strSize3 = dc.GetTextExtent(CString(str2.c_str()));

		dc.SelectObject(pOldFont);

		totalWidth = strSize1.cx + strSize2.cx + strSize3.cx + 2*columnBlank + singleStrSize.cx + 80;

		totalHeight = rowBlank*2 + singleStrSize.cy;

		if (str1.size() != 0 || str2.size() != 0)
		{
			totalHeight += (singleStrSize.cy+rowBlank) * (pDlg->GetEditDistance().GetOPChain().size() + 1);
		}
	}

	SCROLLINFO scroll_info;
	scroll_info.cbSize = sizeof(SCROLLINFO);

	CRect client;
	GetClientRect(client);
	scroll_info.fMask = SIF_RANGE | SIF_PAGE;
	scroll_info.nMin = 0;
	scroll_info.nMax = totalWidth;
	scroll_info.nPage = client.Width();
	SetScrollInfo(SB_HORZ, &scroll_info);

	GetClientRect(client);
	scroll_info.fMask = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
	scroll_info.nMin = 0;
	scroll_info.nMax = totalHeight;
	scroll_info.nPage = client.Height();
	SetScrollInfo(SB_VERT, &scroll_info);
}


// CTransferView 消息处理程序

BOOL CTransferView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK
		|| pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_RBUTTONDBLCLK)
	{
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CTransferView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ResetScrollRange();
}

void CTransferView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x = GetScrollPos(SB_HORZ);

	switch (nSBCode)
	{
	case SB_LEFT:
		x = 0;
		break;

	case SB_RIGHT:
		x = GetScrollLimit(SB_HORZ) - 1;
		break;

	case SB_LINELEFT:
		x -= 1;
		break;

	case SB_LINERIGHT:
		x += 1;
		break;

	case SB_PAGELEFT:
		x -= 10;
		break;

	case SB_PAGERIGHT:
		x += 10;
		break;

	case SB_THUMBTRACK:
		x = nPos;
		break;

	default:
		break;
	}

	int x_max = GetScrollLimit(SB_HORZ);
	if (x > x_max) x = x_max - 1;
	if (x < 0) x = 0;

	SetScrollPos(SB_HORZ, x);
	Invalidate();

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTransferView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int y = GetScrollPos(SB_VERT);

	switch (nSBCode)
	{
	case SB_TOP:
		y = 0;
		break;

	case SB_BOTTOM:
		y = GetScrollLimit(SB_VERT) - 1;
		break;

	case SB_LINEUP:
		y -= 1;
		break;

	case SB_LINEDOWN:
		y += 1;
		break;

	case SB_PAGEUP:
		y -= 10;
		break;

	case SB_PAGEDOWN:
		y += 10;
		break;

	case SB_THUMBTRACK:
		y = nPos;
		break;

	default:
		break;
	}

	int y_max = GetScrollLimit(SB_VERT);
	if (y > y_max) y = y_max - 1;
	if (y < 0) y = 0;

	SetScrollPos(SB_VERT, y);
	Invalidate();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTransferView::OnPaint()
{
	static bool first_paint = 1;

	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CWnd::OnPaint()

	if (first_paint == 1)
	{
		first_paint = 0;
		ShowScrollBar(SB_BOTH);
		SCROLLINFO scroll_info;
		scroll_info.fMask = SIF_DISABLENOSCROLL;
		SetScrollInfo(SB_HORZ, &scroll_info);
		SetScrollInfo(SB_VERT, &scroll_info);
		//EnableScrollBarCtrl(SB_HORZ);
		//EnableScrollBarCtrl(SB_VERT);

		//ResetScrollRange();
	}

	CEditDistanceDlg *pDlg = (CEditDistanceDlg*)GetParent();
	if (pDlg == NULL) return;

	string str1 = pDlg->GetEditDistance().GetOriginalString();
	string str2 = pDlg->GetEditDistance().GetTargetString();

	CFont font1;
	CFont font2;
	font1.CreateFont(16, 0, 000, 000, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
					OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
					VARIABLE_PITCH | FF_ROMAN, CString("Courier New"));
	font2.CreateFont(16, 0, 000, 000, FW_NORMAL, 0, 1, 0, ANSI_CHARSET,
					OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
					VARIABLE_PITCH | FF_ROMAN, CString("Courier New"));
	CFont *pOldFont = dc.SelectObject(&font1);
	int oldTextAlign = dc.SetTextAlign(TA_TOP|TA_LEFT);

	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen *pOldPen = dc.SelectObject(&pen);

	CSize singleStrSize = dc.GetTextExtent(CString(" "));
	CSize strSize1 = dc.GetTextExtent(CString("initial strings"));
	CSize strSize2 = dc.GetTextExtent(CString(str1.c_str()));
	CSize strSize3 = dc.GetTextExtent(CString(str2.c_str()));

	CRect wndRect;
	GetClientRect(wndRect);

	CRgn rgn;
	rgn.CreateRectRgn(wndRect.left, wndRect.top, wndRect.right, wndRect.bottom);
	dc.SelectClipRgn(&rgn, RGN_COPY);
	dc.FillSolidRect(wndRect, RGB(255, 255, 255));

	int dx = GetScrollPos(SB_HORZ);
	int dy = GetScrollPos(SB_VERT);

	int strTotalWidth = strSize1.cx + strSize2.cx + strSize3.cx + 2*columnBlank + singleStrSize.cx;
	int baseColumn1 = wndRect.Width() > strTotalWidth+80 ? (wndRect.Width()-strTotalWidth)/2-dx : 40-dx;
	int baseColumn2 = baseColumn1 + strSize1.cx + columnBlank;
	int baseColumn3 = baseColumn2 + strSize2.cx + columnBlank;

	dc.MoveTo(baseColumn1, rowBlank/2 - dy);
	dc.LineTo(baseColumn1+strTotalWidth, rowBlank/2 - dy);

	dc.TextOut(baseColumn1, rowBlank - dy, CString("Operation"));
	dc.TextOut(baseColumn2, rowBlank - dy, CString("x"));
	dc.TextOut(baseColumn3, rowBlank - dy, CString("y"));

	int baseRow = rowBlank*2 + singleStrSize.cy - dy;

	dc.SelectObject(pOldPen);
	dc.MoveTo(baseColumn1, baseRow-rowBlank/2);
	dc.LineTo(baseColumn1+strTotalWidth, baseRow-rowBlank/2);

	if (str1.size() != 0 || str2.size() != 0)
	{
		dc.TextOut(baseColumn1, baseRow, CString("initial strings"));
		dc.SelectObject(&font2);
		dc.TextOut(baseColumn2, baseRow, (str1.size() > 0 ? CString(str1[0]) : CString(" ")));
		dc.SelectObject(&font1);
		if(str1.size() > 0) dc.TextOut(baseColumn2+singleStrSize.cx, baseRow, CString(str1.c_str()+1));
		dc.SelectObject(&font2);
		dc.TextOut(baseColumn3, baseRow, CString(" "));
		dc.SelectObject(&font1);

		unsigned int originalIndex = 0;
		unsigned int targetIndex = 0;

		for (unsigned int i = 0; i < pDlg->GetEditDistance().GetOPChain().size(); ++i)
		{
			baseRow += (singleStrSize.cy + rowBlank);

			switch (pDlg->GetEditDistance().GetOPChain()[i])
			{
			case OP_COPY:
				dc.TextOut(baseColumn1, baseRow, CString("copy"));
				originalIndex += 1;
				targetIndex += 1;
				break;

			case OP_REPLACE:
				dc.TextOut(baseColumn1, baseRow, CString("replace"));
				originalIndex += 1;
				targetIndex += 1;
				break;

			case OP_DELETE:
				dc.TextOut(baseColumn1, baseRow, CString("delete"));
				originalIndex += 1;
				break;

			case OP_INSERT:
				dc.TextOut(baseColumn1, baseRow, CString("insert"));
				targetIndex += 1;
				break;

			case OP_TWIDDLE:
				dc.TextOut(baseColumn1, baseRow, CString("twiddle"));
				originalIndex += 2;
				targetIndex += 2;
				break;

			case OP_KILL:
				dc.TextOut(baseColumn1, baseRow, CString("kill"));
				originalIndex = str1.size();
				break;
			}

			unsigned int j;

			for (j = 0; j < str1.size(); ++j)
			{
				if (j == originalIndex) dc.SelectObject(&font2);
				dc.TextOut(baseColumn2 + j * singleStrSize.cx, baseRow, CString(str1[j]));
				if (j == originalIndex) dc.SelectObject(&font1);
			}

			if (originalIndex == str1.size())
			{
				dc.SelectObject(&font2);
				dc.TextOut(baseColumn2 + str1.size()*singleStrSize.cx, baseRow, CString(" "));
				dc.SelectObject(&font1);
			}

			for (j = 0; j < targetIndex; ++j)
				dc.TextOut(baseColumn3 + j * singleStrSize.cx, baseRow, CString(str2[j]));

			dc.SelectObject(&font2);
			dc.TextOut(baseColumn3 + j * singleStrSize.cx, baseRow, CString(" "));
			dc.SelectObject(&font1);
		}

		baseRow += (singleStrSize.cy + rowBlank / 2);
		dc.MoveTo(baseColumn1, baseRow);
		dc.LineTo(baseColumn1+strTotalWidth, baseRow);
	}

	dc.SelectObject(pOldFont);
	dc.SetTextAlign(oldTextAlign);
}
