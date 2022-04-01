/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "NotificationCenter.h"
#include "RNSShellUtils.h"

bool RNSShellUtils::createWindow(SkSize windowSize,std::function<void ()> windowReadyCB) {
    bool returnCode{false};
    windowDimension_=windowSize;
    windowReadyTodrawCB=windowReadyCB;

    if(RnsShell::PlatformDisplay::sharedDisplayForCompositing().type() == RnsShell::PlatformDisplay::Type::X11) {
        /*For X11 draw should be done after expose event received*/
        sem_init(&semReadyToDraw,0,0);
        // Registering expose event
        std::function<void(RnsShell::Window*)> handler = std::bind(&RNSShellUtils::onExposeHandler,this,
                                                                          std::placeholders::_1);
        exposeEventID_ = NotificationCenter::defaultCenter().addListener("windowExposed",handler);
    }
    oskWindow_ = RnsShell::Window::createNativeWindow(&RnsShell::PlatformDisplay::sharedDisplayForCompositing(),
                                                   SkSize::Make(windowSize.width(),windowSize.height()),
                                                   RnsShell::SubWindow);
    if(oskWindow_) {
        oskWindowContext_ = RnsShell::WCF::createContextForWindow(reinterpret_cast<GLNativeWindowType>(oskWindow_->nativeWindowHandle()),
                   &RnsShell::PlatformDisplay::sharedDisplayForCompositing(), RnsShell::DisplayParams());
        if(oskWindowContext_) {
            oskWindowContext_->makeContextCurrent();
            backBuffer_ = oskWindowContext_->getBackbufferSurface();
            oskCanvas = backBuffer_->getCanvas();
            returnCode=true;
            if(RnsShell::PlatformDisplay::sharedDisplayForCompositing().type() == RnsShell::PlatformDisplay::Type::X11) {
                sem_post(&semReadyToDraw);
            } else {
                oskWindow_->show();
                if(windowReadyTodrawCB){
                    windowReadyTodrawCB();
                }
            }
        } else {
          RNS_LOG_ERROR("Invalid windowContext for nativeWindowHandle : " << oskWindow_->nativeWindowHandle());
        }
    }

    return returnCode;
}

void RNSShellUtils::closeWindow() {
    if(exposeEventID_) {
        NotificationCenter::defaultCenter().removeListener(exposeEventID_);
        exposeEventID_=-1;
    }
    if(oskWindow_) {
        oskWindow_->closeWindow();
        delete oskWindow_;
        oskWindow_=nullptr;
    }
    sem_destroy(&semReadyToDraw);
    windowReadyTodrawCB=nullptr;
    oskCanvas=nullptr;
}

void RNSShellUtils::pushToDisplay() {
    backBuffer_->flushAndSubmit();
    std::vector<SkIRect> emptyRect;// No partialUpdate handled , so passing emptyRect instead of dirtyRect
    oskWindowContext_->swapBuffers(emptyRect);
}

void RNSShellUtils::setWindowTittle(const char* titleString) {
    if(oskWindow_) oskWindow_->setTitle(titleString);
}

void RNSShellUtils::onExposeHandler(RnsShell::Window* window) {
    if(window  == oskWindow_) {
        sem_wait(&semReadyToDraw);
        oskWindow_->show();
        if(exposeEventID_) {
            NotificationCenter::defaultCenter().removeListener(exposeEventID_);
            exposeEventID_=-1;
        }
        if(windowReadyTodrawCB) windowReadyTodrawCB();
    }
 }


