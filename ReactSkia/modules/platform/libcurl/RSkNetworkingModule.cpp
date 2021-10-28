/*
* Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include <curl/curl.h>

#include "cxxreact/Instance.h"
#include "jsi/JSIDynamic.h"

#include <ReactCommon/TurboModuleUtils.h>
#include "ReactCommon/TurboModule.h"

#include "ReactSkia/utils/RnsLog.h"
#include "ReactSkia/JSITurboModuleManager.h"
#include "RSkNetworkingModule.h"
#ifndef CA_CERTIFICATE
#define CA_CERTIFICATE       "/etc/ssl/certs/ca-certificates.crt"      /**< The certificate of the CA to establish https connection to the server*/
#endif
enum curlStatus {CURL_RETURN_FAILURE=-1,CURL_RETURN_SUCESS};
using namespace std;
namespace facebook {
namespace react {

RSkNetworkingModule::RSkNetworkingModule(
    const std::string &name,
    std::shared_ptr<CallInvoker> jsInvoker,
    Instance *bridgeInstance) :  RSkNetworkingModuleBase(name, jsInvoker, bridgeInstance) {

}

RSkNetworkingModule::~RSkNetworkingModule() {

};

size_t RSkNetworkingModule::progressCallback(void *userdata, double dltotal, double dlnow, double ultotal, double ulnow) {
  ((struct NetworkRequest *)userdata)->self_->sendProgressEventwrapper(userdata,dltotal,dlnow,ultotal,ulnow);
  return 0; 
}

size_t RSkNetworkingModule::writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userdata) {
  size_t realsize = size * nmemb;
  ((struct NetworkRequest *)userdata)->self_->writeMemoryCallbackWrapper(userdata, (char*)contents, realsize);
  return realsize;
}

size_t RSkNetworkingModule::headerCallback(void *contents, size_t size,size_t nitems, void *userdata)
{
  struct NetworkRequest *networkRequest = ((struct NetworkRequest *)userdata);
  networkRequest->self_->headerCallbackWrapper(userdata, (char*)contents);
  std::string buffer = (char*)contents;
  std::string delimiter = "Content-Length:";
  size_t pos = std::string::npos;
  if((pos = buffer.find(delimiter)) != std::string::npos) {
    networkRequest->contentSize_ = stoi(buffer.substr(buffer.find(delimiter)+delimiter.size()+1,buffer.size()));
  }
  return nitems * size;
}
 
uint64_t RSkNetworkingModule::nextUniqueId() {
  static std::atomic<uint64_t> nextId(1);
  uint64_t id;
  do {
    id = nextId.fetch_add(1);
  } while (id == 0);  // 0 invalid id.
  return id;
}

bool RSkNetworkingModule::preparePostRequest(void* request, folly::dynamic headers, folly::dynamic data ) {
  struct NetworkRequest *networkRequest = (struct NetworkRequest*)request;  
  struct curl_slist *curlListRequestHeader = NULL;
  const char *dataPtr = NULL;
  size_t dataSize = 0;
  bool status = false;

  if(data["string"].c_str()) {
    dataPtr = data["string"].c_str();
    dataSize = data["string"].getString().length();
  } else if(data["formData"].c_str()) {
    RNS_LOG_NOT_IMPL;
    return status;
  } else if(data["blob"].c_str()) {
    RNS_LOG_NOT_IMPL;
    return status;
  } else if(data["uri"].c_str()) {
    RNS_LOG_NOT_IMPL;
    return status;
  } else {
    RNS_LOG_ERROR ("Unknown Data for Post Request\n") ;
    return status;
  }
  
  curl_easy_setopt(networkRequest->curl_, CURLOPT_POST, 1L);
  for (auto& pair : headers.items()) {
    /* Key is pair.first, value is pair.second */
    string headerKeyValue = pair.first.c_str();
    const char* parse= ": ";
    headerKeyValue += parse;
    headerKeyValue += pair.second.c_str();
    curlListRequestHeader = curl_slist_append(curlListRequestHeader, headerKeyValue.c_str());
  }
  curl_easy_setopt(networkRequest->curl_, CURLOPT_HTTPHEADER, curlListRequestHeader);
  /* get verbose debug output please */
  curl_easy_setopt(networkRequest->curl_, CURLOPT_POSTFIELDSIZE, (long)dataSize);
  curl_easy_setopt(networkRequest->curl_, CURLOPT_COPYPOSTFIELDS, dataPtr);
  status = true;
  return status;
}

jsi::Value RSkNetworkingModule::sendRequest(
  folly::dynamic query, const jsi::Object &callbackObj, jsi::Runtime &rt) {
  std::string method = query["method"].getString();
  auto url = query["url"];
  auto headers = query["headers"];
  auto data = query["data"];
  std::string responseType  = query["responseType"].getString();
  bool incrementalUpdates  = query["incrementalUpdates"].asInt();
  long timeout = query["timeout"].asDouble();
  auto  withCredentials  = query["withCredentials"];
  CURL *curl;
  CURLcode res = CURLE_FAILED_INIT;
  NetworkRequest *networkRequest =  new NetworkRequest(incrementalUpdates, responseType, this);
  if(curlInit_ == false) {
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    /* Check for errors */
    if(res != CURLE_OK) {
      RNS_LOG_ERROR (stderr << "curl_global_init() failed: %s\n" <<curl_easy_strerror(res));
      return jsi::Value((int)CURL_RETURN_FAILURE);
    }
    curlInit_ = true;
  }

  /* get a curl handle */
  curl = curl_easy_init();
  if(curl == NULL) {
    RNS_LOG_ERROR (stderr << "curl_easy_init() failed \n");
    return jsi::Value((int)CURL_RETURN_FAILURE);
  }
  { 
    networkRequest->curl_ = curl;
    networkRequest->requestId_ = nextUniqueId();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /*The following code gets executed for a https connection.*/

    if(strstr(url.c_str(),"https") != NULL) {
         curl_easy_setopt(curl, CURLOPT_SSLENGINE_DEFAULT, 1L);
         curl_easy_setopt(curl, CURLOPT_CAINFO, CA_CERTIFICATE);
         curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
    }
    if(timeout == 0) {
        /* if timeout is not specified, setting default time as 10L */
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    }
    else
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);//Enable curl logs

    // Progress callback and user data
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L); // Enable progress callback
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, networkRequest );
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressCallback);
     
    //Header callback and user data
    curl_easy_setopt(curl, CURLOPT_WRITEHEADER, networkRequest);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);

    if(method.compare("POST") == 0) {
      if(preparePostRequest(networkRequest, headers, data) == false)
        return jsi::Value((int)CURL_RETURN_FAILURE);
    } else if(method.compare("GET") == 0) {
        // ResponseWrite callback and user data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, networkRequest);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    } else {
        RNS_LOG_ERROR ("Unknown method\n") ;
        return jsi::Value((int)CURL_RETURN_FAILURE);
    }

    // Add the request objet to list and notify app using callback function. 
    connectionList_[networkRequest->requestId_] = networkRequest;
    if(callbackObj.isFunction(rt)) {
      jsi::Function callback = callbackObj.getFunction(rt);
      callback.call(rt, (int) networkRequest->requestId_, 1);
    }
    res = curl_easy_perform(curl);
    {
      // completionBlock
      if(networkRequest->curl_ != NULL) {
        std::scoped_lock lock(isMutating);
        if(!(networkRequest->useIncrementalUpdates_ && (networkRequest->responseType_ == "text")))
          sendData(networkRequest); 
        sendEventWithName("didCompleteNetworkResponse", folly::dynamic::array(networkRequest->requestId_ ,curl_easy_strerror(res), res == CURLE_OPERATION_TIMEDOUT ? "true" : "false" ));
        if(connectionList_[networkRequest->requestId_] != NULL) {
          connectionList_.erase(networkRequest->requestId_);
        }
      }
      delete networkRequest;
    }
  }
  return jsi::Value((int)CURL_RETURN_SUCESS);
}

