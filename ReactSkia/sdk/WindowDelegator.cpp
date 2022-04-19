/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "NotificationCenter.h"
#include "WindowDelegator.h"

RNSShellUtils::RNSShellUtils() {
    displayPlatForm=RnsShell::PlatformDisplay::sharedDisplayForCompositing().type();

    /*Creating a worker Thread, To match OpenGl  requirement of " Create window & render to happen on same thread context"*/
    if(displayPlatForm == RnsShell::PlatformDisplay::Type::X11) {
        sem_init(&semWaitForActionRequest,0,0);
        std::thread workerThread(&RNSShellUtils::windowWorkerThread,this);
        workerThread.detach();
    }
}

bool RNSShellUtils::createWindow(SkSize windowSize,std::function<void ()> windowReadyCB) {
    bool returnCode{true};
    windowDimension_=windowSize;
    windowReadyTodrawCB=windowReadyCB;
    if(displayPlatForm == RnsShell::PlatformDisplay::Type::X11) {
        createWindowRequested=true;
        sem_post(&semWaitForActionRequest);
    } else {
        returnCode = createNativeWindow();
        if(returnCode) {
            oskWindow_->show();
            if(windowReadyTodrawCB) windowReadyTodrawCB();
        }
    }
    return returnCode;
}

void RNSShellUtils::closeWindow() {

    windowReadyTodrawCB=nullptr;

    /*Resetting Platofrm Specific data members*/
    if(displayPlatForm == RnsShell::PlatformDisplay::Type::X11) {
        if(exposeEventID_) {
            NotificationCenter::defaultCenter().removeListener(exposeEventID_);
            exposeEventID_=-1;
        }
        sem_destroy(&semWaitForActionRequest);
        createWindowRequested=false;
        renderToDisplayRequested=false;
    }
    if(oskWindow_) {
        oskWindow_->closeWindow();
        delete oskWindow_;
        oskWindow_=nullptr;
    }
    sem_destroy(&semReadyToDraw);
    oskCanvas=nullptr;
}

void RNSShellUtils::pushToDisplay() {

    if(( displayPlatForm == RnsShell::PlatformDisplay::Type::X11 ) && (!renderToDisplayRequested)) {
        renderToDisplayRequested=true;
        sem_post(&semWaitForActionRequest);
    } else {
        renderToDisplay();
    }
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

bool RNSShellUtils::createNativeWindow() {
    bool returnCode{false};
    oskWindow_ = RnsShell::Window::createNativeWindow(&RnsShell::PlatformDisplay::sharedDisplayForCompositing(),
                                                   SkSize::Make(windowDimension_.width(),windowDimension_.height()),
                                                   RnsShell::SubWindow);
    if(oskWindow_) {
        oskWindowContext_ = RnsShell::WCF::createContextForWindow(reinterpret_cast<GLNativeWindowType>(oskWindow_->nativeWindowHandle()),
                   &RnsShell::PlatformDisplay::sharedDisplayForCompositing(), RnsShell::DisplayParams());
        if(oskWindowContext_) {
            oskWindowContext_->makeContextCurrent();
            backBuffer_ = oskWindowContext_->getBackbufferSurface();
            oskCanvas = backBuffer_->getCanvas();
            returnCode=true;
            if(displayPlatForm == RnsShell::PlatformDisplay::Type::X11) sem_post(&semReadyToDraw);
        } else {
          RNS_LOG_ERROR("Invalid windowContext for nativeWindowHandle : " << oskWindow_->nativeWindowHandle());
        }
    }
    return returnCode;
}
void RNSShellUtils::renderToDisplay() {
    backBuffer_->flushAndSubmit();
    std::vector<SkIRect> emptyRect;// No partialUpdate handled , so passing emptyRect instead of dirtyRect
    oskWindowContext_->swapBuffers(emptyRect);
}
 void RNSShellUtils::windowWorkerThread() {
    while(threadOnNeed) {
        sem_wait(&semWaitForActionRequest);
        if(createWindowRequested) {
             /*For X11 draw should be done after expose event received*/
             sem_init(&semReadyToDraw,0,0);
             // Registering expose event
             std::function<void(RnsShell::Window*)> handler = std::bind(&RNSShellUtils::onExposeHandler,this,
                                                                         std::placeholders::_1);
             exposeEventID_ = NotificationCenter::defaultCenter().addListener("windowExposed",handler);
             createNativeWindow();
             createWindowRequested=false;
             renderToDisplayRequested=false;/*Discard old render request */
        } else if (renderToDisplayRequested) {
            renderToDisplay();
            renderToDisplayRequested=false;
        }
    }
}


