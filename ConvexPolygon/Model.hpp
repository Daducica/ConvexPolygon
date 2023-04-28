#ifndef MODEL_HPP
#define MODEL_HPP

#include "wx/wx.h"

#include "Geometry.hpp"

namespace Model
{
    typedef Geometry::PointHashFunction<wxPoint> UIPointHashFunction;
    typedef std::unordered_set<wxPoint, UIPointHashFunction> UIPointSet;
    typedef std::vector<wxPoint> UIPolygon;

    class CanvasDataUpdater
    {
    public:
        virtual void PointAdded () = 0;
        virtual void CanvasCleared () = 0;
        virtual void PolygonUpdated () = 0;
        virtual ~CanvasDataUpdater ();
    };

    class CanvasData
    {
        Model::UIPointSet points;
        Model::UIPolygon polygonPoints;
        bool isPolygonUpToDate;
        CanvasDataUpdater& updater;
    public:
        CanvasData (CanvasDataUpdater& updater);
        const Model::UIPointSet& GetPoints () const;
        const Model::UIPolygon& GetPolygonPoints () const;
        bool IsPolygonUpToDate () const;
        void ClearPoints ();
        void AddPoint (const wxPoint& newPoint);
        void UpdatePolygon (const Model::UIPolygon& newPolygonPoints);
    };
}

#endif