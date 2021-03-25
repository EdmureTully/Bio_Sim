// Win32Project4.cpp : Definiert den Einstiegspunkt für die Anwendung.
//
#include "stdafx.h"
#include "BioSim-Win32.h"
#include "Ki.h"

#include <commctrl.h>
#include <vector>
#include <stdlib.h> 
#include "Util.h"
//Karte, andere Lösung finden
Map  map(100,100);
Ki ki(map);
const UINT TIMER_SECOND = 1234;
HWND CreateOpenGLWindow(HWND &hMain);
GLvoid KillGLWindow(HWND &hWnd, HGLRC hGlrc, HDC hDC);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	TCHAR szWindowClass[MAX_LOADSTRING];
	BOOL done = FALSE;
	MSG msg;
	TCHAR szTitle[MAX_LOADSTRING];
	// Globale Zeichenfolgen initialisieren
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);
	
	// Anwendungsinitialisierung ausführen:
	if (!InitInstance (hInstance, nCmdShow, szTitle, szWindowClass ))
	{
		MessageBox(NULL, L"Instanz konnte nicht erstellt werden.", L"Error.", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	
	RECT OGL;
	HWND Find = FindWindowEx(NULL, NULL, szWindowClass, NULL);
	GetClientRect(GetDlgItem(Find, IDC_OPENGL), &OGL);
	int hscroll = GetSystemMetrics(SM_CXHSCROLL);
	int vscroll = GetSystemMetrics(SM_CXVSCROLL);
	int right = OGL.right - hscroll;
	int bottom = OGL.bottom - 2*vscroll;
	
	
	while (!done){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT){
				done = TRUE;
			}
			else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else{
			if (!done ){
				int pos = map.getSelection();
				DrawGLScene(map, right, bottom, pos);
				map.setSelection( -1);
				SwapBuffers(GetDC(GetDlgItem(Find, IDC_OPENGL)));
			}
		}
	}
	

	return (int) msg.wParam;
}



//Registriert die Klassen
ATOM MyRegisterClass(HINSTANCE hInstance, TCHAR szWindowClass[])
{
	WNDCLASSEX wcex;
	
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT4));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(LTGRAY_BRUSH);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ASTARMENU);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	RegisterClassEx(&wcex);

	wcex.lpfnWndProc = MenuProc;
	wcex.lpszClassName = L"Menü";
	wcex.hbrBackground = (HBRUSH)(LTGRAY_BRUSH);
	RegisterClassEx(&wcex);
	wcex.style =   CS_OWNDC;
	wcex.lpfnWndProc = OpenGLProc;
	wcex.hbrBackground = NULL;
	wcex.lpszClassName = L"OpenGL";
	wcex.hbrBackground = (HBRUSH)(LTGRAY_BRUSH);
	
	
	
	
	
	return RegisterClassEx(&wcex);
}

