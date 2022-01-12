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

#include "NotificationCenter.h"
#include "RNSKeyCodeMapping.h"
#include "OSKLayout.h"

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
        OSKLayout::KeyLayoutInfo getNextKeyInfo(rnsKey navkey );//process navigation key 
        OSKLayout::KeyLayoutInfo getFocussedKeyInfo(rnsKey Key);
        void highlightKey(OSKLayout::KeyLayoutInfo keyInfo);
        OSKLayout OSKLayout_;
        OSKLayout::OSKTypes currentOSKType_{OSKLayout::OSK_DEFAULT};
        SkPoint currentIndex_{0,0};
        rnsKey focussedKey{RNS_KEY_UnKnown};
};

