#include "Model.hpp"

namespace Model
{
    CanvasDataUpdater::~CanvasDataUpdater () = default;


    CanvasData::CanvasData (CanvasDataUpdater& updater) :
        isPolygonUpToDate (true),
        updater (updater)
    {}


    const Model::UIPointSet& CanvasData::GetPoints () const
    {
        return points;
    }


    const Model::UIPolygon& CanvasData::GetPolygonPoints () const
    {
        return polygonPoints;
    }


    bool CanvasData::IsPolygonUpToDate () const
    {
        return isPolygonUpToDate;
    }


    void CanvasData::ClearPoints ()
    {
        points.clear ();
        polygonPoints.clear ();
        updater.CanvasCleared ();
    }


    void CanvasData::AddPoint (const wxPoint& newPoint)
    {
        points.insert (newPoint);
        if (isPolygonUpToDate)
            isPolygonUpToDate = false;
        updater.PointAdded ();
    }


    void CanvasData::UpdatePolygon (const Model::UIPolygon& newPolygonPoints)
    {
        assert (newPolygonPoints.size () > 2);
        polygonPoints = newPolygonPoints;
        isPolygonUpToDate = true;
        updater.PolygonUpdated ();
    }
}