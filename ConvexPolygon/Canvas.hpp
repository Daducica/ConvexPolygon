#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <unordered_set>
#include "wx/wx.h"

#include "Geometry.hpp"
#include "Model.hpp"

namespace UI
{
    class ButtonStateNotifier;

    class Canvas : public wxPanel, public Model::CanvasDataUpdater
    {
        Model::CanvasData data;
        ButtonStateNotifier& buttonStateNotifier;

        void PaintNow ();
        void Render (wxDC& dc);
        void DrawPoints (wxDC& dc);
        void DrawPolygon (wxDC& dc);
    public:
        Canvas (wxFrame* parent, const wxPoint& position, const wxSize& size, ButtonStateNotifier& buttonStateNotifier);

        void PaintEvent (wxPaintEvent& evt);
        void MouseReleased (wxMouseEvent& event);
        void ClearPoints ();
        const Model::UIPointSet& GetCurrentPointSet () const;
        void DrawNewPolygon (const Model::UIPolygon& newPolygonPoints);

        virtual void PointAdded () override;
        virtual void CanvasCleared () override;
        virtual void PolygonUpdated () override;

        DECLARE_EVENT_TABLE ()
    };
}

#endif