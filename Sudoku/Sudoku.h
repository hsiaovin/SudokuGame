
// Sudoku.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSudokuApp: 
// �йش����ʵ�֣������ Sudoku.cpp
//

class CSudokuApp : public CWinApp
{
public:
	CSudokuApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSudokuApp theApp;
