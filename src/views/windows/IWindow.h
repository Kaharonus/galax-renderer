//
// Created by tomas on 30.1.23.
//

#pragma once


namespace Galax::Windowing {
    class IWindow {
    public:
        IWindow() = default;
        virtual void show(bool blocking) = 0;
    };
}
