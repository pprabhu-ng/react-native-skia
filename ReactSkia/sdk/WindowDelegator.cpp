/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "NotificationCenter.h"
#include "WindowDelegator.h"

void WindowDelegator::createWindow(SkSize windowSize,std::function<void ()> windowReadyCB,bool runOnTaskRunner) {

    windowSize_=windowSize;
    windowReadyTodrawCB_=windowReadyCB;
    displayPlatForm_=RnsShell::PlatformDisplay::sharedDisplayForCompositing().type();

    if(runOnTaskRunner) {
        ownsTaskrunner_ = runOnTaskRunner;
        windowTaskRunner_ = std::make_unique<RnsShell::TaskLoop>();
        std::thread workerThread(&WindowDelegator::windowWorkerThread,this);
        RNS_LOG_TODO("Need to check : Remove worker Thread detach & use join on closewindow");
        workerThread.detach();
        windowTaskRunner_->waitUntilRunning();
        windowTaskRunner_->dispatch([&](){createNativeWindow();});
   } else {
        createNativeWindow();
   }
}

void  WindowDelegator::createNativeWindow() {

    if(displayPlatForm_ == RnsShell::PlatformDisplay::Type::X11) {
        /*For X11 draw should be done after expose event received*/
        sem_init(&semReadyToDraw_,0,0);
        // Registering expose event
        std::function<void(RnsShell::Window*)> handler = std::bind(&WindowDelegator::onExposeHandler,this,
                                                                             std::placeholders::_1);
        exposeEventID_ = NotificationCenter::defaultCenter().addListener("windowExposed",handler);
    }
    oskWindow_ = RnsShell::Window::createNativeWindow(&RnsShell::PlatformDisplay::sharedDisplayForCompositing(),
                                                   SkSize::Make(windowSize_.width(),windowSize_.height()),
                                                   RnsShell::SubWindow);
    if(oskWindow_) {
        oskWindowContext_ = RnsShell::WCF::createContextForWindow(reinterpret_cast<GLNativeWindowType>(oskWindow_->nativeWindowHandle()),
                   &RnsShell::PlatformDisplay::sharedDisplayForCompositing(), RnsShell::DisplayParams());
        if(oskWindowContext_) {
            oskWindowContext_->makeContextCurrent();
            backBuffer_ = oskWindowContext_->getBackbufferSurface();
            oskCanvas = backBuffer_->getCanvas();
            if(displayPlatForm_ == RnsShell::PlatformDisplay::Type::X11) {
                sem_post(&semReadyToDraw_);
            } else if(windowReadyTodrawCB_) windowReadyTodrawCB_();
        } else {
          RNS_LOG_ERROR("Invalid windowContext for nativeWindowHandle : " << oskWindow_->nativeWindowHandle());
        }
    }
}

void WindowDelegator::closeWindow() {

    if(ownsTaskrunner_) windowTaskRunner_->stop();

    if(exposeEventID_) {
        NotificationCenter::defaultCenter().removeListener(exposeEventID_);
        exposeEventID_=-1;
    }
    if(oskWindow_) {
        oskWindow_->closeWindow();
        delete oskWindow_;
        oskWindow_=nullptr;
    }
    sem_destroy(&semReadyToDraw_);
    oskCanvas=nullptr;
    windowReadyTodrawCB_=nullptr;
}

void WindowDelegator::commitDrawCall() {
    if( ownsTaskrunner_ && windowTaskRunner_->running() )  {
        windowTaskRunner_->dispatch([&](){ renderToDisplay(); });
    } else {
        renderToDisplay();
    }
}

inline void WindowDelegator::renderToDisplay() {
    backBuffer_->flushAndSubmit();
    std::vector<SkIRect> emptyRect;// No partialUpdate handled , so passing emptyRect instead of dirtyRect
    oskWindowContext_->swapBuffers(emptyRect);
}

void WindowDelegator::setWindowTittle(const char* titleString) {
    if(oskWindow_) oskWindow_->setTitle(titleString);
}

void WindowDelegator::onExposeHandler(RnsShell::Window* window) {

    if(window  == oskWindow_) {
        sem_wait(&semReadyToDraw_);
        oskWindow_->show();
        if(exposeEventID_) {
            NotificationCenter::defaultCenter().removeListener(exposeEventID_);
            exposeEventID_=-1;
        }
        if(windowReadyTodrawCB_) windowReadyTodrawCB_();
    }
 }

 void WindowDelegator::windowWorkerThread() {
    windowTaskRunner_->run();
}