//Instanziiert das Hauptfenster

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, TCHAR szTitle[], TCHAR szWindowClass[])
{
   HWND hWnd, child, hOpenGL = NULL;
   HINSTANCE hInst;
   
   hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

   hWnd = CreateWindow(szWindowClass, szTitle, WS_BORDER | WS_SYSMENU,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   hOpenGL = CreateOpenGLWindow(hWnd);
   
   child = CreateWindowEx(WS_EX_WINDOWEDGE,
	   L"Menü",
	   L"Menü", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN,
	   10, 0, 100, 150, hWnd, (HMENU)IDC_MenuSeite, GetModuleHandle(NULL), NULL);

   
   if (!hWnd || !child || !hOpenGL)
   {
	  MessageBox(hWnd, L"Hauptfenster konnte nicht erstellt werden.", L"Error.", MB_OK | MB_ICONERROR);
	  KillGLWindow(hWnd, NULL, NULL);
	  KillGLWindow(child, NULL, NULL);
	  return FALSE;
   }
 
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//Prozedur für das Hauptfenster.
//Erzeugt den Raum für die Anwendung und die Statusleiste.
//Erzeugt außerdem ein Kindfenster für das Seitmenü.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND  hStatus = NULL;
	switch (message)
	{
	case WM_CREATE:{
		
					    
		hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE ,
				0, 0, 0, 0, hWnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);
					   
		if ( hStatus == NULL){
			MessageBox(hWnd, L"Anwendungsbereich konnte nicht erzeugt werden.", L"Error.", MB_OK | MB_ICONERROR);
			return 0;
		}
					   
		int statwidths[] = { 100, -1 };
		
		std::wstring stemp(stringToWstring("Status: Bereit."));
		LPCWSTR sw = stemp.c_str();
		std::wstring stemp1(stringToWstring("Anzahl Schritte: 0   Anzahl Kreaturen: 0  Anzahl Pflanzen: 0"));
		LPCWSTR sw1 = stemp1.c_str();
		
		
		SendMessage(hStatus, SB_SETPARTS, 2, (LPARAM)&statwidths);
		
		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)sw);
		SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)sw1);
	}
		break;
	case WM_SIZE:
	{
		SendDlgItemMessage(hWnd, IDC_OPENGL, WM_SIZE, 0, 0);
		
	}
		break;
	case WM_TIMER:
	{
		ki.makeMove();
		std::string s = "Anzahl Schritte: ";
		s += std::to_string(ki.getSteps());
		s += "   Anzahl Kreaturen: " + std::to_string(map.wateranimals.size()  +  map.landanimals.size());
		s += "   Anzahl Pflanzen: " + std::to_string(map.waterplants.size() + map.landplants.size());
		std::wstring stemp1(stringToWstring(s));
		LPCWSTR sw = stemp1.c_str();
		SendMessage(GetDlgItem(hWnd, IDC_MAIN_STATUS), SB_SETTEXT, 1, (LPARAM)sw);
	}break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
	{
			case IDC_ASTERN:
				map.getCursor().setAStarActiv(true);
				break;
			case IDC_ASTERNDEAK:
				map.getCursor().setAStarActiv(false);
				break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


//Prozedur für das Seitenmenü.
//Erzeugt die nötigen Boxen in der WM_CREATE Anweisung
//und positioniert sie in der WM_SIZE Anweisung.
//Beehandelt Events für den Fall, dass die Knöpfe gedrückt werden.

LRESULT CALLBACK MenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	
	int wmId, wmEvent;
	LPCTSTR prop = NULL;
	bool placement = false;
	static bool start = false;
	std::wstring properties;
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Menüauswahl bearbeiten:
		switch (wmId)
		{
		case IDC_CHECK1:
			if (HIWORD(wParam) == BN_CLICKED){
				MessageBox(hWnd, L"Die Simulation sollte starten.", L"Start", MB_OK);
				std::wstring stemp(stringToWstring("Status: Gestartet."));
				LPCWSTR sw = stemp.c_str();
				SendMessage(GetDlgItem(GetParent(hWnd), IDC_MAIN_STATUS), SB_SETTEXT, 0, (LPARAM)sw);
				SetTimer(GetParent(hWnd), TIMER_SECOND, 2500, NULL);
				start = true;
			}
			break;
		case IDC_CHECK2:
			if (HIWORD(wParam) == BN_CLICKED){
				MessageBox(hWnd, L"Pause wurde gedrückt.", L"Pause", MB_OK);
				std::wstring stemp(stringToWstring("Status: Pausiert."));
				LPCWSTR sw = stemp.c_str();
				SendMessage(GetDlgItem(GetParent(hWnd), IDC_MAIN_STATUS), SB_SETTEXT, 0, (LPARAM)sw);
				KillTimer(GetParent(hWnd), TIMER_SECOND);
				start = false;
				
			}
			break;
		case IDC_CHECK3:
			if (HIWORD(wParam) == BN_CLICKED){
			if (!start){
				ki.makeMove();
				std::string s = "Anzahl Schritte: ";
				s += std::to_string(ki.getSteps());
				s += "   Anzahl Kreaturen: " + std::to_string(map.wateranimals.size() +  map.landanimals.size());
				s += "   Anzahl Pflanzen: " + std::to_string(map.waterplants.size() + map.landplants.size());
				std::wstring stemp1(stringToWstring(s));
				LPCWSTR sw = stemp1.c_str();
				std::wstring stemp(stringToWstring("Status: Schritt."));
				LPCWSTR sw1 = stemp.c_str();
				SendMessage(GetDlgItem(GetParent(hWnd), IDC_MAIN_STATUS), SB_SETTEXT, 0, (LPARAM)sw1);
				SendMessage(GetDlgItem(GetParent(hWnd), IDC_MAIN_STATUS), SB_SETTEXT, 1, (LPARAM)sw);
				
			}
				
			if (start)
				MessageBox(GetParent(hWnd), L"Einzelschritt ist bei laufender Simulation nicht möglich!", L"Schritt", MB_OK);
			}
			break;
		case IDC_PLATZIEREN:
			if (HIWORD(wParam) == BN_CLICKED && !start)
				map.setSelection(SendDlgItemMessage(hWnd, IDC_COMBO, CB_GETCURSEL, 0, 0));
			if (HIWORD(wParam) == BN_CLICKED && start)
				MessageBox(hWnd, L"Platzieren ist bei laufender Simulation nicht möglich!", L"Schritt", MB_OK);
			break;
		case IDC_COMBO:
			if (HIWORD(wParam) == CBN_DBLCLK)
				SendDlgItemMessage(hWnd, IDC_COMBO, CB_SETCURSEL, 0, 0);
				updateSelection(map.vCreature, hWnd, SendDlgItemMessage(hWnd, IDC_COMBO, CB_GETCURSEL, 0, 0));
				map.setPreselection (SendDlgItemMessage(hWnd, IDC_COMBO, CB_GETCURSEL, 0, 0));
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == (GetDlgItem(hWnd, IDC_EDITEIGEN)) ||
			(HWND)lParam == (GetDlgItem(hWnd, IDC_EDITGESCHW)) ||
			(HWND)lParam == (GetDlgItem(hWnd, IDC_EDITSTAERKE)) ||
			(HWND)lParam == (GetDlgItem(hWnd, IDC_EDITLEBEN))) {
			HDC hDC = (HDC)wParam;
			SetBkMode(hDC, TRANSPARENT);
			return (LRESULT)(HBRUSH(LTGRAY_BRUSH));
		}
		else return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_CREATE:{
		
		HWND  hGroupButtons, hGroupSelect;
		RECT  rc, rc2;
		hGroupSelect = createSelectMenu( hWnd);
					   
		if (hGroupSelect == NULL){
			MessageBox(hWnd, L"Auswahlmenü konnte nicht erstellt werden.", L"Error.", MB_OK | MB_ICONERROR);
			return 0;
		}
					   
		hGroupButtons = createButtonGroup(hWnd);

		if (hGroupButtons == NULL){
			MessageBox(hWnd, L"Kontrollmenü konnte nicht erstellt werden.", L"Error.", MB_OK | MB_ICONERROR);
			return 0;
		}
		HWND temp = GetDlgItem(hWnd, IDC_COMBO);
		getCreatureNames(map.vCreature, temp );
		SendDlgItemMessage(hWnd, IDC_COMBO, CB_SETCURSEL, 0, 0);
		updateSelection(map.vCreature, hWnd, SendDlgItemMessage(hWnd, IDC_COMBO, CB_SETCURSEL, 0, 0));
		setPositionButtongroup(hWnd);
		setPositionSelectMenu(hWnd);

		GetClientRect(GetDlgItem(GetParent(hWnd), IDC_OPENGL), &rc);
		GetClientRect(GetParent(hWnd), &rc2);
		SetWindowPos(hWnd, NULL, rc.right + 30, 0, (int)(0.25* rc2.right), rc2.bottom, NULL);
		SetForegroundWindow(hWnd);
	}
		break;
	case WM_TIMER:{
		while (start){
			ki.makeMove();
		 }
		break;
	}
	case WM_SIZE:
	{
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK	OpenGLProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	RECT rect;
	static int count;
	switch (message)
	{
	case WM_CREATE:{
		SCROLLINFO sScroll;
		RECT rcMainwindow, rcStatus;
		int iStatusHeight, iEditheight, iEditwidth;
		GetWindowRect(hWnd, &rcMainwindow);
		rect = rcMainwindow;

		GetWindowRect(GetDlgItem(GetParent(hWnd), IDC_MAIN_STATUS), &rcStatus);
		iStatusHeight = rcStatus.bottom - rcStatus.top;

		GetClientRect(GetParent(hWnd), &rcMainwindow);

		iEditheight = rcMainwindow.bottom - iStatusHeight;
		iEditwidth = (int)(0.75 * rcMainwindow.right);
		SetWindowPos(GetDlgItem(GetParent(hWnd), IDC_OPENGL), NULL, 0, 0, iEditwidth, iEditheight, SWP_NOZORDER);
		

		sScroll.cbSize = sizeof(sScroll);
		sScroll.fMask = SIF_RANGE | SIF_PAGE;
		sScroll.nMin = 0;
		sScroll.nMax = 32 * map.getMapHeight();
		sScroll.nPage = sScroll.nMax / (map.getMapHeight()- iEditwidth/32);
		SetScrollInfo(hWnd, SB_HORZ, &sScroll, TRUE);
		sScroll.nMax = 32 * map.getMapWidth() ;
		sScroll.nPage = sScroll.nMax / (map.getMapWidth() - iEditheight / 32);

		SetScrollInfo(hWnd, SB_VERT, &sScroll, TRUE);
	}break;
	case WM_SIZE:
	{
		RECT rWindow;
		GetClientRect(hWnd, &rWindow);
		//ResizeGLScene(rWindow.right, rWindow.bottom);
	}
	break;
	case WM_VSCROLL:{
		SCROLLINFO sScroll;
		int position;
		sScroll.cbSize = sizeof(sScroll);
		sScroll.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &sScroll);
		position = sScroll.nPos;
		if (map.getCursor().getAStarActiv())map.getCursor().setAStar(false);
		switch (LOWORD(wParam)){
		case SB_TOP:
			sScroll.nPos = sScroll.nMin;
			break;
		case SB_BOTTOM:
			sScroll.nPos = sScroll.nMax;
			break;
		case SB_LINEUP:
			sScroll.nPos -= 1;
			break;
		case SB_LINEDOWN:
			sScroll.nPos += 1;
			break;
		case SB_PAGEUP:
			sScroll.nPos -= sScroll.nPage;
			break;
		case SB_PAGEDOWN:
			sScroll.nPos += sScroll.nPage;
			break;
		case SB_THUMBTRACK:
			sScroll.nPos = sScroll.nTrackPos;
			break;
		default:
			return 0;
		}
		sScroll.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_VERT, &sScroll, TRUE);

		GetScrollInfo(hWnd, SB_VERT, &sScroll);

		if (sScroll.nPos != position){
			map.setYpos(sScroll.nPos);
		}
		
	}
	break;
	case WM_HSCROLL:{
		SCROLLINFO sScroll;
		int position;
		sScroll.cbSize = sizeof(sScroll);
		sScroll.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_HORZ, &sScroll);
		position = sScroll.nPos;
		if (map.getCursor().getAStarActiv())map.getCursor().setAStar(false);
		switch (LOWORD(wParam)){
		case SB_LEFT:
			sScroll.nPos = sScroll.nMin;
			break;
		case SB_RIGHT:
			sScroll.nPos = sScroll.nMax;
			break;
		case SB_LINELEFT:
			sScroll.nPos -= 1;
			break;
		case SB_LINERIGHT:
			sScroll.nPos += 1;
			break;
		case SB_PAGELEFT:
			sScroll.nPos -= sScroll.nPage;
			break;
		case SB_PAGERIGHT:
			sScroll.nPos += sScroll.nPage;
			break;
		case SB_THUMBTRACK:
			sScroll.nPos = sScroll.nTrackPos;
			break;
		}
		sScroll.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_HORZ, &sScroll, TRUE);

		GetScrollInfo(hWnd, SB_HORZ, &sScroll);

		if (sScroll.nPos != position){
			map.setXpos(sScroll.nPos);
		}
		
	}break;
	case WM_LBUTTONDOWN:{
		map.getCursor().setActiv(true);
		
		int y = LOWORD(lParam);
		int x = HIWORD(lParam);
		if ((x != map.getCursor().getOldXpos() || y != map.getCursor().getOldYpos())){
			map.getCursor().setOldXpos(map.getCursor().getXpos());
			map.getCursor().setOldYpos(map.getCursor().getYpos());
			if (map.getCursor().getAStarActiv())map.getCursor().setAStar(true);
		}
		count++;
		map.getCursor().setXpos(x);
		map.getCursor().setYpos(y);
		
		
	}break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// Funktion zur Berechnung des Beriechs einer Groupbox.
