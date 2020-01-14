#include <Windows.h>
#include "IndirectHook.h"

int MessageBoxAHooked(HWND, LPCSTR, LPCSTR, UINT);

int main()
{
	// Initializing IndirectHook
	// Be aware, if you are using the class in the DLL initialize it dynamically
	// IndirectHook* test = new IndirectHook;
	// NOTE: It hooks the function globally, meaning any other module in the same process will call your function but not original
	IndirectHook test;
	//

	// Testing original messagebox
	MessageBoxA(NULL, "Not hooked, first.", "First", MB_ICONINFORMATION);
	//

	// Getting addresses of the original and hooked functions
	PROC originalfunction = GetProcAddress(GetModuleHandleA("user32.dll"), "MessageBoxA");
	PROC hookedfunction = (PROC)&MessageBoxAHooked;
	//

	// Hooking function
	IndirectHookStruct* pointer = test.Hook(originalfunction, hookedfunction);
	//

	// Testing messagebox again and see that it was redirected to the MessageBoxAHooked
	MessageBoxA(NULL, "Not hooked, second.", "Second", MB_ICONINFORMATION);
	//

	// Unhooking hooked function
	test.Unhook(pointer);
	//

	// Testing messagebox again and see that it was restored
	MessageBoxA(NULL, "Not hooked, third.", "Third", MB_ICONINFORMATION);
	//
}

int MessageBoxAHooked(
	HWND   hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT   uType
)
{
	return MessageBoxW(NULL, L"Hooked thing.", L"Hook testing", MB_ICONWARNING);
}