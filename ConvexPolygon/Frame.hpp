#ifndef FRAME_HPP
#define FRAME_HPP

#include "wx/wx.h"
#include <wx/wxprec.h>

namespace UI
{
    class Canvas;

    class ButtonStateNotifier
    {
    public:
        virtual void SetClearCanvasButtonState (bool newState) = 0;
        virtual void SetDrawPolygonButtonState (bool newState) = 0;
        virtual ~ButtonStateNotifier ();
    };


    class Frame : public wxFrame, public ButtonStateNotifier
    {
    public:
        enum
        {
            ClearButton = wxID_HIGHEST + 1,
            DrawPolygonButton = wxID_HIGHEST + 2
        };

        Frame ();

        void CreateUIElements ();
        void OnClearButtonClicked (wxCommandEvent& event);
        void OnDrawPolygonButtonClicked (wxCommandEvent& event);

        virtual void SetClearCanvasButtonState (bool newState) override;
        virtual void SetDrawPolygonButtonState (bool newState) override;

        UI::Canvas* canvas;
        wxButton* clearCanvasButton;
        wxButton* drawPolygonButton;

        DECLARE_EVENT_TABLE ()
    };

}

#endif