RECT getClientAreaInGroupBox(HWND &dlg, int id) {
	HWND group = GetDlgItem(dlg, id);
	RECT rc;
	GetWindowRect(group, &rc);
	MapWindowPoints(0, dlg, (POINT*)&rc, 2);

	RECT border = { 4, 8, 4, 4 };
	
	rc.left += border.left;
	rc.right -= border.right;
	rc.top += border.top;
	rc.bottom -= border.bottom;
	return rc;
}


//Erstellt einen Pushdownbutton
void createButton(HWND &hWnd, int id, LPCWSTR lpWindowName){
	 CreateWindowEx(WS_EX_WINDOWEDGE,
		L"BUTTON",
		lpWindowName,
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		0, 0,
		0, 0,
		hWnd,
		(HMENU)id,
		GetModuleHandle(NULL), NULL);

	 if (GetDlgItem(hWnd, id) == NULL){
		 MessageBox(NULL, L"Button  konnte nicht erzeugt werden.", L"Error.", MB_OK | MB_ICONERROR);
	 }
}
//Erstellt Editbox für das Befüllen mit Kreaturwerten.
void createEditwindow(HWND &hWnd, int id, LPCWSTR lpWindowName){
	CreateWindowEx(WS_EX_WINDOWEDGE,
		L"EDIT",
		lpWindowName,
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY,
		0, 0,
		0, 0,
		hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);

	if (GetDlgItem(hWnd, id) == NULL){
		MessageBox(NULL, L"Editbox  konnte nicht erzeugt werden.", L"Error.", MB_OK | MB_ICONERROR);
	}
}
//Erstellt Editbox mit Namen der Eigenschaften
void createEmptyEditwindow(HWND &hWnd, int id, LPCWSTR lpWindowName){
	CreateWindowEx(WS_DISABLED,
		L"EDIT",
		lpWindowName,
		WS_VISIBLE | WS_CHILD | ES_READONLY,
		0, 0,
		0, 0,
		hWnd, (HMENU)id, GetModuleHandle(NULL), NULL);

	if (GetDlgItem(hWnd, id) == NULL){
		MessageBox(NULL, L"Editbox  konnte nicht erzeugt werden.", L"Error.", MB_OK | MB_ICONERROR);
		}
	
}
//Erstellt das Auswahlmenü
HWND createSelectMenu(HWND &hWnd){
	RECT rc;
	HWND hGroupSelect;
	GetClientRect(hWnd, &rc);
	
	hGroupSelect = CreateWindowEx(0,
		L"BUTTON",
		(L""),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_CLIPCHILDREN ,
		rc.left + 10, 0,
		275, (int)(2.5  *rc.bottom), hWnd
		, (HMENU)IDC_GROUPSELECT, GetModuleHandle(NULL), 0);

	createEditwindow(hWnd, IDC_STAERKE, L"###");
	createEditwindow(hWnd, IDC_GESCHW, L"###");
	createEditwindow(hWnd, IDC_LEBEN, L"###");
	createEditwindow(hWnd, IDC_EIGEN, L"###");
	createEmptyEditwindow(hWnd, IDC_EDITSTAERKE, L"Stärke");
	createEmptyEditwindow(hWnd, IDC_EDITGESCHW, L"Geschw.");
	createEmptyEditwindow(hWnd, IDC_EDITLEBEN, L"Leben");
	createEmptyEditwindow(hWnd, IDC_EDITEIGEN, L"Eigens.");
	createButton(hWnd, IDC_PLATZIEREN, L"Platzieren");

	CreateWindowEx(0,
		L"combobox",
		L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 0, 0, 0, 0, hWnd, (HMENU)IDC_COMBO, GetModuleHandle(NULL), NULL);

	return hGroupSelect;
}
//Erstellt das OpenGl-Fenster. Dies beinhaltet das zuweisen
// des DeviceContext, des Renderingcontexts und des 
//Pixelformatdescriptors. Erhält als Übergabeparameter das Handle
//zum Mainwindow.
HWND CreateOpenGLWindow(HWND &hMain){
	HWND hOpenGL;
	GLuint PixelFormat;
	RECT rcStatus, rcMainwindow;
	HGLRC hGlrc = NULL;
	HDC hDC = NULL;
	std::vector <GLuint> textures;
	
	GetWindowRect(GetDlgItem(hMain, IDC_MAIN_STATUS), &rcStatus);
	int iStatusHeight = rcStatus.bottom - rcStatus.top;
	GetClientRect(hMain, &rcMainwindow);
	int iEditheight = rcMainwindow.bottom - iStatusHeight;
	int iEditwidth = (int)(0.75 * rcMainwindow.right);
	
	
	if (!(hOpenGL = CreateWindowEx(WS_EX_WINDOWEDGE, L"OpenGL", L"OpenGL",
		WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_DLGFRAME,
		0, 0, iEditwidth, iEditheight, hMain, (HMENU)IDC_OPENGL,
		GetModuleHandle(NULL), NULL)))
	{
		KillGLWindow(hOpenGL, hGlrc, hDC);
		MessageBox(NULL, L"OpenGL-Fenster konnte nicht erstellt werden.", L"ERROR", MB_OK);
		return FALSE;
	}

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	//Größe
		1,								//Versionsnummer		
		PFD_DRAW_TO_WINDOW |			//unterstütze Fenster
		PFD_SUPPORT_OPENGL |			//unterstütze OpenGL
		PFD_DOUBLEBUFFER,				//double Puffer
		PFD_TYPE_RGBA,					// RGBA Type
		32,								// 24 Bit Farbtiefe
		0, 0, 0, 0, 0,					// Standardeinstellungen
		0,
		0,
		0,
		0, 0, 0, 0,
		32,								//32 Bit z-Puffer
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if (!(hDC = GetDC(hOpenGL))){
		KillGLWindow(hOpenGL, hGlrc, hDC);
		MessageBox(NULL, L"Es konnte kein Gerätekontext erstellt werden.", L"ERROR", MB_OK);
		return FALSE;
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))){
		MessageBox(NULL, L"Passendes Pixelformat konnte nicht gefunden werden.", L"ERROR", MB_OK);
		return FALSE;
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd)){
		KillGLWindow(hOpenGL, hGlrc,hDC);
		MessageBox(NULL, L"Pixelformat konnte nicht eingestellt werden.", L"ERROR", MB_OK);
		return FALSE;
	}

	if (!(hGlrc = wglCreateContext(hDC))){
		KillGLWindow(hOpenGL, hGlrc, hDC);
		MessageBox(NULL, L"Rendering konnte nicht gestartet werden.", L"ERROR", MB_OK);
		return FALSE;
	}

	if (!wglMakeCurrent(hDC, hGlrc)){
		KillGLWindow(hOpenGL, hGlrc, hDC);
		MessageBox(NULL, L"Rendering Kontext kann nicht erstellt werden.", L"ERROR", MB_OK);
		return FALSE;
	}
	ResizeGLScene(iEditwidth, iEditheight);

	if (!initGL(textures)){
		KillGLWindow(hOpenGL, hGlrc, hDC);
		MessageBox(NULL, L"Initialisierung fehlgeschlagen.", L"ERROR", MB_OK);
		return FALSE;
	}
	map.setTextures(textures);
	map.SetCursorTextur(map.getCursor(), textures);
	map.setPathTexture(textures);
	map.setCreatureTextures(textures);
	map.setDeadTexture(textures);
	SendDlgItemMessage(hMain, IDC_OPENGL, WM_SIZE, 0, 0);
	return hOpenGL;
}
//Gibt die Ressourcen des OpenGl-Fensters wieder frei.
GLvoid KillGLWindow(HWND &hWnd, HGLRC hGlrc,HDC hDC ){
	if (hGlrc){
		if (!wglMakeCurrent(NULL, NULL)){
			MessageBox(NULL, L"Freigabe der Kontexte ist fehlgeschlagen. ", L"ERROR", MB_OK);
		}
		if (!wglDeleteContext(hGlrc)){
			MessageBox(NULL, L"Reigabe des Rendering-Kontextes ist fehlgeschlagen.", L"ERROR", MB_OK);
			
		}hGlrc = NULL;
	}

	if (hDC && !ReleaseDC(hWnd, hDC)){
		MessageBox(NULL, L"Freigabe des Gerätekontexts ist fehlgeschlagen.", L"ERROR", MB_OK);
		hDC = NULL;
	}

	if (hWnd && !DestroyWindow(hWnd)){
		MessageBox(NULL, L"OpenGL-Fenster konnte nicht geschlossen werden.", L"ERROR", MB_OK);
		hWnd = NULL;
	}

	if (map.getTextures().size() ){
		deleteTextures(map);
	}


}
//Erstellt das Kontrollmenü der Simulation.
//Dies enthält die Knöpfe Schritt, Pause und Start.
HWND createButtonGroup(HWND &hWnd){
	HWND hGroupButtons;
	hGroupButtons = CreateWindowEx(WS_EX_WINDOWEDGE,
		L"BUTTON",
		L"Simulationssteuerung",
		WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		10, 400,
		275, 100,
		hWnd,
		(HMENU)IDC_GROUP_BUTTON,
		GetModuleHandle(NULL), NULL);

	if (hGroupButtons == NULL){
		MessageBox(hWnd, L"Kontrollmenü konnte nicht erstellt werden.", L"Error.", MB_OK | MB_ICONERROR);
		return 0;
	}

	createButton(hWnd, IDC_CHECK1, L"Start");
	createButton(hWnd, IDC_CHECK2, L"Pause");
	createButton(hWnd, IDC_CHECK3, L"Schritt");

	return hGroupButtons;
}
//Setzt das Auswahlmenü an die richtige Stelle.
//Dazu wird zuerst die Clientarea abgefragt um
//anschließend die Knöpfe darin zu platzieren.
void setPositionSelectMenu(HWND &hWnd){
	 RECT rc;
	 int breite, hoehe, abstandbreite, abstandhoehe, breiteButton, hoeheButton;
	
	 rc = getClientAreaInGroupBox(hWnd, IDC_GROUP_BUTTON);

	 breite = rc.right - rc.left;
	 hoehe = rc.bottom - rc.top;
	 abstandbreite = (int)(breite *0.25* 0.25);
	 abstandhoehe = (int)(hoehe *0.5);
	 breiteButton = (int)(0.2 * breite);
	 hoeheButton = (int)(0.3 * hoehe);
	 
	 rc = getClientAreaInGroupBox(hWnd, IDC_GROUPSELECT);
	 
	 
	 SetWindowPos(GetDlgItem(hWnd, IDC_STAERKE), NULL, (rc.right) / 2, rc.top + 2 * abstandhoehe, 2 * breiteButton, hoeheButton, NULL);
	 SetWindowPos(GetDlgItem(hWnd, IDC_EDITSTAERKE) , NULL, rc.left + breiteButton, rc.top + 2 * abstandhoehe + 5, breiteButton, hoeheButton, NULL);

	 SetWindowPos(GetDlgItem(hWnd, IDC_GESCHW), NULL, rc.right / 2, rc.top + 3 * abstandhoehe, 2 * breiteButton, hoeheButton, NULL);
	 SetWindowPos(GetDlgItem(hWnd, IDC_EDITGESCHW), NULL, rc.left + breiteButton, rc.top + 3 * abstandhoehe + 5, breiteButton, hoeheButton, NULL);

	 SetWindowPos(GetDlgItem(hWnd, IDC_LEBEN), NULL, rc.right / 2, rc.top + 4 * abstandhoehe, 2 * breiteButton, hoeheButton, NULL);
	 SetWindowPos(GetDlgItem(hWnd, IDC_EDITLEBEN), NULL, rc.left + breiteButton, rc.top + 4 * abstandhoehe + 5, breiteButton, hoeheButton, NULL);

	 SetWindowPos(GetDlgItem(hWnd, IDC_EIGEN), NULL, rc.right / 2, rc.top + 5 * abstandhoehe, 2 * breiteButton, 2 * hoeheButton, NULL);
	 SetWindowPos(GetDlgItem(hWnd, IDC_EDITEIGEN) , NULL, rc.left + breiteButton, rc.top + 5 * abstandhoehe + 5, breiteButton, hoeheButton, NULL);

	 SetWindowPos(GetDlgItem(hWnd, IDC_PLATZIEREN), NULL, rc.right / 3, rc.top + 7 * abstandhoehe - 10, breiteButton + 20, hoeheButton, NULL);
	 SetWindowPos(GetDlgItem(hWnd, IDC_COMBO), NULL, rc.left + breiteButton, rc.top + abstandhoehe, 3 * breiteButton, 5 * hoeheButton, NULL);

}
// Setzt die Kontrollknöpfe in die dafür bestimmte Area.
void setPositionButtongroup(HWND &hWnd){
	RECT rc;
	int breite, hoehe, abstandbreite, abstandhoehe, breiteButton, hoeheButton;
	HWND hButton1;

	rc = getClientAreaInGroupBox(hWnd, IDC_GROUP_BUTTON);

	breite = rc.right - rc.left;
	hoehe = rc.bottom - rc.top;
	abstandbreite = (int)(breite *0.25* 0.25);
	abstandhoehe = (int)(hoehe *0.5);
	breiteButton = (int)(0.2 * breite);
	hoeheButton = (int)(0.3 * hoehe);

	hButton1 = GetDlgItem(hWnd, IDC_CHECK1);
	SetWindowPos(hButton1, NULL, rc.left + abstandbreite, rc.top + abstandhoehe, breiteButton, hoeheButton, NULL);

	hButton1 = GetDlgItem(hWnd, IDC_CHECK2);
	SetWindowPos(hButton1, NULL, rc.left + 2 * abstandbreite + breiteButton, rc.top + abstandhoehe, breiteButton, hoeheButton, NULL);

	hButton1 = GetDlgItem(hWnd, IDC_CHECK3);
	SetWindowPos(hButton1, NULL, rc.left + 3 * abstandbreite + 2 * breiteButton, rc.top + abstandhoehe, breiteButton, hoeheButton, NULL);
}

