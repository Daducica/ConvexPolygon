#include "wx/wx.h"
#include "wx/sizer.h"

#include "Canvas.hpp"
#include "Resources.hpp"
#include "UnitTest.hpp"

const bool IsInTestMode = false;

const wxPoint DefaultAppPosition {50, 50};
const wxSize DefaultAppSize {800, 600};

class MyApp : public wxApp
{
    bool OnInit ();
    void CreateUIElements ();

    wxFrame* frame;
    UI::Canvas* canvas;
};

IMPLEMENT_APP (MyApp)

bool MyApp::OnInit ()
{
    if (IsInTestMode) {
        Test::RunTests ();
        return false;
    }

    CreateUIElements ();
    return true;
}

void MyApp::CreateUIElements ()
{
    frame = new wxFrame (nullptr, -1, wxString::FromUTF8 (Resources::DialogTitle),
                         DefaultAppPosition, DefaultAppSize);
    canvas = new UI::Canvas (frame);
    wxBoxSizer* sizer = new wxBoxSizer (wxHORIZONTAL);
    sizer->Add (canvas, 1, wxEXPAND);
    frame->SetSizer (sizer);
    frame->SetAutoLayout (true);
    frame->Show ();
}
