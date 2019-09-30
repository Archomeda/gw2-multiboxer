#include "../pch.h"

using namespace std::filesystem;

extern "C" UINT_PTR mExports[17] = { 0 };
LPCSTR mImportNames[] = {
    "GetFileVersionInfoA",
    "GetFileVersionInfoByHandle",
    "GetFileVersionInfoExA",
    "GetFileVersionInfoExW",
    "GetFileVersionInfoSizeA",
    "GetFileVersionInfoSizeExA",
    "GetFileVersionInfoSizeExW",
    "GetFileVersionInfoSizeW",
    "GetFileVersionInfoW",
    "VerFindFileA",
    "VerFindFileW",
    "VerInstallFileA",
    "VerInstallFileW",
    "VerLanguageNameA",
    "VerLanguageNameW",
    "VerQueryValueA",
    "VerQueryValueW"
};

HMODULE hModule;

bool InitializeExports() {
    wchar_t systemDir[MAX_PATH];
    GetSystemDirectory(systemDir, MAX_PATH);
    path systemVersionDll = path(systemDir) / "version";
    hModule = LoadLibrary(systemVersionDll.c_str());
    if (!hModule) {
        return NULL;
    }

    for (int i = 0; i < 17; i++)
        mExports[i] = (UINT_PTR)GetProcAddress(hModule, mImportNames[i]);

    return hModule;
}

void ShutdownExports() {
    if (hModule) {
        FreeLibrary(hModule);
        hModule = NULL;
    }
}

extern "C" {
    void WrapperGetFileVersionInfoA();
    void WrapperGetFileVersionInfoByHandle();
    void WrapperGetFileVersionInfoExA();
    void WrapperGetFileVersionInfoExW();
    void WrapperGetFileVersionInfoSizeA();
    void WrapperGetFileVersionInfoSizeExA();
    void WrapperGetFileVersionInfoSizeExW();
    void WrapperGetFileVersionInfoSizeW();
    void WrapperGetFileVersionInfoW();
    void WrapperVerFindFileA();
    void WrapperVerFindFileW();
    void WrapperVerInstallFileA();
    void WrapperVerInstallFileW();
    void WrapperVerLanguageNameA();
    void WrapperVerLanguageNameW();
    void WrapperVerQueryValueA();
    void WrapperVerQueryValueW();
}
