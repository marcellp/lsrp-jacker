#include "lsrp-jacker.hpp"

SAMPFUNCS *SF = new SAMPFUNCS();

void pprintf(const char *format, ...)
{
	char buf[128];
	va_list args;

	va_start(args, format);

	sprintf_s(buf, 128, PROGRAM_NAME": ");
	vsnprintf(buf + strlen(buf), 128 - strlen(buf), format, args);

	va_end(args);

	SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF), buf);
}

void CALLBACK cmd_help(std::string param)
{
	char buf[512];

	sprintf_s(buf, 512,
		PROGRAM_NAME " " PROGRAM_VERSION "\n"
		"Copyright (c) 2016 " PROGRAM_AUTHORS "\n"
		"\n"
		"This is a utility for stealing vehicles on LS-RP. It automatically unscrambles\n"
		"the words you need to start the vehicle and it allows you to enter any vehicle you\n"
		"want without having to break into it using a weapon.\n"
		"\n"
		"Use /j-lock to open a locked vehicle. Use /j-noeject to bypass the lock\n"
		"scriptwise. This allows you to enter the vehicle without being ejected.\n"
		"Do not forget to turn this off once you're inside the vehicle.\n"
		"lsrp-jacker will automatically unscramble the words when you\n"
		"attempt to start the engine of a stolen car.\n"
		"\n"
		"Report bugs to: " PROGRAM_BUGREPORT_EMAIL "\n"
		"" PROGRAM_NAME " home page: <" PROGRAM_WEBSITE">\n"
		);

	SF->getSAMP()->getDialog()->ShowDialog(1, 0, "lsrp-jacker", buf, "Hide", "");
	return;
}

void CALLBACK mainloop()
{
	static bool init = false;
	if (!init)
	{
		if (GAME == nullptr)
			return;
		if (GAME->GetSystemState() != eSystemState::GS_PLAYING_GAME)
			return;
		if (!SF->getSAMP()->IsInitialized())
			return;
		
		pprintf("lsrp-jacker " PROGRAM_VERSION " has been loaded, use /lsrp-jacker for general help.");
		SF->getSAMP()->registerChatCommand("lsrp-jacker", cmd_help);
		SF->getSAMP()->registerChatCommand("j-lock", cmd_open_vehicle);
		SF->getSAMP()->registerChatCommand("j-noeject", cmd_noeject);
		SF->getRakNet()->registerRakNetCallback(RAKHOOK_TYPE_INCOMING_RPC, hook_do_not_remove);
		SF->getRakNet()->registerRakNetCallback(RAKHOOK_TYPE_INCOMING_RPC, hook_get_tdstring);

		init = true;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
	switch (dwReasonForCall)
	{
		case DLL_PROCESS_ATTACH:
			SF->initPlugin(mainloop, hModule);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
