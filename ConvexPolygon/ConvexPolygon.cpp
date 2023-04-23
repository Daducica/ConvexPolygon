#include "wx/wx.h"
#include "wx/sizer.h"

#include "Canvas.hpp"
#include "Resources.hpp"

const wxPoint DefaultAppPosition {50, 50};
const wxSize DefaultAppSize {800, 600};

class MyApp : public wxApp
{
    bool OnInit ();

    wxFrame* frame;
    UI::Canvas* canvas;
};

IMPLEMENT_APP (MyApp)

bool MyApp::OnInit ()
{
    frame = new wxFrame (nullptr, -1, wxString::FromUTF8 (Resources::DialogTitle),
                         DefaultAppPosition, DefaultAppSize);
    canvas = new UI::Canvas (frame);
    wxBoxSizer* sizer = new wxBoxSizer (wxHORIZONTAL);
    sizer->Add (canvas, 1, wxEXPAND);
    frame->SetSizer (sizer);
    frame->SetAutoLayout (true);
    frame->Show ();
    return true;
}

