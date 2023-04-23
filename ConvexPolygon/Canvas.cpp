#include "Canvas.hpp"

namespace UI
{
    BEGIN_EVENT_TABLE (Canvas, wxPanel)

        EVT_LEFT_UP (Canvas::MouseReleased)

        EVT_PAINT (Canvas::PaintEvent)

     END_EVENT_TABLE ()


    Canvas::Canvas (wxFrame* parent) :
        wxPanel (parent)
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
        Render (dc);
    }


    void Canvas::Render (wxDC& dc)
    {
        const int xSize = 6;
        const int halfXSize = xSize / 2;
        for (const Geometry::Point& point : points) {
            dc.SetPen (wxPen (wxColor (0, 0, 0), 2));
            dc.DrawLine (point.x - halfXSize, point.y - halfXSize, point.x + halfXSize, point.y + halfXSize);
            dc.DrawLine (point.x - halfXSize, point.y + halfXSize, point.x + halfXSize, point.y - halfXSize);
        }
    }


    void Canvas::MouseReleased (wxMouseEvent& event)
    {
        const wxPoint newWxPoint = event.GetPosition ();
        const Geometry::Point newPoint {newWxPoint.x, newWxPoint.y};
        points.insert (newPoint);
        PaintNow ();
    }

}