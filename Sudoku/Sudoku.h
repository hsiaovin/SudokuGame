#pragma once

class Sudoku
{
public:
	Sudoku();
	Sudoku(char* data);
	~Sudoku() { }

	struct Cell;

	void Set(int i, int j, char v);
	char Get(int i, int j);
	bool IsOrigin(int i, int j);
	bool IsValid(int i, int j);
	bool IsFinish();

private:
	Cell* m_Grid[9][9];

	bool checkRows();
	bool checkCols();
	bool checkBlocks();

	bool CheckRow(int i, int j);
	bool CheckCol(int i, int j);
	bool CheckBlock(int i, int j);

};

