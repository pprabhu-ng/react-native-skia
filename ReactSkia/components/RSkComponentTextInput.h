/*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "modules/skparagraph/include/TextStyle.h"
#include "react/renderer/components/textinput/TextInputShadowNode.h"
#include "react/renderer/components/textinput/TextInputEventEmitter.h"
#include "ReactSkia/components/RSkComponent.h"
#include "ReactSkia/textlayoutmanager/RSkTextLayoutManager.h"


namespace facebook {
namespace react {
struct cursor{
  int locationFromEnd;//Relative position from end
  int end;// lentgh of the string
};
class RSkComponentTextInput final : public RSkComponent {
 public:
  RSkComponentTextInput(const ShadowView &shadowView);
  void onHandleKey(rnsKey  eventKeyType,bool *stopPropagate)override;
  RnsShell::LayerInvalidateMask updateComponentProps(const ShadowView &newShadowView,bool forceUpdate) override;
  /*
    TODO  Need to Add command function to Handle Command.
  */
  void handleCommand(std::string commandName,folly::dynamic args)override;
 protected:
  void OnPaint(SkCanvas *canvas) override;
 private:
  bool isInEditingMode_;
  bool editable_ = true;
  bool caretHidden_ = false;
  bool isTextInputInFocus_=false;
  bool secureTextEntry_=false;
  int eventCount_;
  int maxLength_;
  std::string displayString_{}; // Text to be displayed on screen
  std::string placeholderString_{}; // Placeholder Text
  SharedColor placeholderColor_;  // Placeholder Text Color
  SharedColor selectionColor_;
  struct cursor cursor_;
  SkPaint cursorPaint_;
  std::shared_ptr<skia::textlayout::Paragraph> paragraph_;
  void flushLayer();
  void drawTextInput(
      skia::textlayout::ParagraphStyle &paraStyle,
      SkCanvas *canvas,
      LayoutMetrics layout,
      std::shared_ptr<skia::textlayout::ParagraphBuilder> &builder,
      const TextInputProps& props);
  void processEventKey(rnsKey eventKeyType,bool* stopPropagation,bool *waitForupdateProps, bool updateString);
  void keyEventProcessingThread();
};

} // namespace react
} // namespace facebook