#include "wx/wx.h"
#include <wx/wxprec.h>

#include "Canvas.hpp"
#include "Logic.hpp"
#include "Resources.hpp"
#include "UnitTest.hpp"

const bool IsInTestMode = true;

const wxPoint DefaultAppPosition {50, 50};
const wxSize DefaultAppSize {1200, 700};

const wxPoint CanvasPosition {0, 45};
const wxSize CanvasSize {1200, 635};

const wxPoint ClearButtonPosition {400, 10};
const wxSize ClearButtonSize {150, 25};

const wxPoint DrawButtonPosition {650, 10};
const wxSize DrawButtonSize {150, 25};

class Frame;

class MyApp : public wxApp
{
    bool OnInit ();
    void CreateUIElements ();

    Frame* frame;
};


class Frame : public wxFrame
{
public:
    enum
    {
        ClearButton = wxID_HIGHEST + 1,
        DrawPolygonButton = wxID_HIGHEST + 2
    };

    Frame () :
        wxFrame (nullptr, -1, wxString::FromUTF8 (Resources::DialogTitle),
                 DefaultAppPosition, DefaultAppSize, wxDEFAULT_FRAME_STYLE^ wxRESIZE_BORDER)
    { }

    void CreateUIElements ();
    void OnClearButtonClicked (wxCommandEvent& event);
    void OnDrawPolygonButtonClicked (wxCommandEvent& event);

    UI::Canvas* canvas;
    wxButton* clearCanvasButton;
    wxButton* drawPolygonButton;

    DECLARE_EVENT_TABLE ()
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
    frame = new Frame;
    frame->CreateUIElements ();
}


BEGIN_EVENT_TABLE (Frame, wxFrame)
EVT_BUTTON (ClearButton, Frame::OnClearButtonClicked)
EVT_BUTTON (DrawPolygonButton, Frame::OnDrawPolygonButtonClicked)
END_EVENT_TABLE ()


void Frame::CreateUIElements ()
{
    canvas = new UI::Canvas (this, CanvasPosition, CanvasSize);
    clearCanvasButton = new wxButton (this, ClearButton, wxString::FromUTF8 (Resources::ClearButtonText),
                                      ClearButtonPosition, ClearButtonSize);
    drawPolygonButton = new wxButton (this, DrawPolygonButton, wxString::FromUTF8 (Resources::DrawPolygonButtonText),
                                      DrawButtonPosition, DrawButtonSize);
    SetAutoLayout (true);
    Show ();
}


void Frame::OnClearButtonClicked (wxCommandEvent& event)
{
    canvas->ClearPoints ();
}


void Frame::OnDrawPolygonButtonClicked (wxCommandEvent& event)
{
    if (canvas->GetCurrentPointSet ().size () < 3)
        return;

    Geometry::Polygon polygonPoints = Logic::CalculateBoundingPolygon (canvas->GetCurrentPointSet ());
    canvas->DrawNewPolygon (polygonPoints);
}
