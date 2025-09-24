#pragma once

#include <vector>

// Simple bounding box with normalized coordinates (0..1).
struct BoundingBox {
  float x1;
  float y1;
  float x2;
  float y2;
};

class LiteRTFaceModel {
public:
  // Construct from a .tflite model path.
  explicit LiteRTFaceModel(const char* model_path);
  ~LiteRTFaceModel();

  // Run inference on a BGRA (4 bytes per pixel) image of size in_w x in_h.
  // Returns bounding boxes with normalized coordinates (relative to the image).
  std::vector<BoundingBox> RunOnImage(const unsigned char* bgra_image,
                                      int in_w, int in_h);

  // Non-copyable.
  LiteRTFaceModel(const LiteRTFaceModel&) = delete;
  LiteRTFaceModel& operator=(const LiteRTFaceModel&) = delete;

private:
  struct Impl;
  Impl* impl;  // Defined in litert_model.cc
};

