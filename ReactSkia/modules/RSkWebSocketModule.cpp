/*  * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.  
*  * This source code is licensed under the MIT license found in the  
* LICENSE file in the root directory of this source tree.  */
#include <nopoll.h>
#include <glog/logging.h>

#include "cxxreact/Instance.h"
#include "jsi/JSIDynamic.h"

#include <ReactCommon/TurboModuleUtils.h>
#include "ReactCommon/TurboModule.h"

#include "ReactSkia/JSITurboModuleManager.h"
#include "RSkWebSocketModule.h"

#define WEBSOCKET_URL 0
#define WEBSOCKET_PORTNO 1

namespace facebook {
namespace react {
RSkWebSocketModule::RSkWebSocketModule(
    const std::string &name,
    std::shared_ptr<CallInvoker> jsInvoker,
    Instance *bridgeInstance)
    : TurboModule(name, jsInvoker) {

    methodMap_["connect"] = MethodMetadata{4, getConnectWrapper};
    methodMap_["send"] = MethodMetadata{2, NoOp};
    methodMap_["sendBinary"] = MethodMetadata{2, NoOp};
    methodMap_["ping"] = MethodMetadata{1, NoOp};
    methodMap_["close"] = MethodMetadata{3, getCloseWrapper};
    methodMap_["addListener"] = MethodMetadata{1, NoOp};
    methodMap_["removeListeners"] = MethodMetadata{1, NoOp};

}

std::string * parseUrl(std::string& url) {
  std::string* webSocketUrl = new std::string[2];
  std::string delimiter = ":";
  std::string webSocketEndPoint = url.substr(url.find(delimiter)+3,url.size());
  webSocketUrl[WEBSOCKET_URL]  = webSocketEndPoint.substr(0,
		 		   webSocketEndPoint.find(delimiter));

  webSocketUrl[WEBSOCKET_PORTNO] = webSocketEndPoint.substr(
		                     webSocketEndPoint.find(delimiter)+1,
				     webSocketEndPoint.size());

  return webSocketUrl;
}

jsi::Value RSkWebSocketModule::getConnectWrapper(
     jsi::Runtime &rt,
     TurboModule &turboModule,
     const jsi::Value *args,
     size_t count)  {
	if(count != 4) {
            return jsi::Value::undefined();
	}

  LOG(INFO) << "Websocket Connect call";
  auto &self = static_cast<RSkWebSocketModule &>(turboModule);
  std::string url = args[0].getString(rt).utf8(rt);
  auto protocols = jsi::dynamicFromValue(rt, args[1]);	
  int socketID = args[3].getNumber();
  return self.getConnect(url, protocols, args[2].getObject(rt), socketID);
}

jsi::Value RSkWebSocketModule::getConnect(
  std::string url,
  folly::dynamic protocols,
  jsi::Object options,
  int socketID)  {
  std::string* parsedUrl = parseUrl(url);
  ctx_ = nopoll_ctx_new ();

  if(ctx_) {
      LOG(INFO) << "Websocket Context created";
  }
  else {
      LOG(ERROR) << "nopoll_ctx is NULL ";
      return jsi::Value::undefined();
  }

  nopoll_log_enable (ctx_, true);
  nopoll_log_color_enable (ctx_, true);
  /* creating a connection 
   * TO DO: NULL/optional arguments has to be verified */
  noPollConn* conn = nopoll_conn_new(ctx_,  parsedUrl[0].c_str() , parsedUrl[1].c_str() ,  NULL , "/", NULL , NULL);
  
  if(nopoll_conn_is_ok(conn)) {
      connectionList_[socketID]= conn;
      LOG(INFO) << "nopoll_conn_new is connected";
  }
  else {
    LOG(ERROR) << "nopoll_conn_new failed!!!";
    nopoll_conn_close (conn);
    return jsi::Value::undefined();
  }

  return jsi::Value::undefined();
}

jsi::Value RSkWebSocketModule::getCloseWrapper(
     jsi::Runtime &rt,
     TurboModule &turboModule,
     const jsi::Value *args,
     size_t count)  {
  if(count != 3) {
      return jsi::Value::undefined();
  }
  LOG(INFO) << "Websocket close call";
  auto &self = static_cast<RSkWebSocketModule &>(turboModule);
  int code = args[0].getNumber();
  std::string reason = args[1].getString(rt).utf8(rt);
  int socketID = args[2].getNumber();

  return self.getClose(code, reason, socketID);

}
jsi::Value RSkWebSocketModule::getClose(
  int code,
  std::string reason,
  int socketID)  {
	noPollConn* conn =  connectionList_[socketID];
	if(conn != NULL) {
	    LOG(INFO) << "Websocket close call";
            connectionList_.erase(socketID);
	    nopoll_conn_unref(conn);
	    return jsi::Value::undefined();
	}
  return jsi::Value::undefined();
}


} // namespace react
} // namespace facebook


