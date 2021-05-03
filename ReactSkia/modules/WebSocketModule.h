/*  * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.  
*  * This source code is licensed under the MIT license found in the  
* LICENSE file in the root directory of this source tree.  */

#include "ReactCommon/TurboModule.h"
#include "cxxreact/Instance.h"
#include <better/map.h>
#include <nopoll.h>
namespace facebook {
namespace react {
class WebSocketModule: public TurboModule {
    public:
        WebSocketModule(
            const std::string &name,
            std::shared_ptr<CallInvoker> jsInvoker,
            Instance *bridgeInstance);

        static jsi::Value NoOp(
            jsi::Runtime &rt,
            TurboModule &turboModule,
            const jsi::Value *args,
            size_t count) {

            return jsi::Value::undefined();
        }

    private:
        static jsi::Value getConnectWrapper(
            jsi::Runtime &rt,
            TurboModule &turboModule,
            const jsi::Value *args,
            size_t count);

        jsi::Value getConnect(
            std::string,
            std::string,
            jsi::Object,
            int);

        static jsi::Value getCloseWrapper(
            jsi::Runtime &rt,
            TurboModule &turboModule,
            const jsi::Value *args,
            size_t count);

        jsi::Value getClose(
            int,
            std::string,
            int);
#ifdef NOPOLL_ENABLED
        better::map <int , noPollConn*> socketList;
        noPollCtx * ctx_;
#endif
};
} // namespace react
} // namespace facebook
