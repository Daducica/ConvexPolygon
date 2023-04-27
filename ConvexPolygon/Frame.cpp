#include "Frame.hpp"

#include "Canvas.hpp"
#include "Logic.hpp"
#include "Resources.hpp"

namespace UI
{
    ButtonStateNotifier::~ButtonStateNotifier () = default;

    const wxPoint DefaultAppPosition {50, 50};
    const wxSize DefaultAppSize {1200, 700};

    const wxPoint CanvasPosition {0, 45};
    const wxSize CanvasSize {1200, 635};

    const wxPoint ClearButtonPosition {400, 10};
    const wxSize ClearButtonSize {150, 25};

    const wxPoint DrawButtonPosition {650, 10};
    const wxSize DrawButtonSize {150, 25};


    BEGIN_EVENT_TABLE (Frame, wxFrame)
        EVT_BUTTON (ClearButton, Frame::OnClearButtonClicked)
        EVT_BUTTON (DrawPolygonButton, Frame::OnDrawPolygonButtonClicked)
        END_EVENT_TABLE ()


    Frame::Frame () :
        wxFrame (nullptr, -1, wxString::FromUTF8 (Resources::DialogTitle),
                 DefaultAppPosition, DefaultAppSize, wxDEFAULT_FRAME_STYLE^ wxRESIZE_BORDER)
    {
    }


    void Frame::CreateUIElements ()
    {
        canvas = new UI::Canvas (this, CanvasPosition, CanvasSize, *this);
        clearCanvasButton = new wxButton (this, ClearButton, wxString::FromUTF8 (Resources::ClearButtonText),
                                          ClearButtonPosition, ClearButtonSize);
        drawPolygonButton = new wxButton (this, DrawPolygonButton, wxString::FromUTF8 (Resources::DrawPolygonButtonText),
                                          DrawButtonPosition, DrawButtonSize);
        clearCanvasButton->Enable (false);
        drawPolygonButton->Enable (false);
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


    void Frame::SetClearCanvasButtonState (bool newState)
    {
        clearCanvasButton->Enable (newState);
    }


    void Frame::SetDrawPolygonButtonState (bool newState)
    {
        drawPolygonButton->Enable (newState);
    }

}