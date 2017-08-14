#include "stdafx.h"
#include "Sudoku.h"

#pragma comment(lib, "wininet.lib")

struct Sudoku::Cell {
	const bool isOrigin;
	char value;

	Cell(char v, bool is_origin)
		: isOrigin(is_origin), value(v) { }
};

Sudoku::Sudoku() {
	for (int i = 0; i < 81; i++) {
		m_Grid[i / 9][i % 9] = new Cell(' ', false);
	}
}

Sudoku::Sudoku(char* data) {
	for (int i = 0; i < 81; i++) {
		bool isOri = (data[i] >= '1' && data[i] <= '9') ? TRUE: FALSE;
		m_Grid[i / 9][i % 9] = new Cell(isOri ? data[i] : ' ', isOri);
	}
}

void Sudoku::Set(int i, int j, char v) {
	if (i < 0 || i >= 9 || j < 0 || j >= 9) return;
	if (!m_Grid[i][j]->isOrigin) m_Grid[i][j]->value = v;
}

char Sudoku::Get(int i, int j) {
	if (i < 0 || i >= 9 || j < 0 || j >= 9) return ' ';
	return m_Grid[i][j]->value;
}

bool Sudoku::IsOrigin(int i, int j) {
	if (i < 0 || i >= 9 || j < 0 || j >= 9) return false;
	return m_Grid[i][j]->isOrigin;
}

bool Sudoku::IsValid(int i, int j) {
	return CheckRow(i, j) && CheckCol(i, j) && CheckBlock(i, j);
}

bool Sudoku::IsFinish() {
	return checkRows() && checkCols() && checkBlocks();
}

bool Sudoku::CheckRow(int x, int y) {
	char val = Get(x, y);
	for (int i = 0; i < 9; i++) {
		if (Get(x, i) == val && Get(x, i) != ' ' && i != y) {
			return false;
		}
	}
	return true;
}

bool Sudoku::CheckCol(int x, int y) {
	char val = Get(x, y);
	for (int i = 0; i < 9; i++) {
		if (Get(i, y) == val && Get(i, y) != ' ' && i != x) {
			return false;
		}
	}
	return true;
}

bool Sudoku::CheckBlock(int x, int y) {
	char val = Get(x, y);
	for (int i = x / 3 * 3; i < x / 3 * 3 + 3; i++) {
		for (int j = y / 3 * 3; j < y / 3 * 3 + 3; j++) {
			if (Get(i, y) == val && Get(i, y) != ' ' && i != x && j != y) {
				return false;
			}
		}
	}
	return true;
}

bool Sudoku::checkRows() {
	for (int i = 0; i < 9; i++) {
		int bits = 0x01FF;
		for (int j = 0; j < 9; j++) {
			bits ^= 1 << (Get(i, j) - '1');
		}
		if (bits != 0) return false;
	}
	return true;
}

bool Sudoku::checkCols() {
	for (int i = 0; i < 9; i++) {
		int bits = 0x01FF;
		for (int j = 0; j < 9; j++) {
			bits ^= 1 << (Get(j, i) - '1');
		}
		if (bits != 0) return false;
	}
	return true;
}

bool Sudoku::checkBlocks() {
	for (int i = 0; i < 9; i+=3) {
		for (int j = 0; j < 9; j+=3) {

			int bits = 0x01FF;
			for (int m = i; m < 3+i; m++) {
				for (int n = j; n < 3+j; n++) {
					bits ^= 1 << (Get(m, n) - '1');
				}
			}
			if (bits != 0) return false;

		}
	}
	return true;
}