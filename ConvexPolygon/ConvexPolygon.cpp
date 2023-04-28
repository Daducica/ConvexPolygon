#include "wx/wx.h"
#include <wx/wxprec.h>

#include "Frame.hpp"
#include "UnitTest.hpp"

const bool IsInTestMode = false;

class MyApp : public wxApp
{
    bool OnInit ();
    void CreateUIElements ();

    UI::Frame* frame;
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
    frame = new UI::Frame;
    frame->CreateUIElements ();
}
