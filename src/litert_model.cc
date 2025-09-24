#include "litert_model.h"

// Include LiteRT headers only in this translation unit.
#include "absl/types/span.h" // Needed for writing to LiteRT buffers
#include "litert/cc/litert_compiled_model.h"
#include "litert/cc/litert_environment.h"
#include "litert/cc/litert_macros.h"
#include "litert/cc/litert_model.h"

#include <memory>
#include <vector>
#include <stdexcept>
#include <algorithm>

struct LiteRTFaceModel::Impl {
  std::unique_ptr<litert::Environment> env;
  std::unique_ptr<litert::Model> model;
  std::unique_ptr<litert::CompiledModel> compiled_model;
  std::vector<litert::Buffer> input_buffers;
  std::vector<litert::Buffer> output_buffers;
};

static void preprocess_to_rgb_normalized(std::vector<float> &out,
                                         const unsigned char *in_bgra,
                                         int in_w, int in_h, int out_w,
                                         int out_h) {
  float x_ratio = static_cast<float>(in_w) / out_w;
  float y_ratio = static_cast<float>(in_h) / out_h;
  for (int y = 0; y < out_h; ++y) {
    for (int x = 0; x < out_w; ++x) {
      int px = static_cast<int>(x_ratio * x);
      int py = static_cast<int>(y_ratio * y);
      const unsigned char *pixel = &in_bgra[(py * in_w + px) * 4]; // BGRA
      out[(y * out_w + x) * 3 + 0] = (pixel[2] / 255.0f - 0.5f) * 2.0f; // R
      out[(y * out_w + x) * 3 + 1] = (pixel[1] / 255.0f - 0.5f) * 2.0f; // G
      out[(y * out_w + x) * 3 + 2] = (pixel[0] / 255.0f - 0.5f) * 2.0f; // B
    }
  }
}

LiteRTFaceModel::LiteRTFaceModel(const char *model_path) {
  impl = new Impl();

  LITERT_ASSIGN_OR_THROW(impl->env, litert::Environment::Create({}));
  LITERT_ASSIGN_OR_THROW(impl->model, litert::Model::CreateFromFile(model_path));
  LITERT_ASSIGN_OR_THROW(
      impl->compiled_model,
      litert::CompiledModel::Create(impl->env.get(), impl->model.get()));
  LITERT_ASSIGN_OR_THROW(impl->input_buffers,
                         impl->compiled_model->CreateInputBuffers());
  LITERT_ASSIGN_OR_THROW(impl->output_buffers,
                         impl->compiled_model->CreateOutputBuffers());
}

LiteRTFaceModel::~LiteRTFaceModel() {
  delete impl;
}

std::vector<BoundingBox> LiteRTFaceModel::RunOnImage(
    const unsigned char *bgra_image, int in_w, int in_h) {
  const int MODEL_WIDTH = 128;
  const int MODEL_HEIGHT = 128;

  std::vector<float> preprocessed(MODEL_WIDTH * MODEL_HEIGHT * 3);
  preprocess_to_rgb_normalized(preprocessed, bgra_image, in_w, in_h,
                               MODEL_WIDTH, MODEL_HEIGHT);

  std::vector<float> scores_data(896);     // [1, 896, 1] -> 896
  std::vector<float> boxes_data(896 * 16); // [1, 896, 16] -> 14336

  LITERT_THROW_IF_ERROR(
      impl->input_buffers[0].Write(absl::MakeConstSpan(preprocessed)));
  LITERT_THROW_IF_ERROR(impl->compiled_model->Run(impl->input_buffers,
                                                  impl->output_buffers));
  LITERT_THROW_IF_ERROR(
      impl->output_buffers[0].Read(absl::MakeSpan(scores_data)));
  LITERT_THROW_IF_ERROR(
      impl->output_buffers[1].Read(absl::MakeSpan(boxes_data)));

  // Postprocess -> return normalized boxes in [0,1] relative to the image.
  std::vector<BoundingBox> faces;
  const float score_threshold = 0.75f;
  for (int i = 0; i < 896; ++i) {
    if (scores_data[i] > score_threshold) {
      float y_center = boxes_data[i * 16 + 0];
      float x_center = boxes_data[i * 16 + 1];
      float h = boxes_data[i * 16 + 2];
      float w = boxes_data[i * 16 + 3];

      float x1 = x_center - w / 2.0f;
      float y1 = y_center - h / 2.0f;
      float x2 = x_center + w / 2.0f;
      float y2 = y_center + h / 2.0f;

      // Clamp to [0,1]
      x1 = std::max(0.0f, std::min(1.0f, x1));
      y1 = std::max(0.0f, std::min(1.0f, y1));
      x2 = std::max(0.0f, std::min(1.0f, x2));
      y2 = std::max(0.0f, std::min(1.0f, y2));

      faces.push_back({x1, y1, x2, y2});
    }
  }
  return faces;
}

