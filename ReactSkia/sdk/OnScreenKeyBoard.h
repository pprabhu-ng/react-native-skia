 /*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "include/core/SkCanvas.h"

#include "rns_shell/common/Window.h"
#include "rns_shell/platform/graphics/PlatformDisplay.h"
#include "rns_shell/compositor/Compositor.h"
#include "rns_shell/platform/graphics/WindowContextFactory.h"

#include "ReactSkia/sdk/NotificationCenter.h"
#include "ReactSkia/sdk/RNSKeyCodeMapping.h"


class OnScreenKeyboard {
    public:
        static void init();
        OnScreenKeyboard();
        void drawOSK();
        static void launch();
        static void exit();
        void keyHandler(rnsKey key, rnsKeyAction eventKeyAction);
    private:
        std::unique_ptr<RnsShell::Window> OSKwindow_;
        std::unique_ptr<RnsShell::WindowContext> OSKwindowContext_;
        sk_sp<SkSurface> backBuffer_;
        SkCanvas *OSKcanvas=nullptr;


};
