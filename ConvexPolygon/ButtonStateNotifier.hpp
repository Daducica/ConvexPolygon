#ifndef BUTTONSTATE_NOTIFIER_HPP
#define BUTTONSTATE_NOTIFIER_HPP

namespace UI
{
    class ButtonStateNotifier
    {
    public:
        virtual void SetClearCanvasButtonState (bool newState) = 0;
        virtual void SetDrawPolygonButtonState (bool newState) = 0;
        virtual ~ButtonStateNotifier ();
    };
}


#endif