jsi::Value RSkNetworkingModule::abortRequest(folly::dynamic requestId) {
  std::scoped_lock lock(isMutating);
  struct NetworkRequest *networkRequest = connectionList_[requestId.asInt()];
  if(networkRequest == NULL ) {
    RNS_LOG_ERROR ("networkRequest is not valid \n");
    return jsi::Value((int)CURL_RETURN_FAILURE);
  }
  curl_easy_cleanup(networkRequest->curl_);
  networkRequest->curl_  = NULL;
  connectionList_.erase(requestId.asInt());
  return jsi::Value((int)CURL_RETURN_SUCESS);
}

void RSkNetworkingModule::sendProgressEventwrapper(void* userData, double dltotal,double dlnow, double ultotal, double ulnow) {
  struct NetworkRequest *networkRequest = (struct NetworkRequest*)userData;
  if(networkRequest->uploadComplete_ == false && ultotal != 0) {
    sendEventWithName("didSendNetworkData", folly::dynamic::array(networkRequest->requestId_ , ulnow,ultotal ));
    if(ulnow >= ultotal)
      networkRequest->uploadComplete_ = true;
  }
  if(networkRequest->downloadComplete_ == false && dltotal != 0) {
    // TODO: responseText property needs to get and pass the value in the didReceiveNetworkIncrementalData event
    if(networkRequest->useIncrementalUpdates_) {
      if(networkRequest->responseType_ == "text") {
        sendEventWithName("didReceiveNetworkIncrementalData", folly::dynamic::array(networkRequest->requestId_ , "", dlnow,dltotal ));
      }else 
        sendEventWithName("didReceiveNetworkDataProgress", folly::dynamic::array(networkRequest->requestId_ , dlnow,dltotal ));
    }
    if(dlnow >= dltotal)
      networkRequest->downloadComplete_ = true;
  }
}

