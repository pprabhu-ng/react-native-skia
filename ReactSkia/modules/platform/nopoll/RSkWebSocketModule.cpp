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
#define WEBSOCKET_RETURN_SUCESS 0
#define WEBSOCKET_RETURN_FAILURE -1
#define MAX_SEND_RETRIES 5
#define B64DECODE_OUT_SAFESIZE(x) (((x)*3)/4)

namespace facebook {
namespace react {

std::string * RSkWebSocketModule::parseUrl(std::string& url) {
// finding the "s://" substring in the url ex: wss://echo.websocket.org:80
  if(url.find("s://") == -1) {
      LOG(ERROR) << "websocket url is invalid";
      return NULL;
  }
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

jsi::Value RSkWebSocketModule::getConnect(
  std::string url,
  folly::dynamic protocols,
  folly::dynamic options,
  int socketID)  {
  std::string* parsedUrl = RSkWebSocketModule::parseUrl(url);
  if(parsedUrl == NULL) {
      LOG(ERROR) << " parsedUrl is NULL ";
      return jsi::Value(WEBSOCKET_RETURN_FAILURE);
  }
  ctx_ = nopoll_ctx_new ();

  if(!ctx_) {
      LOG(ERROR) << "nopoll_ctx is NULL ";
      return jsi::Value(WEBSOCKET_RETURN_FAILURE);
  }
  /* creating a connection 
   * TO DO: NULL/optional arguments has to be verified */
  noPollConn* conn = nopoll_conn_new(ctx_,  parsedUrl[0].c_str() , parsedUrl[1].c_str() ,  NULL , "/", NULL , NULL);
  delete []parsedUrl;

  if(conn == NULL) {
      LOG(ERROR) << "websocket connection is NULL";
      return jsi::Value(WEBSOCKET_RETURN_FAILURE);
  }

  LOG(WARNING) << "waiting until connection is ok";
  if(!nopoll_conn_wait_until_connection_ready (conn, 5)) {
      LOG(ERROR) << "websocket connection is not ready";
      nopoll_conn_unref(conn);
      return jsi::Value(WEBSOCKET_RETURN_FAILURE);
  }
  LOG(WARNING) << "websocket connection sucess";
  
  connectionList_[socketID] = conn;  
  
  return jsi::Value(WEBSOCKET_RETURN_SUCESS);
}



jsi::Value RSkWebSocketModule::getClose(
  int code,
  std::string reason,
  int socketID)  {
	noPollConn* conn =  connectionList_[socketID];
	if(conn != NULL) {
            nopoll_conn_close_ext(conn, code, reason.c_str(), reason.size());
	    connectionList_.erase(socketID);
	    return jsi::Value(WEBSOCKET_RETURN_SUCESS);
	}
  LOG(ERROR) << "close connection is failed";
  return jsi::Value(WEBSOCKET_RETURN_FAILURE);
}

jsi::Value RSkWebSocketModule::send(
  std::string message,
  int socketID)  {
	int result = 0;
        int num_retries = 0;
	noPollConn* conn =  connectionList_[socketID];
        if(conn != NULL ) {
            result = nopoll_conn_send_text(conn,  message.c_str(), message.length());
	    if (result== message.length()) {
		LOG(INFO) << "sending data sucessfully";
		return jsi::Value(WEBSOCKET_RETURN_SUCESS);
	    } 
	    else {
		/* may be not all data were sent */
                while((num_retries < MAX_SEND_RETRIES) && nopoll_conn_pending_write_bytes(conn) > 0) {
                    /* wait a little bit before re-trying */
                    nopoll_sleep(10000);

                    /* flush and check if write operation completed */
                    if(nopoll_conn_complete_pending_write(conn) == 0) {
                        /* finally all data is sent */
			LOG(INFO) << "sending all data sucessfully";
                        return jsi::Value(WEBSOCKET_RETURN_SUCESS);;
                    }

                    num_retries++;
                }
            }
	        
	    
	}
  LOG(ERROR) << "send data is failed";
  return jsi::Value(WEBSOCKET_RETURN_FAILURE);

}

jsi::Value RSkWebSocketModule::sendBinary(
  std::string base64String,
  int socketID)  {
	int result = 0;
	int num_retries = 0;
	char webSocketBuffer[B64DECODE_OUT_SAFESIZE(base64String.length())];
	int size = B64DECODE_OUT_SAFESIZE(base64String.length()); 
  	int wsBufferSize = B64DECODE_OUT_SAFESIZE(base64String.length());
  	noPollConn* conn =  connectionList_[socketID];
	if(!nopoll_base64_decode(base64String.c_str(), base64String.length(),
		       	webSocketBuffer, &wsBufferSize)) {
             LOG(ERROR) << "nopoll base64 decoding is failed";
             return jsi::Value(WEBSOCKET_RETURN_FAILURE);

	}
	else if(conn != NULL ) {
            result = nopoll_conn_send_binary(conn, webSocketBuffer,strlen(webSocketBuffer) );
	    if (result == strlen(webSocketBuffer)) {
		LOG(INFO) << "sending binary data sucessfully";
		return jsi::Value(WEBSOCKET_RETURN_SUCESS);
	    } 
	    else {
		/* may be not all data were sent */
                while((num_retries < MAX_SEND_RETRIES) &&  nopoll_conn_pending_write_bytes(conn) > 0) {
                    /* wait a little bit before re-trying */
                    nopoll_sleep(10000);

                    /* flush and check if write operation completed */
                    if(nopoll_conn_complete_pending_write(conn) == 0) {
                        /* finally all data is sent */
			LOG(INFO) << "sending all binary data sucessfully";
                        return jsi::Value(WEBSOCKET_RETURN_SUCESS);
                    }

                    num_retries++;
                }
            }
	        
	    
	}
  LOG(ERROR) << "sending binary data is failed";
  return jsi::Value(WEBSOCKET_RETURN_FAILURE);


}

jsi::Value RSkWebSocketModule::ping(
  int socketID)  {
        noPollConn* conn =  connectionList_[socketID];

        if(conn != NULL ) {
	    if(nopoll_conn_send_ping(conn)) {
                LOG(INFO) << "ping operation sucess";
	        return jsi::Value(WEBSOCKET_RETURN_SUCESS);
	    }

        }
  LOG(ERROR) << "ping operation failed";
  return jsi::Value(WEBSOCKET_RETURN_FAILURE);

}

} // namespace react
} // namespace facebook


