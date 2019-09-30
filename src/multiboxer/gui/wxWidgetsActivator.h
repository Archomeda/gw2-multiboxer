#pragma once
#include "../pch.h"

namespace gui {

    class wxWidgetsActivator {

    public:
        wxWidgetsActivator(HINSTANCE hInstance, HWND hWnd, HANDLE hActCtx) {
            if (!ActivateActCtx(hActCtx, &this->cookie)) {
                this->cookie = NULL;
                return;
            }

            this->app = new wxApp();
            wxApp::SetInstance(this->app);
            wxEntryStart(hInstance);

            if (hWnd) {
                this->parent = new wxWindow();
                this->parent->SetHWND((WXHWND)hWnd);
                this->parent->AdoptAttributesFromHWND();
                wxTopLevelWindows.Append(this->parent);
            }
        }

        virtual ~wxWidgetsActivator() {
            if (!this->cookie) {
                return;
            }

            if (this->parent) {
                wxTopLevelWindows.DeleteObject(this->parent);
                this->parent->SetHWND(NULL);
            }

            wxEntryCleanup();
            wxApp::SetInstance(NULL);
            this->app = NULL;
            this->parent = NULL;

            DeactivateActCtx(0, this->cookie);
            this->cookie = NULL;
        }

        wxWindow* GetParent() {
            return this->parent;
        }


    protected:
        ULONG_PTR cookie = NULL;
        wxApp* app = NULL;
        wxWindow* parent = NULL;

    };

}
