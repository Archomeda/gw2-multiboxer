#pragma once

// Standard
#include <algorithm>
#include <filesystem>
#include <sstream>

// Windows
#define WIN32_LEAN_AND_MEAN
#define NOSYSMETRICS
#define NOMENUS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NODRAWTEXT
#define NOKERNEL
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define ISOLATION_AWARE_ENABLED 1
#include <Windows.h>
#include <shellapi.h>
#include <CommCtrl.h>

extern "C" WINUSERAPI int WINAPI MessageBoxTimeoutW(IN HWND hWnd, IN PCWSTR lpText, IN PCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);

// Third-party
#define wxNO_HTML_LIB
#define wxNO_WEBVIEW_LIB
#define wxNO_MEDIA_LIB
#define wxNO_XRC_LIB
#define wxNO_AUI_LIB
#define wxNO_RIBBON_LIB
#define wxNO_PROPGRID_LIB
#define wxNO_RICHTEXT_LIB
#define wxNO_STC_LIB
#define wxNO_GL_LIB
#define wxNO_QA_LIB
#ifdef NDEBUG
#define wxDEBUG_LEVEL 0
#endif
#include <wx/wxprec.h>
#include <MinHook.h>
#include "minhook.h"
