#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <unordered_set>
#include "wx/wx.h"

#include "Geometry.hpp"

namespace UI
{
    class Canvas : public wxPanel
    {
        std::unordered_set<Geometry::Point, Geometry::Point::HashFunction> points;

        void PaintNow ();
        void Render (wxDC& dc);
    public:
        Canvas (wxFrame* parent);

        void PaintEvent (wxPaintEvent& evt);
        void MouseReleased (wxMouseEvent& event);

        DECLARE_EVENT_TABLE ()
    };
}

#endif