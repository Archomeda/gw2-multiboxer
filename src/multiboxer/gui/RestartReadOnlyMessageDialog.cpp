#include "RestartReadOnlyMessageDialog.h"
#include "wxWidgetsActivator.h"

using namespace std;

#define TIMER_ID 1000

namespace gui {

    int RestartReadOnlyMessageDialog::ShowDialog(HINSTANCE hInstance, HWND hWnd, HANDLE hActCtx) {
        wxWidgetsActivator activator(hInstance, hWnd, hActCtx);

        RestartReadOnlyMessageDialog dialog(activator.GetParent());
        return dialog.ShowModal();
    }


    tuple<wxPanel*, wxStaticText*> CreateTextWithPadding(wxWindow* parent, wxWindowID id, int padding, const wxString& label) {
        auto textPanel = new wxPanel(parent);
        auto hbox = new wxBoxSizer(wxHORIZONTAL);
        textPanel->SetSizer(hbox);

        auto text = new wxStaticText(textPanel, id, label);
        hbox->Add(text, wxSizerFlags().Expand().Border(wxALL, padding));
        return tuple(textPanel, text);
    }

    RestartReadOnlyMessageDialog::RestartReadOnlyMessageDialog(wxWindow* parent) : timer(this, TIMER_ID) {
        this->Create(parent, wxID_ANY, L"GW2 Multiboxer");
        this->SetWindowStyle(wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX);
        this->SetGw2Icon();
        this->Populate();
        this->Center();
    }

    int RestartReadOnlyMessageDialog::ShowModal() {
        this->timer.Start(1000);
        this->timer.Notify();
        return wxDialog::ShowModal();
    }

    void RestartReadOnlyMessageDialog::SetGw2Icon() {
        HINSTANCE hExe = GetModuleHandleW(NULL);
        if (hExe) {
            wchar_t path[MAX_PATH];
            GetModuleFileNameW(hExe, path, MAX_PATH);
            HICON hIcon = ExtractIconW(hExe, path, 0);

            wxIcon icon;
            icon.CreateFromHICON(hIcon);
            this->SetIcon(icon);
        }
    }

    void RestartReadOnlyMessageDialog::Populate() {
        auto panel = new wxPanel(this);
        auto vbox = new wxBoxSizer(wxVERTICAL);
        panel->SetSizer(vbox);

        auto hbox = new wxBoxSizer(wxHORIZONTAL);
        vbox->Add(hbox, wxSizerFlags().Expand());

        auto [textPanel, text] = CreateTextWithPadding(panel, wxID_STATIC, 15, L"You're starting Guild Wars 2 with a custom Local.dat filename.\r\n"
            L"Do you want to enable -shareArchive for multiboxing support? "
            L"This will restart Guild Wars 2 in read-only mode.");
        textPanel->SetBackgroundColour("white");
        text->SetBackgroundColour("white");
        text->Wrap(400);
        hbox->Add(textPanel, wxSizerFlags().Top().Expand());

        auto buttons = new wxStdDialogButtonSizer();
        auto yes = new wxButton(panel, wxID_YES);
        auto no = new wxButton(panel, wxID_NO);
        no->SetDefault();
        buttons->SetAffirmativeButton(yes);
        buttons->SetNegativeButton(no);
        buttons->Realize();
        vbox->Add(buttons, wxSizerFlags().Expand().DoubleBorder(wxALL));
        this->textNo = no->GetLabelText();

        panel->Fit();
        this->Fit();
        this->SetEscapeId(wxID_NO);
    }

    void RestartReadOnlyMessageDialog::OnYes(wxCommandEvent& event) {
        this->EndModal(wxID_YES);
    }

    void RestartReadOnlyMessageDialog::OnNo(wxCommandEvent& event) {
        this->EndModal(wxID_NO);
    }

    void RestartReadOnlyMessageDialog::OnTick(wxTimerEvent& event) {
        auto button = (wxButton*)this->FindWindowById(wxID_NO);
        button->SetLabelText(wxString::Format(this->textNo + L" (%i)", this->secondsLeft));

        --this->secondsLeft;
        if (this->secondsLeft < 0)
            this->EndModal(wxID_NO);
    }

    BEGIN_EVENT_TABLE(RestartReadOnlyMessageDialog, wxDialog)
        EVT_BUTTON(wxID_YES, RestartReadOnlyMessageDialog::OnYes)
        EVT_BUTTON(wxID_NO, RestartReadOnlyMessageDialog::OnNo)
        EVT_TIMER(TIMER_ID, RestartReadOnlyMessageDialog::OnTick)
    END_EVENT_TABLE()

}
