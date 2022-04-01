/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <semaphore.h>

#include "include/core/SkCanvas.h"

#include "rns_shell/compositor/Compositor.h"
#include "rns_shell/common/Window.h"
#include "rns_shell/platform/graphics/PlatformDisplay.h"
#include "rns_shell/platform/graphics/WindowContextFactory.h"


class RNSShellUtils {
    public:
        bool createWindow(SkSize windowSize,std::function<void ()> windowReadyCB);
        void closeWindow();
        void setWindowTittle(const char* titleString);
        void pushToDisplay();

        SkCanvas *oskCanvas{nullptr};

    private:
    void onExposeHandler(RnsShell::Window* window);

    std::unique_ptr<RnsShell::WindowContext> oskWindowContext_{nullptr};
    RnsShell::Window* oskWindow_{nullptr};
    sem_t semReadyToDraw;/* To sync expose event & window creation for x11 backend*/
    std::function<void ()> windowReadyTodrawCB{nullptr};
    SkSize            windowDimension_;
    sk_sp<SkSurface>  backBuffer_;
    int exposeEventID_{-1};

};


