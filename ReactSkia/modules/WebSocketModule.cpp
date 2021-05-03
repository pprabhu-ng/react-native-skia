/*  * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.  
*  * This source code is licensed under the MIT license found in the  
* LICENSE file in the root directory of this source tree.  */

#include "jsi/JSIDynamic.h"
#include "ReactSkia/JSITurboModuleManager.h"
#include "WebSocketModule.h"
#include "ReactCommon/TurboModule.h"
#include "cxxreact/Instance.h"
#include <nopoll.h>
#include <glog/logging.h>
namespace facebook {
namespace react {
WebSocketModule::WebSocketModule(
    const std::string &name,
    std::shared_ptr<CallInvoker> jsInvoker,
    Instance *bridgeInstance)
    : TurboModule(name, jsInvoker)	{

    methodMap_["connect"] = MethodMetadata{4, getConnectWrapper};
    methodMap_["send"] = MethodMetadata{2, NoOp};
    methodMap_["sendBinary"] = MethodMetadata{2, NoOp};
    methodMap_["ping"] = MethodMetadata{1, NoOp};
    methodMap_["close"] = MethodMetadata{3, getCloseWrapper};
    methodMap_["addListener"] = MethodMetadata{1, NoOp};
    methodMap_["removeListeners"] = MethodMetadata{1, NoOp};

}
std::string * parse(std::string& url)  {
  std::string* names=new std::string[2];
  std::string delimiter = ":";
  std::string token = url.substr(url.find(delimiter)+3,url.size());
  names[0]  = token.substr(0,token.find(delimiter));
  names[1] = token.substr(token.find(delimiter)+1, token.size());
 	
  return names;
}

jsi::Value WebSocketModule::getConnectWrapper(
     jsi::Runtime &rt,
     TurboModule &turboModule,
     const jsi::Value *args,
     size_t count)  {
	if(count != 4) {
	  return jsi::Value::undefined();
	}

  LOG(INFO) << "Websocket Connect call";
  auto &self = static_cast<WebSocketModule &>(turboModule);
  std::string url = args[0].getString(rt).utf8(rt);
  std::string protocols = args[0].getString(rt).utf8(rt);
  int socketID = args[3].getNumber();

  return self.getConnect(url, protocols, args[2].getObject(rt), socketID);
}

jsi::Value WebSocketModule::getConnect(
  std::string url,
  std::string url1,
  jsi::Object options,
  int socketID)  {
  std::string* parsedUrl = parse(url);
  ctx_ = nopoll_ctx_new ();

  if ( ctx_) {
    LOG(INFO) << "Websocket Context created";
  }
  else {
    LOG(ERROR) << "nopoll_ctx is NULL ";
    return jsi::Value::undefined();
  }

  nopoll_log_enable (ctx_, true);
  nopoll_log_color_enable (ctx_, true);
  /* connection */
  noPollConn* conn = nopoll_conn_new(ctx_,  parsedUrl[0].c_str() , parsedUrl[1].c_str() ,  NULL , "/", NULL , NULL);
  
  if (nopoll_conn_is_ok(conn)) {
    socketList[socketID]= conn;
    LOG(INFO) << "nopoll_conn_new is connected";
  }
  else {
    LOG(ERROR) << "nopoll_conn_new failed!!!";
    nopoll_conn_close (conn);

    return jsi::Value::undefined();
  }

  return jsi::Value::undefined();
}

jsi::Value WebSocketModule::getCloseWrapper(
     jsi::Runtime &rt,
     TurboModule &turboModule,
     const jsi::Value *args,
     size_t count)  {
  if(count != 3) {
    return jsi::Value::undefined();
  }
  LOG(INFO) << "Websocket close call";
  auto &self = static_cast<WebSocketModule &>(turboModule);
  int code = args[0].getNumber();
  std::string reason = args[1].getString(rt).utf8(rt);
  int socketID = args[2].getNumber();

  return self.getClose(code, reason, socketID);

}
jsi::Value WebSocketModule::getClose(
  int code,
  std::string reason,
  int socketID)  {
	noPollConn* conn =  socketList[socketID];
	if(conn != NULL) {
		LOG(INFO) << "Websocket close call";
		nopoll_conn_close_ext(conn, code, NULL, socketID);
		nopoll_conn_unref(conn);
		return jsi::Value::undefined();
	
	}
  return jsi::Value::undefined();

}

} // namespace react
} // namespace facebook


