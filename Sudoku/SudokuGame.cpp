#include "stdafx.h"
#include "SudokuGame.h"
#include <WinInet.h>

#pragma comment(lib, "wininet.lib")


SudokuGame::SudokuGame(CWnd* Cwnd)
	: m_pCwnd(Cwnd), m_bIsBegin(false), m_bIsLoading(false),
	  m_Timer(1997, 1, 1, 0, 0, 0), m_SelectedX(-1), m_SelectedY(-1) {

	m_pDC = Cwnd->GetDC();
	m_Font.CreateFontW(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arival"));
	m_pDC->SelectObject(m_Font);
}

void SudokuGame::DrawBoard() {
	CRect border = CRect(m_Padding-1, m_Padding-1, m_BoardWidth + m_Margin * 2 + m_Padding+1, m_BoardWidth + m_Margin * 2 + m_Padding+1);
	m_pDC->FillSolidRect(border, RGB(200, 200, 200));

	CRect canvas = CRect(m_Padding, m_Padding, m_BoardWidth + m_Margin * 2 + m_Padding, m_BoardWidth + m_Margin * 2 + m_Padding);
	m_pDC->FillSolidRect(canvas, RGB(150, 150, 150));

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int gap = m_Margin;
			int w = m_BoardWidth / 3;
			int pointx = j*w + m_Margin + m_Padding;
			int pointy = i*w + m_Margin + m_Padding;
			CRect rect = CRect(pointx + gap, pointy + gap, pointx + w - gap, pointy + w - gap);
			m_pDC->FillSolidRect(rect, RGB(200, 200, 200));
		}
	}

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			DrawCell(i, j);
		}
	}
}

int SudokuGame::GetBoardSize() {
	return m_BoardWidth + 2*m_Margin + 2*m_Padding;
}

UINT SudokuGame::NewGameThread(void *param) {
	CString str = HttpGet(_T("http://show.websudoku.com/?level=3"));
	int idx = str.Find(_T("<INPUT NAME=cheat ID=\"cheat\" TYPE=hidden VALUE="));
	CString cData = str.Mid(idx + 48, 81);
	idx = str.Find(_T("<INPUT ID=\"editmask\" TYPE=hidden VALUE="));
	CString cMask = str.Mid(idx + 40, 81);

	//char* data = UnicodeToChar(cData);
	char* data = new char[81];
	for (int i = 0; i < 81; i++) {
		if (cMask[i] == _TCHAR('0')) 
			data[i] = char(cData[i]);
	}

	SudokuGame *game = (SudokuGame*)param;
	game->m_Board = Sudoku(data);
	game->DrawBoard();
	game->m_Timer = CTime(1997, 1, 1, 0, 0, 0);
	game->m_bIsBegin = true;
	game->m_bIsLoading = false;
	return 0;
}

void SudokuGame::NewGame() {
	if (m_bIsLoading) return;
	m_bIsBegin = false;
	m_bIsLoading = true;
	AfxBeginThread(NewGameThread, this, THREAD_PRIORITY_IDLE);
}

void SudokuGame::TimerUpdate() {
	if (m_bIsBegin) m_Timer += CTimeSpan(0, 0, 0, 1);
}

CString SudokuGame::GetTimer() {
	if (m_Timer.GetHour() > 0)
		return m_Timer.Format(_T("%H:%M:%S"));
	else
		return m_Timer.Format(_T("%M:%S"));
}

CRect SudokuGame::CellRect(int i, int j) {
	int gap = m_Margin / 2;
	POINT pos = { X(i) + gap, X(j) + gap };
	SIZE size = { m_CellWidth - gap, m_CellWidth - gap };
	return CRect(pos, size);
}

CPoint SudokuGame::CursorToCell(CPoint cursor) {
	return { WithIn(cursor.x), WithIn(cursor.y) };
}

void SudokuGame::DrawCell(int x, int y) {
	if (x < 0 || x >= 9 || y < 0 || y >= 9) return;
	
	CRect rect = CellRect(x, y);
	if (m_SelectedX == x && m_SelectedY == y) {
		m_pDC->SetTextColor(TextColor);
		m_pDC->FillSolidRect(rect, SelectedCellColor);
	}
	else if (m_Board.IsOrigin(x, y)) {
		m_pDC->SetTextColor(OriginTextColor);
		m_pDC->FillSolidRect(rect, OriginCellColor);
	} 
	else if (!m_Board.IsValid(x, y)) {
		m_pDC->SetTextColor(TextColor);
		m_pDC->FillSolidRect(rect, WarningColor);
	} 
	else {
		m_pDC->SetTextColor(TextColor);
		m_pDC->FillSolidRect(rect, CellColor);
	}
	m_pDC->DrawText(CString(m_Board.Get(x, y)), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void SudokuGame::Select(CPoint point) {
	POINT p = CursorToCell(point);

	int old_x = m_SelectedX, old_y = m_SelectedY;
	m_SelectedX = p.x;
	m_SelectedY = p.y;
	DrawCell(old_x, old_y); // clear old selected;
	DrawCell(m_SelectedX, m_SelectedY);
}

bool SudokuGame::OnKeyDown(WPARAM msg) {
	if ((msg >= '1' && msg <= '9') || msg == ' ') {
		Set(msg);
		return true;
	}
	else if (msg >= VK_LEFT && msg <= VK_DOWN) {
		Move(msg);
		return true;
	}
	return false;
}

void SudokuGame::Move(WPARAM key) {
	int old_x = m_SelectedX, old_y = m_SelectedY;
	switch (key) {
	case VK_LEFT:
		if (old_x > 0) m_SelectedX -= 1;
		break;
	case VK_RIGHT:
		if (old_x < 8) m_SelectedX += 1;
		break;
	case VK_UP:
		if (old_y > 0) m_SelectedY -= 1;
		break;
	case VK_DOWN:
		if (old_y < 8) m_SelectedY += 1;
		break;
	}
	DrawCell(old_x, old_y);
	DrawCell(m_SelectedX, m_SelectedY);
}

void SudokuGame::Set(char value) {
	if (m_SelectedX == -1 || m_SelectedY == -1) return;

	m_Board.Set(m_SelectedX, m_SelectedY, value);
	DrawCell(m_SelectedX, m_SelectedY);

	if (m_Board.IsFinish()) {
		m_bIsBegin = false;
		AfxMessageBox(_T("恭喜！\n 你已成功完成，用时为")+GetTimer()+"。");
	}
}

CString SudokuGame::HttpGet(_TCHAR* url) {
	CString res;
	HINTERNET hSession = InternetOpen(_T("Chrome"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL) {
		HINTERNET hHttp = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (hHttp != NULL) {
			const int TEMP_SIZE = 8192;
			char Temp[TEMP_SIZE];
			ULONG Number = 1;
			while (Number > 0) {
				InternetReadFile(hHttp, Temp, TEMP_SIZE - 1, &Number);
				Temp[Number] = '\0';
				res.Append(CString(Temp));
			}
			InternetCloseHandle(hHttp);
			hHttp = NULL;
		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
	return res;
}

char* SudokuGame::UnicodeToChar(CString src) {
	//int iLen = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	//if (iLen == 0) return "";
	//char *dst = new char[iLen];
	//WideCharToMultiByte(CP_ACP, 0, src, -1, dst, iLen, NULL, NULL);
	//return dst;

	//size_t len = wcslen(src) + 1;
	//char* dst = new char[len];
	//wcstombs_s(NULL, dst, len, src, len);
	//return dst;
}