void RSkNetworkingModule::headerCallbackWrapper(void* userdata ,char* headerInfoBuffer)
{
  struct NetworkRequest *networkRequest = (struct NetworkRequest*)userdata;
  long response_code;
  char *url = NULL;
  curl_easy_getinfo(networkRequest->curl_, CURLINFO_EFFECTIVE_URL, &url);
  curl_easy_getinfo(networkRequest->curl_, CURLINFO_RESPONSE_CODE, &response_code);  
  sendEventWithName("didReceiveNetworkResponse", folly::dynamic::array(networkRequest->requestId_  , response_code,  headerInfoBuffer ,url));
}

void RSkNetworkingModule::writeMemoryCallbackWrapper(void* userdata ,char* writeMemoryBuffer, size_t realSize)
{              
  struct NetworkRequest *networkRequest = (struct NetworkRequest*)userdata;
  if(networkRequest->responseBuffer_ == nullptr && networkRequest->contentSize_ != 0)
    networkRequest->responseBuffer_  = (char *) malloc(networkRequest->contentSize_);
  if(!(networkRequest->responseBuffer_))
    return;
  memcpy(&(networkRequest->responseBuffer_[networkRequest->responseBufferOffset_]), writeMemoryBuffer, realSize);
  networkRequest->responseBufferOffset_ += realSize;
  if(networkRequest->contentSize_ == networkRequest->responseBufferOffset_) {
     networkRequest->responseBuffer_[networkRequest->responseBufferOffset_] = 0;
  }
}

void RSkNetworkingModule::sendData(NetworkRequest *networkRequest)
{
  if(!(networkRequest->responseBuffer_)){
    if(networkRequest->contentSize_ == 0)
      return;
  }
  if(networkRequest->responseType_ == "text")
    sendEventWithName("didReceiveNetworkData", folly::dynamic::array(networkRequest->requestId_  , networkRequest->responseBuffer_)); 
  else if(networkRequest->responseType_ == "base64") {
    RNS_LOG_NOT_IMPL;
    return;
  } else {
    RNS_LOG_ERROR ( "Invalid responseType: \n");
    return; 
  }
 
}

}// namespace react
}//namespace facebook
