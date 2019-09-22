#include "pch.h"
#include "minhook.h"
#include "utils/debug.h"
#include "utils/string.h"

using namespace std;
using namespace std::filesystem;
using namespace multiboxer;


namespace multiboxer {

#pragma region Required version.dll passthrough exports
    struct VersionDllExports {
        HMODULE Module;
        FARPROC GetFileVersionInfoA;
        FARPROC GetFileVersionInfoByHandle;
        FARPROC GetFileVersionInfoExA;
        FARPROC GetFileVersionInfoExW;
        FARPROC GetFileVersionInfoSizeA;
        FARPROC GetFileVersionInfoSizeExA;
        FARPROC GetFileVersionInfoSizeExW;
        FARPROC GetFileVersionInfoSizeW;
        FARPROC GetFileVersionInfoW;
        FARPROC VerFindFileA;
        FARPROC VerFindFileW;
        FARPROC VerInstallFileA;
        FARPROC VerInstallFileW;
        FARPROC VerLanguageNameA;
        FARPROC VerLanguageNameW;
        FARPROC VerQueryValueA;
        FARPROC VerQueryValueW;
    };
    VersionDllExports versionDllExports;

    void WINAPI GetFileVersionInfoA() {
        void(*jmp)() = (void(*)())versionDllExports.GetFileVersionInfoA;
        (*jmp)();
    }

    void WINAPI GetFileVersionInfoByHandle() {
        void(*jmp)() = (void(*)())versionDllExports.GetFileVersionInfoByHandle;
        (*jmp)();
    }

    void WINAPI GetFileVersionInfoExA() {
        void(*jmp)() = (void(*)())versionDllExports.GetFileVersionInfoExA;
        (*jmp)();
    }

    void WINAPI GetFileVersionInfoExW() {
        void(*jmp)() = (void(*)())versionDllExports.GetFileVersionInfoExW;
        (*jmp)();
    }

    void WINAPI GetFileVersionInfoSizeA() {
        void(*jmp)() = (void(*)())versionDllExports.GetFileVersionInfoSizeA;
        (*jmp)();
    }

    void WINAPI GetFileVersionInfoSizeExA() {
        void(*jmp)() = (void(*)())versionDllExports.GetFileVersionInfoSizeExA;
        (*jmp)();
    }

    void WINAPI GetFileVersionInfoSizeExW() {
        void(*jmp)() = (void(*)())versionDllExports.GetFileVersionInfoSizeExW;
        (*jmp)();
    }

    void WINAPI GetFileVersionInfoSizeW() {
        void(*jmp)() = (void(*)())versionDllExports.GetFileVersionInfoSizeW;
        (*jmp)();
    }

    void WINAPI GetFileVersionInfoW() {
        void(*jmp)() = (void(*)())versionDllExports.GetFileVersionInfoW;
        (*jmp)();
    }

    void WINAPI VerFindFileA() {
        void(*jmp)() = (void(*)())versionDllExports.VerFindFileA;
        (*jmp)();
    }

    void WINAPI VerFindFileW() {
        void(*jmp)() = (void(*)())versionDllExports.VerFindFileW;
        (*jmp)();
    }

    void WINAPI VerInstallFileA() {
        void(*jmp)() = (void(*)())versionDllExports.VerInstallFileA;
        (*jmp)();
    }

    void WINAPI VerInstallFileW() {
        void(*jmp)() = (void(*)())versionDllExports.VerInstallFileW;
        (*jmp)();
    }

    void WINAPI VerLanguageNameA() {
        void(*jmp)() = (void(*)())versionDllExports.VerLanguageNameA;
        (*jmp)();
    }

    void WINAPI VerLanguageNameW() {
        void(*jmp)() = (void(*)())versionDllExports.VerLanguageNameW;
        (*jmp)();
    }

    void WINAPI VerQueryValueA() {
        void(*jmp)() = (void(*)())versionDllExports.VerQueryValueA;
        (*jmp)();
    }

    void WINAPI VerQueryValueW() {
        void(*jmp)() = (void(*)())versionDllExports.VerQueryValueW;
        (*jmp)();
    }
#pragma endregion

    path localDatPath;

    namespace hooks {
        typedef HANDLE(WINAPI CreateFileA_t)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
        typedef HANDLE(WINAPI CreateFileW_t)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
        typedef HANDLE(WINAPI CreateMutexA_t)(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitiaOwner, LPCSTR lpName);
        typedef HANDLE(WINAPI CreateMutexW_t)(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitiaOwner, LPCWSTR lpName);

        CreateFileA_t* SystemCreateFileA;
        CreateFileW_t* SystemCreateFileW;
        CreateMutexA_t* SystemCreateMutexA;
        CreateMutexW_t* SystemCreateMutexW;

        HANDLE WINAPI HookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
            if (!lpFileName) {
                return SystemCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
            }

