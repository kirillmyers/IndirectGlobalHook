#ifdef _WIN64
static const int size = 12;
#else
static const int size = 7;
#endif

struct IndirectHookStruct
{
	PROC function;
	BYTE originalbytes[size];
};

class IndirectHook
{
private:
#ifdef _WIN64
	const int addresspointer = 2;
	BYTE HookArray[size] = { 0x48,0xB8,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0xFF,0xE0 };
#else
	const int addresspointer = 1;
	BYTE HookArray[size] = { 0xB8,0x90,0x90,0x90,0x90,0xFF,0xE0 };
#endif
	DWORD oldprotection = NULL;

public:
	// constructor
	IndirectHook() {}
	//

	// Hook function (original - address of the original function which we want to hook)
	// hooked - address of the function which we will redirect to
	// returns the pointer of the restore function structure which may be used in unhook function
	IndirectHookStruct* Hook(PROC original, PROC hooked)
	{
		// restore function creation and filling
		IndirectHookStruct* restore = new IndirectHookStruct;
		restore->function = original;
		memcpy(restore->originalbytes, original, size);
		//

		// copy hooked function address to the HookArray
		memcpy((HookArray + addresspointer), &hooked, sizeof(PROC));
		//

		// Hooking original function
		VirtualProtect(original, size, PAGE_EXECUTE_READWRITE, &oldprotection);
		memcpy(original, HookArray, size);
		VirtualProtect(original, size, oldprotection, &oldprotection);
		//

		return restore;
	}

	void Unhook(IndirectHookStruct* pointer)
	{
		VirtualProtect(pointer->function, size, PAGE_EXECUTE_READWRITE, &oldprotection);
		memcpy(pointer->function, pointer->originalbytes, size);
		VirtualProtect(pointer->function, size, oldprotection, &oldprotection);
	}
};