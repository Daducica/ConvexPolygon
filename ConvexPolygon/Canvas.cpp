#include "Canvas.hpp"

#include "ButtonStateNotifier.hpp"
#include "Logic.hpp"

namespace UI
{
    const wxColor PolygonColor (0, 102, 153);
    const wxColor InvalidPolygonColor (255, 204, 153);

    BEGIN_EVENT_TABLE (Canvas, wxPanel)

        EVT_LEFT_UP (Canvas::MouseReleased)

        EVT_PAINT (Canvas::PaintEvent)

        END_EVENT_TABLE ()


    Canvas::Canvas (wxFrame* parent, const wxPoint& position, const wxSize& size, ButtonStateNotifier& buttonStateNotifier) :
        wxPanel (parent, -1, position, size),
        data (*this),
        buttonStateNotifier (buttonStateNotifier)
    {
    }


    void Canvas::PaintEvent (wxPaintEvent& evt)
    {
        wxPaintDC dc (this);
        Render (dc);
    }


    void Canvas::PaintNow ()
    {
        wxClientDC dc (this);
        dc.Clear ();
        Render (dc);
    }


    void Canvas::Render (wxDC& dc)
    {
        DrawPoints (dc);
        DrawPolygon (dc);
    }


    void Canvas::DrawPoints (wxDC& dc)
    {
        const int xSize = 6;
        const int halfXSize = xSize / 2;
        dc.SetPen (wxPen (wxColor (0, 0, 0), 2));
        for (const wxPoint& point : data.GetPoints ()) {
            dc.DrawLine (point.x - halfXSize, point.y - halfXSize, point.x + halfXSize, point.y + halfXSize);
            dc.DrawLine (point.x - halfXSize, point.y + halfXSize, point.x + halfXSize, point.y - halfXSize);
        }
    }


    void Canvas::DrawPolygon (wxDC& dc)
    {

        const Model::UIPolygon polygon = data.GetPolygonPoints ();
        if (polygon.size () < 2)
            return;

        dc.SetPen (wxPen (data.IsPolygonUpToDate () ? PolygonColor : InvalidPolygonColor, 2));

        for (int index = 0; index < polygon.size () - 1; index++) {
            const wxPoint& point1 = polygon[index];
            const wxPoint& point2 = polygon[index + 1];
            dc.DrawLine (point1.x, point1.y, point2.x, point2.y);
        }
    }


    void Canvas::MouseReleased (wxMouseEvent& event)
    {
        data.AddPoint (event.GetPosition ());
    }


    void Canvas::ClearPoints ()
    {
        data.ClearPoints ();
    }


    const Model::UIPointSet& Canvas::GetCurrentPointSet () const
    {
        return data.GetPoints ();
    }


    void Canvas::DrawNewPolygon (const Model::UIPolygon& newPolygonPoints)
    {
        data.UpdatePolygon (newPolygonPoints);
    }


    void Canvas::PointAdded ()
    {
        buttonStateNotifier.SetClearCanvasButtonState (true);
        if (!Geometry::AreAllPointsInOneLine (Logic::ConvertUIPointsToLogicalPoints (data.GetPoints ()))) {
            buttonStateNotifier.SetDrawPolygonButtonState (true);
        }
        PaintNow ();
    }
    
    
    void Canvas::CanvasCleared ()
    {
        buttonStateNotifier.SetClearCanvasButtonState (false);
        buttonStateNotifier.SetDrawPolygonButtonState (false);
        PaintNow ();
    }
    
    
    void Canvas::PolygonUpdated ()
    {
        PaintNow ();
    }
}