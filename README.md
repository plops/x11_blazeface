# BlazeFace sample app (LiteRT)

This sample shows how to build and run a minimal C++ program that uses LiteRT to run the BlazeFace face detector. The intended application flow is:

- capture a full-screen X11 screenshot using XShm,
- resize it to 128×128,
- run the BlazeFace model,
- draw bounding boxes for detected faces on the screen without permanently overwriting other screen content.

This repository contains a starter `main.cc` that demonstrates model loading and invocation with dummy input. Replace the dummy input with real XShm capture + preprocessing and add post-processing to decode BlazeFace outputs into boxes/scores and draw them.

## Prerequisites
- Linux with X11 (Xorg).
- Development headers for X11/XShm (e.g. `libx11-dev`, `libxext-dev`).
- Bazel (version compatible with the project).
- A working C++ toolchain (the sample build disables hermetic toolchain by default; see build command below).

## Model
Download the BlazeFace TFLite model (short-range float16 example):
```
wget https://storage.googleapis.com/mediapipe-models/face_detector/blaze_face_short_range/float16/latest/blaze_face_short_range.tflite -O blaze_face.tflite
```

## Build
Use Bazel from the repository root:
```
bazel build //src:blazeface_hermetic --enable_workspace
```

## Run
```
./bazel-bin/litert/samples/blazeface_app/blazeface_app blaze_face_short_range.tflite
```

## Usage
- The program expects a single argument: path to the TFLite model file.
- The provided `main.cc` currently uses a synthetic 1×128×128×3 float input for demonstration and prints output tensor shapes.
- To make this a real app:
  1. Capture the screen using `XShmGetImage` (or `XGetImage`) into an `XImage`.
  2. Resize the image to 128×128 (bilinear or other resampling) and convert to float32 normalized pixels (e.g. [0,1] or model-specific normalization).
  3. Populate the input tensor with the resized data and invoke the interpreter.
  4. Decode the model outputs: BlazeFace typically outputs location/keypoint and score tensors (e.g. [1,896,16] and [1,896,1]) which you must post-process to extract boxes and confidences.
  5. Draw rectangles for high-confidence detections. To avoid permanently modifying the screen, consider:
     - drawing into a transparent overlay window (override-redirect, shaped window, or XComposite overlay), or
     - saving the underlying pixels and restoring them after drawing, or
     - using X11 drawing modes (e.g. XOR) carefully for transient drawing.

## Notes and tips
- If you prefer higher-level image handling and resizing, use OpenCV (`cv::Mat`, `cv::resize`) for simplicity; then convert `cv::Mat` to the input tensor.
- Make sure the tensor layout and normalization match the model's expected input (NHWC float32 in this sample).
- Post-processing (decoding anchors/prior boxes, non-maximum suppression) is required to convert network outputs into usable bounding boxes.
- For performance, reuse XShm and avoid copying where possible; perform in-place normalization when filling the input tensor buffer.

## Where to modify
- `main.cc`: replace the dummy input generation with screen capture + resize + normalization, and add post-processing/drawing code as described above.

## License / Attribution
- The model download link above comes from MediaPipe model storage. Respect the model license when using it.



# LiteRT-LM

- build takes 3min

```
I0000 00:00:1758744856.497065   56310 session_basic.cc:54] Benchmark is enabled.
I0000 00:00:1758744856.497113   56310 threadpool.cc:86] ThreadPool 'engine': Created a worker thread since all 0 worker threads are (supposed to be) busy.
I0000 00:00:1758744856.497116   56310 session_basic.cc:176] RunDecodeAsync
I0000 00:00:1758744856.500380   56314 session_basic.cc:83] PrefillInternal: <start_of_turn>user
What is the tallest building in the world?<end_of_turn>
<start_of_turn>model

I0000 00:00:1758744856.592803   56314 session_basic.cc:117] RunPrefillAsync status: OK
I0000 00:00:1758744856.592869   56314 io_types.cc:317] Inference Done.

As of today (October 26, 2023), the tallest building in the world belongs to **Burj Khalifa** located in Dubai.

Here's why:

*   **Height:** At an impressive height of approximately **828 meters (2721 feet)**
*   **Location:** Dubai
*   **Type:** This was an architectural marvel designed by Andre Juergen Juergens

While other buildings have reached impressive heights (like the Shanghai Tower), Burj Khalifa holds its position as currently the tallest structure!I0000 00:00:1758744860.457236   56314 io_types.cc:317] Inference Done.

I0000 00:00:1758744860.457295   56310 litert_lm_main.cc:193] BenchmarkInfo:
  Init Phases (2):
    - Executor initialization: 212.03 ms
    - Tokenizer initialization: 0.00 ms
    Total init time: 212.03 ms
--------------------------------------------------
  Prefill Turns (Total: 1):
    Prefill Turn 1: Processed 18 tokens in 92.41119ms duration.
      Prefill Speed: 194.78 tokens/sec.
--------------------------------------------------
  Decode Turns (Total: 1):
    Decode Turn 1: Processed 115 tokens in 3.86435584s duration.
      Decode Speed: 29.76 tokens/sec.
--------------------------------------------------
--------------------------------------------------

I0000 00:00:1758744860.457492   56310 threadpool.cc:46] ThreadPool 'engine': Shutting down...
I0000 00:00:1758744860.457505   56314 threadpool.cc:157] ThreadPool 'engine': Worker thread stopped.
I0000 00:00:1758744860.457554   56310 threadpool.cc:65] ThreadPool 'engine': Shutdown complete. 

```
