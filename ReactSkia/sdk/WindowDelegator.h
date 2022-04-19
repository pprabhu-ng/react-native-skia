/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <semaphore.h>
#include <thread>
#include <atomic>

#include "include/core/SkCanvas.h"

#include "rns_shell/compositor/Compositor.h"
#include "rns_shell/common/Window.h"
#include "rns_shell/platform/graphics/PlatformDisplay.h"
#include "rns_shell/platform/graphics/WindowContextFactory.h"


class RNSShellUtils {
    public:
        RNSShellUtils();
        ~RNSShellUtils(){threadOnNeed=false;};
        bool createWindow(SkSize windowSize,std::function<void ()> windowReadyCB);
        void closeWindow();
        void setWindowTittle(const char* titleString);
        void pushToDisplay();

        SkCanvas *oskCanvas{nullptr};

    private:
    void onExposeHandler(RnsShell::Window* window);
    void platformSpecificInit();
    void windowWorkerThread();
    bool createNativeWindow();
    void renderToDisplay();

    std::unique_ptr<RnsShell::WindowContext> oskWindowContext_{nullptr};
    RnsShell::Window* oskWindow_{nullptr};
    sk_sp<SkSurface>  backBuffer_;

    /* members to fuilfill X11 suggestion of "draw on receiving expose event to avoid data loss" */
    sem_t semReadyToDraw;
    std::function<void ()> windowReadyTodrawCB{nullptr};
    RnsShell::PlatformDisplay::Type displayPlatForm;
    SkSize            windowDimension_;
    int exposeEventID_{-1};

    /* member to fulfill OpenGl  requirement of " Create window & render to happen on same thread context */
    sem_t semWaitForActionRequest;
    bool threadOnNeed{true};
    std::atomic <bool> createWindowRequested{false};
    std::atomic <bool> renderToDisplayRequested{false};

};