            string fileName(lpFileName);
            path filePath(fileName);
            if (!localDatPath.empty() && filePath.filename() == "Local.dat") {
                if (localDatPath.is_relative()) {
                    filePath = filePath.remove_filename() / localDatPath;
                }
                else {
                    filePath = localDatPath;
                }
                fileName = filePath.string();
            }
            return SystemCreateFileA(fileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }

        HANDLE WINAPI HookCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
            if (!lpFileName) {
                return SystemCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
            }

            path filePath(lpFileName);
            if (!localDatPath.empty() && filePath.filename() == "Local.dat") {
                if (localDatPath.is_relative()) {
                    filePath = filePath.remove_filename() / localDatPath;
                }
                else {
                    filePath = localDatPath;
                }
                lpFileName = filePath.c_str();
            }
            return SystemCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }

        HANDLE WINAPI HookCreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName) {
            if (!lpName) {
                return SystemCreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
            }

            string name(lpName);
            if (name.starts_with("AN-Mutex-Window-") || name.starts_with("AN-Mutex-Install-")) {
                name.append("-" + to_string(GetCurrentProcessId()));
            }
            return SystemCreateMutexA(lpMutexAttributes, bInitialOwner, name.c_str());
        }

        HANDLE WINAPI HookCreateMutexW(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCWSTR lpName) {
            if (!lpName) {
                return SystemCreateMutexW(lpMutexAttributes, bInitialOwner, lpName);
            }

            wstring name(lpName);
            if (name.starts_with(L"AN-Mutex-Window-") || name.starts_with(L"AN-Mutex-Install-")) {
                name.append(L"-" + to_wstring(GetCurrentProcessId()));
            }
            return SystemCreateMutexW(lpMutexAttributes, bInitialOwner, name.c_str());
        }
    }


    bool InitializePassthrough() {
        // It's unadvised to call LoadLibrary in DllMain, because it can cause a deadlock.
        // But we're just a proxy to the system version.dll, so we can ignore that.
        wchar_t systemDir[MAX_PATH];
        GetSystemDirectory(systemDir, MAX_PATH);
        path systemVersionDll = path(systemDir) / "version";
        HMODULE hModule = LoadLibrary(systemVersionDll.c_str());
        if (!hModule) {
            MessageBoxW(NULL, L"Failed to find system version.dll. Aborting.", L"GW2 Multiboxer error", MB_ICONERROR | MB_OK);
            return false;
        }

        versionDllExports.Module = hModule;
        versionDllExports.GetFileVersionInfoA = GetProcAddress(hModule, "GetFileVersionInfoA");
        versionDllExports.GetFileVersionInfoByHandle = GetProcAddress(hModule, "GetFileVersionInfoByHandle");
        versionDllExports.GetFileVersionInfoExA = GetProcAddress(hModule, "GetFileVersionInfoExA");
        versionDllExports.GetFileVersionInfoExW = GetProcAddress(hModule, "GetFileVersionInfoExW");
        versionDllExports.GetFileVersionInfoSizeA = GetProcAddress(hModule, "GetFileVersionInfoSizeA");
        versionDllExports.GetFileVersionInfoSizeExA = GetProcAddress(hModule, "GetFileVersionInfoSizeExA");
        versionDllExports.GetFileVersionInfoSizeExW = GetProcAddress(hModule, "GetFileVersionInfoSizeExW");
        versionDllExports.GetFileVersionInfoSizeW = GetProcAddress(hModule, "GetFileVersionInfoSizeW");
        versionDllExports.GetFileVersionInfoW = GetProcAddress(hModule, "GetFileVersionInfoW");
        versionDllExports.VerFindFileA = GetProcAddress(hModule, "VerFindFileA");
        versionDllExports.VerFindFileW = GetProcAddress(hModule, "VerFindFileW");
        versionDllExports.VerInstallFileA = GetProcAddress(hModule, "VerInstallFileA");
        versionDllExports.VerInstallFileW = GetProcAddress(hModule, "GetFVerInstallFileWileVersionA");
        versionDllExports.VerLanguageNameA = GetProcAddress(hModule, "VerLanguageNameA");
        versionDllExports.VerLanguageNameW = GetProcAddress(hModule, "VerLanguageNameW");
        versionDllExports.VerQueryValueA = GetProcAddress(hModule, "VerQueryValueA");
        versionDllExports.VerQueryValueW = GetProcAddress(hModule, "VerQueryValueW");

        return true;
    }

    bool InitializeMinHook() {
        if (MH_Initialize() != MH_OK) {
            MessageBoxW(NULL, L"Failed to initialize MinHook. Continuing without multiboxer hooks.", L"GW2 Multiboxer warning", MB_ICONWARNING | MB_OK);
            return true;
        }

        MH_STATUS mhStatus = MH_OK;
        mhStatus = !mhStatus ? MH_CreateHookEx(&CreateFileA, hooks::HookCreateFileA, &hooks::SystemCreateFileA) : mhStatus;
        mhStatus = !mhStatus ? MH_CreateHookEx(&CreateFileW, hooks::HookCreateFileW, &hooks::SystemCreateFileW) : mhStatus;
        mhStatus = !mhStatus ? MH_CreateHookEx(&CreateMutexA, hooks::HookCreateMutexA, &hooks::SystemCreateMutexA) : mhStatus;
        mhStatus = !mhStatus ? MH_CreateHookEx(&CreateMutexW, hooks::HookCreateMutexW, &hooks::SystemCreateMutexW) : mhStatus;
        mhStatus = !mhStatus ? MH_QueueEnableHook(&CreateFileA) : mhStatus;
        mhStatus = !mhStatus ? MH_QueueEnableHook(&CreateFileW) : mhStatus;
        mhStatus = !mhStatus ? MH_QueueEnableHook(&CreateMutexA) : mhStatus;
        mhStatus = !mhStatus ? MH_QueueEnableHook(&CreateMutexW) : mhStatus;
        mhStatus = !mhStatus ? MH_ApplyQueued() : mhStatus;

        if (mhStatus) {
            MessageBoxW(NULL, L"Failed to hook to required WinAPI functions. Aborting.", L"GW2 Multiboxer error", MB_ICONERROR | MB_OK);
            return false;
        }

        return true;
    }

    bool Initialize() {
        if (InitializePassthrough() && InitializeMinHook()) {
            int argumentCount = 0;
            LPWSTR commandLine = GetCommandLineW();
            LPWSTR* arguments = CommandLineToArgvW(commandLine, &argumentCount);
            bool hasLocalDatPath = false;
            bool hasShareArchive = false;

            for (int i = 0; i < argumentCount; i++) {
                wstring argument(ToLower(arguments[i]));
                if ((argument == L"-ldat" || argument == L"/ldat") && i + 1 < argumentCount) {
                    hasLocalDatPath = true;
                    localDatPath = wstring(arguments[i + 1]);
                }
                if (argument == L"-sharearchive" || argument == L"/sharearchive") {
                    hasShareArchive = true;
                }
            }

            if (hasLocalDatPath && !hasShareArchive) {
                int result = MessageBoxTimeoutW(NULL,
                    L"You're starting Guild Wars 2 with a custom Local.dat filename.\r\n"
                    L"Do you want to enable -shareArchive for multiboxing support? "
                    L"This will put restart Guild Wars 2 in read-only mode.\r\n\r\n"
                    L"This popup will close and assume 'No' within 5 seconds.",
                    L"GW2 Multiboxer",
                    MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2, 0, 5000);
                if (result == IDYES) {
                    size_t programNameLength = wcslen(arguments[0]);
                    if (commandLine[0] == L'\"') {
                        programNameLength += 2;
                    }
                    wstring parameters(wstring(commandLine).substr(programNameLength + 1));
                    parameters.append(L" -shareArchive");
                    LPWSTR lpParameters = new wchar_t[parameters.size() + 1];
                    copy(parameters.begin(), parameters.end(), lpParameters);
                    lpParameters[parameters.size()] = 0;

                    STARTUPINFOW info = { sizeof(info) };
                    PROCESS_INFORMATION processInfo;
                    if (!CreateProcessW(arguments[0], lpParameters, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo)) {
                        DWORD error = GetLastError();
                        MessageBoxW(NULL, (wstring(L"Error ") + to_wstring(error) + L" while restarting GW2 client.").c_str(), L"GW2 Multiboxer error", MB_ICONERROR | MB_OK);
                    }
                    delete[] lpParameters;
                    CloseHandle(processInfo.hProcess);
                    CloseHandle(processInfo.hThread);
                    ExitProcess(0);
                }
            }

            LocalFree(arguments);
            return true;
        }
        return false;
    }

    void ShutdownPassthrough() {
        if (versionDllExports.Module) {
            FreeLibrary(versionDllExports.Module);
            versionDllExports.Module = nullptr;
        }
    }

    void ShutdownMinHook() {
        MH_QueueDisableHook(&CreateFileA);
        MH_QueueDisableHook(&CreateFileW);
        MH_QueueDisableHook(&CreateMutexA);
        MH_QueueDisableHook(&CreateMutexW);
        MH_ApplyQueued();
        MH_RemoveHook(&CreateFileA);
        MH_RemoveHook(&CreateFileW);
        MH_RemoveHook(&CreateMutexA);
        MH_RemoveHook(&CreateMutexW);
        MH_Uninitialize();
    }

    void Shutdown() {
        ShutdownMinHook();
        ShutdownPassthrough();
    }
}


bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LaunchDebugger();
        return multiboxer::Initialize();
    case DLL_PROCESS_DETACH:
        multiboxer::Shutdown();
        break;
    }
    return true;
}
