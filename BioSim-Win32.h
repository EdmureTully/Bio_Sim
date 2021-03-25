#pragma once

#include "resource.h"
#include "CreatureProps.h"
#include <Windows.h>
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib") 
#include <gl\gl.h>
#include    <gl\glu.h>  

//Vorwärtsdeklarationen
ATOM				MyRegisterClass(HINSTANCE hInstance, TCHAR szWindowClass[]);
BOOL				InitInstance(HINSTANCE, int, TCHAR szTitle[], TCHAR szWindowClass[]);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	MenuProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	OpenGLProc(HWND, UINT, WPARAM, LPARAM);


void updateSelection(const std::vector <CreatureProps> &creatVector, HWND &hWnd, int pos);
void createButton(HWND &hWnd, int id, LPCTSTR lpWindowName);
void createEditwindow(HWND &hWnd, int id, LPCWSTR lpWindowName);
void createEmptyEditwindow(HWND &hWnd, int id, LPCWSTR lpWindowName);
void setPositionSelectMenu(HWND &hWnd);
void setPositionButtongroup(HWND &hWnd);
void getCreatureNames(const std::vector <CreatureProps> &creatVector, HWND &hWnd);
HWND createSelectMenu(HWND &hWnd);
HWND createButtonGroup(HWND &hWnd);

UINT getCreatureStrength(const std::vector <CreatureProps> &creatVector, int pos);
UINT setCreatureSpeed(const std::vector <CreatureProps> &creatVector, int pos);
UINT setCreatureLifetime(const std::vector <CreatureProps> &creatVector, int pos);

std::wstring setCreatureProperties(const std::vector <CreatureProps> &creatVector, int pos);

std::wstring stringToWstring(const std::string& s);
inline UINT AddString(const HWND &combo, const std::wstring &s);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
RECT getClientAreaInGroupBox(HWND &dlg, int id);
