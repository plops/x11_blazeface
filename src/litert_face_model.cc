#include "litert_face_model.h"  // changed: use new unique header name

// Include LiteRT headers only in this translation unit.
#include "absl/types/span.h" // Needed for writing to LiteRT buffers
#include "litert/cc/litert_compiled_model.h"
#include "litert/cc/litert_environment.h"
#include "litert/cc/litert_macros.h"
#include "litert/cc/litert_model.h"

#include <memory>
#include <vector>
// #include <stdexcept>
#include <algorithm>
#include <optional>

struct LiteRTFaceModel::Impl {
  explicit Impl() = default;
  // Use std::optional for move-only / non-default-constructible LiteRT types.
  // std::optional<litert::Environment> env;
  // std::optional<litert::Model> model;
  // std::optional<litert::CompiledModel> compiled_model;
  // std::vector<litert::TensorBuffer> input_buffers;
  // std::vector<litert::TensorBuffer> output_buffers;
};
//
// static void preprocess_to_rgb_normalized(std::vector<float> &out,
//                                          const unsigned char *in_bgra,
//                                          int in_w, int in_h, int out_w,
//                                          int out_h) {
//   float x_ratio = static_cast<float>(in_w) / out_w;
//   float y_ratio = static_cast<float>(in_h) / out_h;
//   for (int y = 0; y < out_h; ++y) {
//     for (int x = 0; x < out_w; ++x) {
//       int px = static_cast<int>(x_ratio * x);
//       int py = static_cast<int>(y_ratio * y);
//       const unsigned char *pixel = &in_bgra[(py * in_w + px) * 4]; // BGRA
//       out[(y * out_w + x) * 3 + 0] = (pixel[2] / 255.0f - 0.5f) * 2.0f; // R
//       out[(y * out_w + x) * 3 + 1] = (pixel[1] / 255.0f - 0.5f) * 2.0f; // G
//       out[(y * out_w + x) * 3 + 2] = (pixel[0] / 255.0f - 0.5f) * 2.0f; // B
//     }
//   }
// }

LiteRTFaceModel::LiteRTFaceModel(const char *model_path) {
  impl = new Impl();
  //
  // // Create environment, model and compiled_model using the macros and emplace into optionals.
  // LITERT_ASSIGN_OR_ABORT(auto env_val, litert::Environment::Create({}));
  // impl->env.emplace(std::move(env_val));
  //
  // LITERT_ASSIGN_OR_ABORT(auto model_val, litert::Model::CreateFromFile(model_path));
  // impl->model.emplace(std::move(model_val));
  //
  // LITERT_ASSIGN_OR_ABORT(auto compiled_val,
  //                        litert::CompiledModel::Create(*impl->env, *impl->model,
  //                                                      kLiteRtHwAcceleratorCpu));
  // impl->compiled_model.emplace(std::move(compiled_val));
  //
  // // Create and move input/output buffers into the impl
  // LITERT_ASSIGN_OR_ABORT(auto in_bufs, impl->compiled_model->CreateInputBuffers());
  // impl->input_buffers = std::move(in_bufs);
  //
  // LITERT_ASSIGN_OR_ABORT(auto out_bufs, impl->compiled_model->CreateOutputBuffers());
  // impl->output_buffers = std::move(out_bufs);
}

LiteRTFaceModel::~LiteRTFaceModel() {
  delete impl;
}

std::vector<BoundingBox> LiteRTFaceModel::RunOnImage(
  const unsigned char *bgra_image, int in_w, int in_h) {
  // const int MODEL_WIDTH = 128;
  // const int MODEL_HEIGHT = 128;
  //
  // std::vector<float> preprocessed(MODEL_WIDTH * MODEL_HEIGHT * 3);
  // preprocess_to_rgb_normalized(preprocessed, bgra_image, in_w, in_h,
  //                              MODEL_WIDTH, MODEL_HEIGHT);
  //
  // std::vector<float> scores_data(896);     // [1, 896, 1] -> 896
  // std::vector<float> boxes_data(896 * 16); // [1, 896, 16] -> 14336
  //
  // // Write input (explicit template argument) and run via optional-held compiled_model.
  // LITERT_ABORT_IF_ERROR(
  //     impl->input_buffers[0].Write<float>(absl::MakeConstSpan(preprocessed)));
  // LITERT_ABORT_IF_ERROR(impl->compiled_model->Run(impl->input_buffers,
  //                                                 impl->output_buffers));
  // LITERT_ABORT_IF_ERROR(
  //     impl->output_buffers[0].Read<float>(absl::MakeSpan(scores_data)));
  // LITERT_ABORT_IF_ERROR(
  //     impl->output_buffers[1].Read<float>(absl::MakeSpan(boxes_data)));
  //
  // // Postprocess -> return normalized boxes in [0,1] relative to the image.
  std::vector<BoundingBox> faces;
  // const float score_threshold = 0.75f;
  // for (int i = 0; i < 896; ++i) {
  //   if (scores_data[i] > score_threshold) {
  //     float y_center = boxes_data[i * 16 + 0];
  //     float x_center = boxes_data[i * 16 + 1];
  //     float h = boxes_data[i * 16 + 2];
  //     float w = boxes_data[i * 16 + 3];
  //
  //     float x1 = x_center - w / 2.0f;
  //     float y1 = y_center - h / 2.0f;
  //     float x2 = x_center + w / 2.0f;
  //     float y2 = y_center + h / 2.0f;
  //
  //     // Clamp to [0,1]
  //     x1 = std::max(0.0f, std::min(1.0f, x1));
  //     y1 = std::max(0.0f, std::min(1.0f, y1));
  //     x2 = std::max(0.0f, std::min(1.0f, x2));
  //     y2 = std::max(0.0f, std::min(1.0f, y2));
  //
  //     faces.push_back({x1, y1, x2, y2});
  //   }
  // }
  return faces;
}
