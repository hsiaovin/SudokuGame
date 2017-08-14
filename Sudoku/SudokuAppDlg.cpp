
// SudokuDlg.cpp : ʵ���ļ�
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

// CSudokuDlg �Ի���

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


// CSudokuDlg ��Ϣ�������

BOOL CSudokuAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_pSudoku = new SudokuGame(this);
	SetTimer(ID_TIMER, 1000, NULL);

	// ���ô��ڴ�С
	CRect client;
	GetClientRect(client);
	int size = m_pSudoku->GetBoardSize();
	MoveWindow(client.left, client.top, 
		client.left+size+15, client.top+size+80, FALSE);

	// ����Button��Static��λ��
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSudokuAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_pSudoku->DrawBoard();
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSudokuAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSudokuAppDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pSudoku->Select(point);
}

BOOL CSudokuAppDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN) {
		if (m_pSudoku->OnKeyDown(pMsg->wParam)) 
			return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSudokuAppDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
