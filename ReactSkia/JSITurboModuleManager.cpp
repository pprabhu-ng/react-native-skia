#include <folly/io/async/ScopedEventBaseThread.h>

#include "cxxreact/Instance.h"
#include "jsi/JSIDynamic.h"

#include "JSITurboModuleManager.h"
#include "version.h"
#include "core_modules/RSkDeviceInfo.h"
#include "core_modules/RSkTimingModule.h"
#include "core_modules/RSkKeyboardObserver.h"
#include "modules/platform/nopoll/RSkWebSocketModule.h"
#include "modules/platform/libcurl/RSkNetworkingModule.h"
#include "modules/RSkTVNavigationEventEmitter.h"
#include "utils/RnsLog.h"

namespace facebook {
namespace react {

namespace {

static jsi::Value NoOp(
    jsi::Runtime &rt,
    TurboModule &turboModule,
    const jsi::Value *args,
    size_t count) {
  return jsi::Value::undefined();
}

class StaticTurboModule : public TurboModule {
 public:
  StaticTurboModule(
      const std::string &name,
      std::shared_ptr<CallInvoker> jsInvoker)
      : TurboModule(name, jsInvoker) {}

  facebook::jsi::Value get(
      facebook::jsi::Runtime &runtime,
      const facebook::jsi::PropNameID &propName) override {
    std::string propNameUtf8 = propName.utf8(runtime);
    if (propNameUtf8 == "getConstants") {
      return jsi::Function::createFromHostFunction(
          runtime,
          propName,
          0,
          [this](
              jsi::Runtime &rt,
              const jsi::Value &thisVal,
              const jsi::Value *args,
              size_t count) { return jsi::valueFromDynamic(rt, constants_); });
    }
    return jsi::Value::undefined();
  }

  void SetConstants(folly::dynamic &&constants) {
    constants_ = std::move(constants);
  }

 private:
  folly::dynamic constants_;
};

class ExceptionsManagerModule : public TurboModule {
 public:
  ExceptionsManagerModule(
      const std::string &name,
      std::shared_ptr<CallInvoker> jsInvoker)
      : TurboModule(name, jsInvoker) {
    methodMap_["reportException"] = MethodMetadata{1, ReportException};

    methodMap_["updateExceptionMessage"] = MethodMetadata{3, NoOp};
  }

 private:
  static jsi::Value ReportException(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    if (count == 1 && args[0].isObject()) {
      auto data = args[0].asObject(rt);
      auto message = data.getProperty(rt, "message");
      RNS_LOG_ERROR("[ExceptionManager] message : " << message.asString(rt).utf8(rt));
    }
    return jsi::Value::undefined();
  }
};

class AppStateModule : public TurboModule {
 public:
  AppStateModule(
      const std::string &name,
      std::shared_ptr<CallInvoker> jsInvoker)
      : TurboModule(name, jsInvoker) {
    methodMap_["getConstants"] = MethodMetadata{0, GetConstants};

    methodMap_["getCurrentAppState"] = MethodMetadata{2, NoOp};
    methodMap_["addListener"] = MethodMetadata{1, NoOp};
    methodMap_["removeListener"] = MethodMetadata{1, NoOp};
  }

 private:
  static jsi::Value GetConstants(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    return jsi::valueFromDynamic(
        rt, folly::dynamic::array("initialAppState", "active"));
  }
};

class AppearanceModule: public TurboModule {
 public:
  AppearanceModule(
      const std::string &name,
      std::shared_ptr<CallInvoker> jsInvoker)
      : TurboModule(name, jsInvoker) {
    methodMap_["getColorScheme"] = MethodMetadata{0, getColorScheme};
    methodMap_["addListener"] = MethodMetadata{1, addListener};
    methodMap_["removeListeners"] = MethodMetadata{1, removeListeners};
  }

 private:
  static jsi::Value getColorScheme(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("AppearanceModule getColorScheme");
    return jsi::Value::undefined();
  }

  static jsi::Value addListener(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("AppearanceModule addListener");
    return jsi::Value::undefined();
  }
  static jsi::Value removeListeners(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("AppearanceModule removeListeners");
    return jsi::Value::undefined();
  }
};

class DevLoadingViewModule: public TurboModule {
 public:
  DevLoadingViewModule(
      const std::string &name,
      std::shared_ptr<CallInvoker> jsInvoker)
      : TurboModule(name, jsInvoker) {
    methodMap_["showMessage"] = MethodMetadata{3, showMessage};
    methodMap_["hide"] = MethodMetadata{0, hide};
  }

 private:
  static jsi::Value showMessage(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevLoadingView showMessage");
    return jsi::Value::undefined();
  }

  static jsi::Value hide(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevLoadingView hide");
    return jsi::Value::undefined();
  }
};

class DevSettingsModule : public TurboModule {
 public:
  DevSettingsModule(
      const std::string &name,
      std::shared_ptr<CallInvoker> jsInvoker)
      : TurboModule(name, jsInvoker) {
    methodMap_["reload"] = MethodMetadata{0, reload};
    methodMap_["reloadWithReason"] = MethodMetadata{1, reloadWithReason};
    methodMap_["onFastRefresh"] = MethodMetadata{0, onFastRefresh};
    methodMap_["setHotLoadingEnabled"] = MethodMetadata{1, setHotLoadingEnabled};
    methodMap_["setIsDebuggingRemotely"] = MethodMetadata{1, setIsDebuggingRemotely};
    methodMap_["setProfilingEnabled"] = MethodMetadata{1, setProfilingEnabled};
    methodMap_["toggleElementInspector"] = MethodMetadata{0, toggleElementInspector};
    methodMap_["addMenuItem"] = MethodMetadata{1, addMenuItem};
    methodMap_["addListener"] = MethodMetadata{1, addListener};
    methodMap_["removeListeners"] = MethodMetadata{1, removeListeners};
    methodMap_["setIsShakeToShowDevMenuEnabled"] = MethodMetadata{1, setIsShakeToShowDevMenuEnabled};
  }