//Liest die Kreaturnamen aus dem Vektor und schickt sie an die entsprechende Box.
void getCreatureNames(const std::vector <CreatureProps> &creatVector, HWND &hWnd){
	for (unsigned int i = 0; i < creatVector.size(); i++){
		CreatureProps cProp = creatVector[i];
		std::string s = cProp.getName();
		std::wstring stemp = std::wstring(s.begin(), s.end());
		AddString(hWnd, stemp);
	}
}
//Ermittelt die Werte der ausgewählten Kreatur und sendet diese 
// an die jeweilige Box.
void updateSelection(const std::vector <CreatureProps> &creatVector, HWND &hWnd, int pos){
	CreatureProps c = creatVector[pos];
	UINT strength = (c.getStrength());
	UINT speed = c.getSpeed();
	UINT life = c.getLifetime();
	std::string s = c.getProperties();
	std::wstring stemp(stringToWstring(s));
	{
	SetDlgItemInt(hWnd, IDC_GESCHW, speed, FALSE);
	SetDlgItemInt(hWnd, IDC_LEBEN, life, FALSE);
	SetDlgItemInt(hWnd, IDC_STAERKE, strength, FALSE);
	SetDlgItemText(hWnd, IDC_EIGEN, stemp.c_str());
	}
}
//Konvertiert einen übergeben String in wString
std::wstring stringToWstring(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

inline UINT AddString(const HWND &hCombo, const std::wstring &s){
	return static_cast<UINT> (SendMessage(hCombo, CB_ADDSTRING, 0, reinterpret_cast <LPARAM>(s.c_str())));
}

void deleteTextures(Map &map){
	std::vector <GLuint> temp = map.getTextures();
	
	for (unsigned int i = 0; i < map.vCreat.size(); i++){
		glDeleteTextures(temp.size(), &map.getTextures()[0]);
	}
}
