#include <Windows.h>
#include <windowsx.h>
int last_x, last_y, x, y;
COLORREF lineColor = RGB(0, 0, 0);
bool isDraw = false;
WNDCLASS createWindowClass(HBRUSH bgColor, HCURSOR cursor, HINSTANCE hInstance, HICON icon, LPCWSTR windowName, WNDPROC windowProcedure);
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	WNDCLASS mainWindow = createWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInstance, LoadIcon(NULL, IDI_QUESTION), L"MainWndClass", windowProcedure);

	if (!RegisterClassW(&mainWindow))
		return -1;

	CreateWindow(L"MainWndClass", L"MainWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 800, NULL, NULL, NULL, NULL);

	MSG message = { 0 };
	while (GetMessage(&message, NULL, NULL, NULL)) 
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

WNDCLASS createWindowClass(HBRUSH bgColor, HCURSOR cursor, HINSTANCE hInstance, HICON icon, LPCWSTR windowName, WNDPROC windowProcedure)
{
	WNDCLASS wc = { 0 };

	wc.hCursor = cursor;
	wc.hIcon = icon;
	wc.hInstance = hInstance;
	wc.lpszClassName = windowName;
	wc.hbrBackground = bgColor;
	wc.lpfnWndProc = windowProcedure;

	return wc;
}

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	BOOL fDraw = FALSE;
	HPEN Pen = CreatePen(PS_SOLID, 3, lineColor);

	switch (msg) 
	{
	case WM_CREATE:
		Pen = CreatePen(PS_SOLID, 3, lineColor);
		break;

	case WM_DESTROY:
		DeleteObject(Pen);
		PostQuitMessage(0);
		break;

	case WM_RBUTTONDOWN:
		if (!isDraw) 
		{
			isDraw = true;
			x = last_x = LOWORD(lp);
			y = last_y = HIWORD(lp);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else
		{
			isDraw = false;
			x = LOWORD(lp);
			y = HIWORD(lp);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_KEYDOWN:
		if (wp == 'C' || wp == 'c') 
		{
			lineColor = RGB(rand() % 256, rand() % 256, rand() % 256);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		if ((last_x != x) || (last_y != y))
		{
			HPEN OldPen = (HPEN)SelectObject(hdc, Pen);
			MoveToEx(hdc, last_x, last_y, NULL);
			LineTo(hdc, x, y);
			SelectObject(hdc, OldPen);
		}

		EndPaint(hWnd, &ps);
		break;
	}

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}
