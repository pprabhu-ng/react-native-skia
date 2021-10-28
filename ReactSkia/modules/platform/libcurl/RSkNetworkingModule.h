/*
* Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include <curl/curl.h>
#include <better/map.h>


#include "ReactSkia/modules/RSkNetworkingModuleBase.h"

namespace facebook {
namespace react {
class RSkNetworkingModule;
struct NetworkRequest {
       NetworkRequest(bool useIncrementalUpdates,  std::string responseType, RSkNetworkingModule *self)
       :self_(self)
       ,useIncrementalUpdates_(useIncrementalUpdates)
       ,responseType_(responseType)
       ,uploadComplete_(false)
       ,downloadComplete_(false)
       ,responseBuffer_(nullptr)
       ,contentSize_(0)
       ,responseBufferOffset_(0)
       {
       }
  CURL* curl_;
  int requestId_;
  RSkNetworkingModule *self_;
  bool useIncrementalUpdates_;
  std::string responseType_;
  bool uploadComplete_;
  bool downloadComplete_;
  char *responseBuffer_;
  size_t contentSize_;
  int responseBufferOffset_;


};

class RSkNetworkingModule:  public RSkNetworkingModuleBase {
 public:
  RSkNetworkingModule(
      const std::string &name,
      std::shared_ptr<CallInvoker> jsInvoker,
      Instance *bridgeInstance);
      ~ RSkNetworkingModule();

   jsi::Value sendRequest(
       folly::dynamic,
       const jsi::Object&,
       jsi::Runtime&) override;

   jsi::Value abortRequest(
       folly::dynamic) override;

  void sendData(NetworkRequest*);
  bool preparePostRequest(CURL*, folly::dynamic, folly::dynamic );
  void sendProgressEventwrapper(void*,double,double,double,double);
  void headerCallbackWrapper(void*, char*);
  void writeMemoryCallbackWrapper(void* ,char*, size_t);
  better::map <int , NetworkRequest*> connectionList_;
  static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
  static size_t progressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
  static size_t headerCallback(void *contents, size_t size, size_t nitems, void *userdata);

 private:
  std::mutex isMutating;
  bool curlInit_ = false;
  uint64_t nextUniqueId();

};

}// namespace react
}// namespace facebook
