
// SudokuDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SudokuApp.h"
#include "SudokuAppDlg.h"
#include "SudokuGame.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER 0

// CSudokuDlg 对话框

CSudokuAppDlg::CSudokuAppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SUDOKU_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSudokuAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSudokuAppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CSudokuAppDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CSudokuAppDlg::OnNMCustomdrawProgress1)
END_MESSAGE_MAP()


// CSudokuDlg 消息处理程序

BOOL CSudokuAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_pSudoku = new SudokuGame(this);
	SetTimer(ID_TIMER, 1000, NULL);

	// 设置窗口大小
	CRect client;
	GetClientRect(client);
	int size = m_pSudoku->GetBoardSize();
	MoveWindow(client.left, client.top, 
		client.left+size+15, client.top+size+80, FALSE);

	// 设置Button和Static的位置
	CWnd* pWButton = GetDlgItem(IDOK);
	int buttonSize = 110;
	pWButton->SetWindowPos(NULL, client.top+size-buttonSize, 
		client.left+size, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	CWnd* pWStatic = GetDlgItem(IDC_STATIC);
	pWStatic->SetWindowPos(pWButton, 270, 450, 0, 0, 
		SWP_NOZORDER | SWP_NOSIZE);

	//CProgressCtrl *pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	//int nPrePos = pProgCtrl->StepIt();
	//pProgCtrl->SetRange(0, 100);
	//pProgCtrl->SetStep(1);
	//for (int j = 0; j < 100; j++) {
	//	for (int i = 0; i < 100; i++) {
	//		pProgCtrl->SetPos(i);
	//	}
	//}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSudokuAppDlg::OnPaint()
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
		m_pSudoku->DrawBoard();
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSudokuAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSudokuAppDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pSudoku->Select(point);
}

BOOL CSudokuAppDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN) {
		if (m_pSudoku->OnKeyDown(pMsg->wParam)) 
			return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSudokuAppDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pSudoku->NewGame();
	// Loading Effect
	AfxBeginThread(ProgressThread, this, THREAD_PRIORITY_IDLE);
}

UINT CSudokuAppDlg::ProgressThread(void* param) {
	CWnd* pCwnd = (CWnd*)param;
	CRect client;
	pCwnd->GetClientRect(client);
	CRect ProgRect = CRect(client.left, client.top, client.right, client.left + 4);
	CProgressCtrl *pProgCtrl = new CProgressCtrl();
	pProgCtrl->Create(WS_VISIBLE, ProgRect, pCwnd, 99);
	pProgCtrl->SetRange(0, 100);
	pProgCtrl->SetStep(1);
	for (int i = 0; i < 5000; i++) {
		pProgCtrl->SetPos(i);
	}
	delete pProgCtrl;
	return 0;
}

void CSudokuAppDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);
	switch (nIDEvent) {
	case ID_TIMER:
	{
		m_pSudoku->TimeCounting();
		SetDlgItemText(IDC_STATIC, m_pSudoku->GetTimer());
		break;
	}
	default:
		KillTimer(nIDEvent);
		break;
	}
}

void CSudokuAppDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
