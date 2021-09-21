/*
* Copyright 2016 Google Inc.
* Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <array>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "ReactSkia/utils/RnsLog.h"
#include "ReactSkia/utils/RnsUtils.h"
#include "layerprops.h"

namespace RnsShell {

class Layer;

enum LayerType {
  LAYER_TYPE_DEFAULT = 0, // Default layer type which which will use component specific APIs to paint.
  LAYER_TYPE_PICTURE, // SkPiture based layer.
  LAYER_TYPE_TEXTURED, // SkTexture based layer.
};

typedef std::vector<std::shared_ptr<Layer> > LayerList;
using SharedLayer = std::shared_ptr<Layer>;

struct PaintContext {
    SkCanvas* canvas;
    std::vector<SkIRect>* damageRect; // Dirty rects in current frame
    const SkRect& dirtyClipBound; // combined clip bounds based on all the dirty rects.
    GrDirectContext* grContext;
};

class Layer {
public:
//    float opacity{1.0};
  /*  float shadowOpacity{1.0};
    float shadowRadius{0};
    int  backfaceVisibility;
    SkSize shadowOffset;
    SkColor shadowColor;
    SkMatrix transform;*/
    static SharedLayer Create(LayerType type = LAYER_TYPE_DEFAULT);
    Layer(LayerType);
    virtual ~Layer() {};

    Layer* rootLayer();
    Layer* parent() { return parent_; }

    LayerType type() { return type_; }
    int layerId() { return layerId_;}

    const LayerList& children() const { return children_; }
    bool needsPainting(PaintContext& context);
    void appendChild(SharedLayer child);
    void insertChild(SharedLayer child, size_t index);
    void removeChild(SharedLayer child, size_t index);

    virtual void paintSelf(PaintContext& context);
    virtual void prePaint(PaintContext& context);
    virtual void paint(PaintContext& context);
    virtual void onPaint(SkCanvas*) {}

    const SkIRect& getFrame() const { return frame_; }
    void setFrame(const SkIRect& frame) { frame_ = frame; }

    const SkIRect& getBounds() const { return bounds_; }
    void setBounds(const SkIRect& bounds) { bounds_ = bounds; }

    const SkPoint& anchorPosition() const { return anchorPosition_; }
    void setAnchorPosition(const SkPoint& anchorPosition) { anchorPosition_ = anchorPosition; }

    const bool masksToBounds() const { return masksToBounds_; }
    void setMasksTotBounds(bool masksToBounds) { masksToBounds_ = masksToBounds; }
//    friend void layerProps(const facebook::react::ShadowView &newShadowView,const facebook::react::ShadowView &oldShadowView);
    const Components_Props getProps() const { return componentsProps_; }
    void setProps(Components_Props props) { componentsProps_ =props; printf("@@@@%f\n\t",componentsProps_.viewProps.opacity); } 
private:
   // float opacity{1.0};
    Components_Props componentsProps_;
    static uint64_t nextUniqueId();

    void setParent(Layer* layer);

    int layerId_;
    Layer *parent_;
    LayerType type_;
    LayerList children_;

    //Layer Geometry
    SkIRect frame_; //The paint bounds should include any transform performed by the layer itself in its parents coordinate space
    SkIRect bounds_; //The paint bounds in its own coordinate space
    SkPoint anchorPosition_; // Position of Layer wrt anchor point in parents coordinate space. This will be used during the transformation.

    //Layer’s Appearance
    bool isHidden_ = { false }; // Wheather layer is hidden
    bool masksToBounds_ = { false }; // Clip childrens
    //Borders & Shadows ?
   // friend void layerProps(const facebook::react::ShadowView &newShadowView,const facebook::react::ShadowView &oldShadowView);
};

}   // namespace RnsShell
