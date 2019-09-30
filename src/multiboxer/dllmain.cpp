#include "pch.h"
#include "exports/exports.h"
#include "gui/RestartReadOnlyMessageDialog.h"
#include "utils/debug.h"
#include "utils/string.h"

using namespace std;
using namespace std::filesystem;
using namespace gui;
using namespace utils;


HMODULE hDllModule;
HANDLE hActCtx;
path localDatPath;
bool shareArchive = false;

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
        if (!lpFileName || localDatPath.empty()) {
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
        if (!lpFileName || localDatPath.empty()) {
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
        if (!lpName || !shareArchive) {
            return SystemCreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
        }

        string name(lpName);
        if (name.starts_with("AN-Mutex-Window-") || name.starts_with("AN-Mutex-Install-")) {
            name.append("-" + to_string(GetCurrentProcessId()));
        }
        return SystemCreateMutexA(lpMutexAttributes, bInitialOwner, name.c_str());
    }

    HANDLE WINAPI HookCreateMutexW(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCWSTR lpName) {
        if (!lpName || !shareArchive) {
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
    if (!InitializeExports()) {
        MessageBoxW(NULL, L"Failed to find system version.dll. Aborting.", L"GW2 Multiboxer error", MB_ICONERROR | MB_OK);
        return false;
    }
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

        for (int i = 0; i < argumentCount; i++) {
            wstring argument(ToLower(arguments[i]));
            if ((argument == L"-ldat" || argument == L"/ldat") && i + 1 < argumentCount) {
                localDatPath = wstring(arguments[i + 1]);
            }
            if (argument == L"-sharearchive" || argument == L"/sharearchive") {
                shareArchive = true;
            }
        }

        if (!localDatPath.empty() && !shareArchive) {
            int result = RestartReadOnlyMessageDialog::ShowDialog(hDllModule, NULL, hActCtx);
            if (result == wxID_YES) {
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
    ShutdownExports();
}


bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hDllModule = hModule;
        GetCurrentActCtx(&hActCtx);
        LaunchDebugger();
        return Initialize();
    case DLL_PROCESS_DETACH:
        Shutdown();
        if (hActCtx)
            ReleaseActCtx(hActCtx);
        break;
    }
    return true;
}