 private:
  static jsi::Value reloadWithReason(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings reloadWithReason");
    return jsi::Value::undefined();
  }

  static jsi::Value reload(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings reload");
    return jsi::Value::undefined();
  }
  static jsi::Value onFastRefresh(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings onFastRefresh");
    return jsi::Value::undefined();
  }
  static jsi::Value setHotLoadingEnabled(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings setHotLoadingEnabled");
    return jsi::Value::undefined();
  }
  static jsi::Value setIsDebuggingRemotely(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings setIsDebuggingRemotely");
    return jsi::Value::undefined();
  }
  static jsi::Value setProfilingEnabled(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings setProfilingEnabled");
    return jsi::Value::undefined();
  }
  static jsi::Value toggleElementInspector(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings toggleElementInspector");
    return jsi::Value::undefined();
  }
  static jsi::Value addMenuItem(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings addMenuItem");
    return jsi::Value::undefined();
  }

  static jsi::Value addListener(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings addListener");
    return jsi::Value::undefined();
  }
  static jsi::Value removeListeners(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings removeListeners");
    return jsi::Value::undefined();
  }
  static jsi::Value setIsShakeToShowDevMenuEnabled(
      jsi::Runtime &rt,
      TurboModule &turboModule,
      const jsi::Value *args,
      size_t count) {
    RNS_LOG_INFO("DevSettings setIsShakeToShowDevMenuEnabled");
    return jsi::Value::undefined();
  }
};

class UnimplementedTurboModule : public TurboModule {
 public:
  UnimplementedTurboModule(
      const std::string &name,
      std::shared_ptr<CallInvoker> jsInvoker)
       : TurboModule(name, jsInvoker) {}

  facebook::jsi::Value get(
      facebook::jsi::Runtime &runtime,
      const facebook::jsi::PropNameID &propName) override {
    return jsi::Value::undefined();
  };
};

} // namespace

JSITurboModuleManager::JSITurboModuleManager(Instance *bridgeInstance)
    : bridgeInstance_(bridgeInstance) {
  std::shared_ptr<CallInvoker> jsInvoker = bridgeInstance->getJSCallInvoker();
  auto staticModule =
      std::make_shared<StaticTurboModule>("SourceCode", jsInvoker);
  staticModule->SetConstants(folly::dynamic::object("scriptURL", "foo"));
  modules_["SourceCode"] = std::move(staticModule);

  staticModule =
      std::make_shared<StaticTurboModule>("PlatformConstants", jsInvoker);
  auto rnVersion = folly::dynamic::object("major", RN_MAJOR_VERSION)("minor", RN_MINOR_VERSION)("patch", RN_PATCH_VERSION);
  staticModule->SetConstants(folly::dynamic::object("isTesting", true)(
      "reactNativeVersion", std::move(rnVersion)) ("osVersion",STRINGIFY(RNS_OS_VERSION))
#if TARGET_OS_TV
      ("interfaceIdiom", STRINGIFY(tv))
#else
      ("interfaceIdiom", STRINGIFY(unknown))
#endif
    );
  modules_["PlatformConstants"] = std::move(staticModule);

  modules_["ExceptionsManager"] =
      std::make_shared<ExceptionsManagerModule>("ExceptionsManager", jsInvoker);

  modules_["Timing"] =
      std::make_shared<RSkTimingModule>("Timing", jsInvoker, bridgeInstance);
  modules_["TVNavigationEventEmitter"] =
      std::make_shared<RSkTVNavigationEventEmitter>("TVNavigationEventEmitter",jsInvoker, bridgeInstance);
  modules_["AppState"] =
      std::make_shared<AppStateModule>("AppState", jsInvoker);
  modules_["Networking"] =
      std::make_shared<RSkNetworkingModule>("Networking", jsInvoker, bridgeInstance );
  modules_["WebSocketModule"] =
      std::make_shared<RSkWebSocketModule>("WebSocketModule", jsInvoker, bridgeInstance);
  modules_["KeyboardObserver"] =
      std::make_shared<RSkKeyboardObserver>("KeyboardObserver", jsInvoker, bridgeInstance);
  modules_["DeviceInfo"] =
      std::make_shared<RSkDeviceInfoModule>("DeviceInfo", jsInvoker, bridgeInstance);

  modules_["DevSettings"] =
      std::make_shared<DevSettingsModule>("DevSettings", jsInvoker);
  modules_["DevLoadingView"] =
      std::make_shared<DevLoadingViewModule>("DevLoadingView", jsInvoker);
  modules_["Appearance"] =
      std::make_shared<AppearanceModule>("Appearance", jsInvoker);

  modules_["ImageLoader"] =
      std::make_shared<UnimplementedTurboModule>("ImageLoader", jsInvoker);
  modules_["StatusBarManager"] =
      std::make_shared<UnimplementedTurboModule>("StatusBarManager", jsInvoker);

  modules_["NativeAnimatedModule"] = std::make_shared<UnimplementedTurboModule>(
      "NativeAnimatedModule", jsInvoker);
}

TurboModuleProviderFunctionType JSITurboModuleManager::GetProvider() {
  return [this](
             const std::string &name,
             const jsi::Value *schema) -> std::shared_ptr<TurboModule> {
    if (modules_.find(name) != modules_.end()) {
      return modules_[name];
    }
    return nullptr;
  };
}

} // namespace react
} // namespace facebook
