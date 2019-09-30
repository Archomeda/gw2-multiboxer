#include "debug.h"

using namespace std;

namespace utils {

    bool LaunchDebugger() {
#ifdef _DEBUG
        if (IsDebuggerPresent()) {
            return true;
        }

        // https://stackoverflow.com/a/20387632
        // Get System directory, typically c:\windows\system32
        wstring systemDir(MAX_PATH + 1, '\0');
        UINT nChars = GetSystemDirectoryW(&systemDir[0], static_cast<UINT>(systemDir.length()));
        if (nChars == 0) return false; // failed to get system directory
        systemDir.resize(nChars);

        // Get process ID and create the command line
        DWORD pid = GetCurrentProcessId();
        wostringstream s;
        s << systemDir << L"\\vsjitdebugger.exe -p " << pid;
        wstring cmdLine = s.str();

        // Start debugger process
        STARTUPINFOW si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);

        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessW(NULL, &cmdLine[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) return false;

        // Wait for the debugger to attach
        while (true) {
            DWORD exitCode;
            if (GetExitCodeProcess(pi.hProcess, &exitCode) && exitCode != STILL_ACTIVE) {
                break;
            }
            Sleep(100);
        };

        // Close debugger process handles to eliminate resource leak
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

#endif
        return true;
    }

}
