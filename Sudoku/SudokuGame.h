#pragma once

#include "stdafx.h"
#include "Sudoku.h"

class SudokuGame
{
public:
	SudokuGame(CWnd* Cwnd);
	~SudokuGame() { }

	void NewGame();
	void DrawBoard();
	bool OnKeyDown(WPARAM msg);
	void Move(WPARAM key);
	void Select(CPoint point);
	void Set(char value);
	void TimeCounting();
	CString GetTimer();
	int GetBoardSize();

private:
	static UINT NewGameThread(void *param);

	void DrawCell(int x, int y);
	CRect CellRect(int i, int j);

	CPoint CursorToCell(CPoint cur);

	inline int X(int i) {
		int x = m_Padding + 2 * m_Margin + i * m_CellWidth;
		if (i >= 3) x += 2 * m_Margin;
		if (i >= 6) x += 2 * m_Margin;
		return x;
	}

	inline int WithIn(int x) {
		for (int i = 0; i < 9; i++) {
			if (X(i) <= x && X(i + 1) > x) return i;
		}
		return -1;
	}

private:
	CWnd* m_pCwnd;
	CDC* m_pDC;
	CFont m_Font;

	const int m_BoardWidth = 400;
	const int m_Margin = 2;
	const int m_Padding = 20;
	const int m_CellWidth = (m_BoardWidth - 6 * m_Margin) / 9;

	Sudoku m_Board;
	bool m_bIsBegin;
	bool m_bIsLoading;
	CTime m_Timer;
	int m_SelectedX;
	int m_SelectedY;

	// color
	const COLORREF CellColor = RGB(250, 250, 250);
	const COLORREF TextColor = RGB(150, 150, 150);
	const COLORREF OriginCellColor = RGB(240, 240, 240);
	const COLORREF OriginTextColor = RGB(90, 90, 90);
	const COLORREF SelectedCellColor = RGB(238, 249, 254);
	const COLORREF SelectedTextColor = RGB(238, 249, 254);
	const COLORREF WarningColor = RGB(254, 238, 251);

private:
	// Utils
	static CString HttpGet(_TCHAR* url);
	static char* UnicodeToChar(CString src);
};

