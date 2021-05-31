/*  * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.  
*  * This source code is licensed under the MIT license found in the  
* LICENSE file in the root directory of this source tree.  */
#include <glog/logging.h>

#include "cxxreact/Instance.h"
#include "jsi/JSIDynamic.h"

#include <ReactCommon/TurboModuleUtils.h>
#include "ReactCommon/TurboModule.h"

#include "ReactSkia/JSITurboModuleManager.h"
#include "RSkWebSocketModuleBase.h"


namespace facebook {
namespace react {
RSkWebSocketModuleBase::RSkWebSocketModuleBase(
    const std::string &name,
    std::shared_ptr<CallInvoker> jsInvoker,
    Instance *bridgeInstance)
    : RSkEventEmitter(name, jsInvoker, bridgeInstance) {
    methodMap_["connect"] = MethodMetadata{4, getConnectWrapper};
    methodMap_["send"] = MethodMetadata{2, sendWrapper};
    methodMap_["sendBinary"] = MethodMetadata{2,sendBinaryWrapper};
    methodMap_["ping"] = MethodMetadata{1, pingWrapper};
    methodMap_["close"] = MethodMetadata{3, getCloseWrapper};
}

jsi::Value RSkWebSocketModuleBase::getConnectWrapper(
     jsi::Runtime &rt,
     TurboModule &turboModule,
     const jsi::Value *args,
     size_t count)  {
  if(count != 4) {
      LOG(ERROR) <<"arguments is not matching"; 
      return jsi::Value::undefined();
  }
  auto &self = static_cast<RSkWebSocketModuleBase &>(turboModule);
  std::string url = args[0].getString(rt).utf8(rt);
  auto protocols = jsi::dynamicFromValue(rt, args[1]);
  auto options = jsi::dynamicFromValue(rt, args[2]);
  int socketID = args[3].getNumber();
 
  return self.getConnect(url, protocols, options, socketID);
}

jsi::Value RSkWebSocketModuleBase::getCloseWrapper(
     jsi::Runtime &rt,
     TurboModule &turboModule,
     const jsi::Value *args,
     size_t count)  {
  if(count != 3) {
      LOG(ERROR) <<"arguments is not matching";
      return jsi::Value::undefined();
  }
  auto &self = static_cast<RSkWebSocketModuleBase &>(turboModule);
  int code = args[0].getNumber();
  std::string reason = args[1].getString(rt).utf8(rt);
  int socketID = args[2].getNumber();
  return self.getClose(code, reason.c_str(), socketID);
}

jsi::Value RSkWebSocketModuleBase::sendWrapper(
     jsi::Runtime &rt,
     TurboModule &turboModule,
     const jsi::Value *args,
     size_t count)  {
  if(count != 2) {
      LOG(ERROR) <<"arguments is not matching";
      return jsi::Value::undefined();
  }
  auto &self = static_cast<RSkWebSocketModuleBase &>(turboModule);
  std::string message = args[0].getString(rt).utf8(rt);
  int socketID = args[1].getNumber();

  return self.send(message, socketID);

}

jsi::Value RSkWebSocketModuleBase::sendBinaryWrapper(
     jsi::Runtime &rt,
     TurboModule &turboModule,
     const jsi::Value *args,
     size_t count)  {
  if(count != 2) {
      LOG(ERROR) <<"arguments is not matching";
      return jsi::Value::undefined();
  }
  auto &self = static_cast<RSkWebSocketModuleBase &>(turboModule);
  std::string base64String = args[0].getString(rt).utf8(rt);
  int socketID = args[1].getNumber();

  return self.sendBinary(base64String, socketID);

}

jsi::Value RSkWebSocketModuleBase::pingWrapper(
     jsi::Runtime &rt,
     TurboModule &turboModule,
     const jsi::Value *args,
     size_t count)  {
  if(count != 1) {
      LOG(ERROR) <<"arguments is not matching";
      return jsi::Value::undefined();
  }
  auto &self = static_cast<RSkWebSocketModuleBase &>(turboModule);
  int socketID = args[0].getNumber();

  return self.ping(socketID);

}

}// namespace react
}// namespace facebook
