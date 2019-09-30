#pragma once
#include "../pch.h"

namespace gui {

    class RestartReadOnlyMessageDialog : public wxDialog
    {
    public:
        static int ShowDialog(HINSTANCE hInstance, HWND hWnd, HANDLE hActCtx);

        RestartReadOnlyMessageDialog(wxWindow* parent);

    private:
        wxString textNo;
        int secondsLeft = 5;

        virtual int ShowModal() override;

        void SetGw2Icon();
        void Populate();

        DECLARE_EVENT_TABLE()
        void OnYes(wxCommandEvent& event);
        void OnNo(wxCommandEvent& event);
        void OnTick(wxTimerEvent& event);

        wxTimer timer;
    };

}
