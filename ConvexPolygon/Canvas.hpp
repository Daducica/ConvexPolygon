#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <unordered_set>
#include "wx/wx.h"

#include "Geometry.hpp"

namespace UI
{
    class Canvas : public wxPanel
    {
        Geometry::PointSet points;
        Geometry::Polygon polygonPoints;

        void PaintNow ();
        void Render (wxDC& dc);
        void DrawPoints (wxDC& dc);
        void DrawPolygon (wxDC& dc);
    public:
        Canvas (wxFrame* parent, const wxPoint& position, const wxSize& size);

        void PaintEvent (wxPaintEvent& evt);
        void MouseReleased (wxMouseEvent& event);
        void ClearPoints ();
        const Geometry::PointSet& GetCurrentPointSet () const;
        void DrawNewPolygon (const Geometry::Polygon& newPolygonPoints);

        DECLARE_EVENT_TABLE ()
    };
}

#endif