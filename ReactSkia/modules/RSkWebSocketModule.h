/*  * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.  
*  * This source code is licensed under the MIT license found in the  
* LICENSE file in the root directory of this source tree.  */
#include <nopoll.h>
#include <better/map.h>

#include "cxxreact/Instance.h"
#include "ReactCommon/TurboModule.h"

namespace facebook {
namespace react {
class RSKWebSocketModuleBase {
        virtual jsi::Value getConnect(
            std::string,
            folly::dynamic,
            jsi::Object,
            int){
		return {};
	}

        virtual jsi::Value getClose(
            int,
            std::string,
            int){
		return {};
	}

};
class RSkWebSocketModule: public TurboModule , public RSKWebSocketModuleBase {
    public:
        RSkWebSocketModule(
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

        static jsi::Value getCloseWrapper(
            jsi::Runtime &rt,
            TurboModule &turboModule,
            const jsi::Value *args,
            size_t count);
	
	jsi::Value getConnect(
            std::string,
            folly::dynamic,
            jsi::Object,
            int) override;

	jsi::Value getClose(
            int,
            std::string,
            int) override;

#ifdef NOPOLL_ENABLED
        better::map <int , noPollConn*> connectionList_;
        noPollCtx* ctx_;
#endif
};
} // namespace react
} // namespace facebook
