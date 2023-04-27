#include "Canvas.hpp"

namespace UI
{
    BEGIN_EVENT_TABLE (Canvas, wxPanel)

        EVT_LEFT_UP (Canvas::MouseReleased)

        EVT_PAINT (Canvas::PaintEvent)

        END_EVENT_TABLE ()


        Canvas::Canvas (wxFrame* parent, const wxPoint& position, const wxSize& size) :
        wxPanel (parent, -1, position, size)
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
        for (const Geometry::Point& point : points) {
            dc.DrawLine (point.x - halfXSize, point.y - halfXSize, point.x + halfXSize, point.y + halfXSize);
            dc.DrawLine (point.x - halfXSize, point.y + halfXSize, point.x + halfXSize, point.y - halfXSize);
        }
    }


    void Canvas::DrawPolygon (wxDC& dc)
    {
        if (polygonPoints.size () < 2)
            return;

        dc.SetPen (wxPen (wxColor (100, 100, 0), 2));
        for (int index = 0; index < polygonPoints.size () - 1; index++) {
            const Geometry::Point& point1 = polygonPoints[index];
            const Geometry::Point& point2 = polygonPoints[index + 1];
            dc.DrawLine (point1.x, point1.y, point2.x, point2.y);
        }
    }


    void Canvas::MouseReleased (wxMouseEvent& event)
    {
        const wxPoint newWxPoint = event.GetPosition ();
        const Geometry::Point newPoint {newWxPoint.x, newWxPoint.y};
        points.insert (newPoint);
        PaintNow ();
    }


    void Canvas::ClearPoints ()
    {
        points.clear ();
        polygonPoints.clear ();
        PaintNow ();
    }


    const Geometry::PointSet& Canvas::GetCurrentPointSet () const
    {
        return points;
    }


    void Canvas::DrawNewPolygon (const Geometry::Polygon& newPolygonPoints)
    {
        assert (newPolygonPoints.size () > 2);
        polygonPoints = newPolygonPoints;
        PaintNow ();
    }

}