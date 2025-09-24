#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <thread>
#include <vector>

#include "litert_model.h"

// A simple structure to hold bounding box data.
// (Now provided by litert_model.h)
// struct BoundingBox { float x1,y1,x2,y2; };

// Manages the X11 overlay window for drawing face boxes.
class FaceOverlay {
public:
  FaceOverlay(Display *display) : display_(display) {
    screen_ = DefaultScreen(display_);
    root_ = RootWindow(display_, screen_);
    screen_width_ = DisplayWidth(display_, screen_);
    screen_height_ = DisplayHeight(display_, screen_);

    XVisualInfo vinfo;
    if (!XMatchVisualInfo(display_, screen_, 32, TrueColor, &vinfo)) {
      throw std::runtime_error("No 32-bit visual found for transparency.");
    }
    Visual *visual = vinfo.visual;
    int depth = vinfo.depth;

    XSetWindowAttributes attrs;
    std::memset(&attrs, 0, sizeof(attrs));
    attrs.colormap = XCreateColormap(display_, root_, visual, AllocNone);
    attrs.border_pixel = 0;
    attrs.background_pixel = 0;
    attrs.override_redirect = True;

    window_ =
        XCreateWindow(display_, root_, 0, 0, screen_width_, screen_height_, 0,
                      depth, InputOutput, visual,
                      CWColormap | CWBorderPixel | CWOverrideRedirect, &attrs);

    if (!window_)
      throw std::runtime_error("Failed to create overlay window");

    XserverRegion region = XFixesCreateRegion(display_, nullptr, 0);
    XFixesSetWindowShapeRegion(display_, window_, ShapeInput, 0, 0, region);
    XFixesDestroyRegion(display_, region);

    XMapRaised(display_, window_);
    XFlush(display_);

    gc_ = XCreateGC(display_, window_, 0, nullptr);
    if (!gc_)
      throw std::runtime_error("Failed to create GC");
    XSetForeground(display_, gc_, 0xFFFF0000); // Red
    XSetLineAttributes(display_, gc_, 2, LineSolid, CapButt, JoinMiter);
  }

  ~FaceOverlay() {
    if (gc_)
      XFreeGC(display_, gc_);
    if (window_)
      XDestroyWindow(display_, window_);
  }

  void draw_faces(const std::vector<BoundingBox> &faces) {
    XClearWindow(display_, window_);
    for (const auto &box : faces) {
      // boxes are in screen coordinates here.
      XDrawRectangle(display_, window_, gc_, static_cast<int>(box.x1),
                     static_cast<int>(box.y1),
                     static_cast<int>(box.x2 - box.x1),
                     static_cast<int>(box.y2 - box.y1));
    }
    XFlush(display_);
    XRaiseWindow(display_, window_);
    XFlush(display_);
  }

private:
  Display *display_;
  int screen_;
  Window root_;
  Window window_;
  GC gc_;
  int screen_width_;
  int screen_height_;
};

// Main application: X11 capture + call LiteRTFaceModel.
int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <path_to_blazeface.tflite>"
              << std::endl;
    return 1;
  }

  // --- X11 Setup ---
  Display *display = XOpenDisplay(nullptr);
  if (!display) {
    std::cerr << "Failed to open X display." << std::endl;
    return 1;
  }
  if (!XShmQueryExtension(display)) {
    std::cerr << "XShm extension not available." << std::endl;
    XCloseDisplay(display);
    return 1;
  }

  int screen_width = DisplayWidth(display, DefaultScreen(display));
  int screen_height = DisplayHeight(display, DefaultScreen(display));

  XImage *ximage = XShmCreateImage(display, DefaultVisual(display, 0),
                                   DefaultDepth(display, 0), ZPixmap, nullptr,
                                   /*shminfo*/ nullptr, screen_width,
                                   screen_height);
  XShmSegmentInfo shminfo;
  shminfo.shmid =
      shmget(IPC_PRIVATE, ximage->bytes_per_line * ximage->height,
             IPC_CREAT | 0777);
  shminfo.shmaddr = ximage->data = (char *)shmat(shminfo.shmid, 0, 0);
  shminfo.readOnly = False;
  XShmAttach(display, &shminfo);

  // --- LiteRT model (shared library) ---
  LiteRTFaceModel model(argv[1]);

  // --- Overlay Setup ---
  std::unique_ptr<FaceOverlay> overlay =
      std::make_unique<FaceOverlay>(display);

  try {
    while (true) {
      XShmGetImage(display, RootWindow(display, 0), ximage, 0, 0, 0xFFFFFFFF);

      // Call into the shared LiteRT library, which returns normalized boxes.
      auto normalized_faces =
          model.RunOnImage(reinterpret_cast<unsigned char *>(ximage->data),
                           screen_width, screen_height);

      // Convert normalized boxes to screen coordinates.
      std::vector<BoundingBox> screen_faces;
      screen_faces.reserve(normalized_faces.size());
      for (const auto &b : normalized_faces) {
        screen_faces.push_back({b.x1 * screen_width, b.y1 * screen_height,
                                b.x2 * screen_width, b.y2 * screen_height});
      }

      overlay->draw_faces(screen_faces);

      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    XShmDetach(display, &shminfo);
    XDestroyImage(ximage);
    shmdt(shminfo.shmaddr);
    shmctl(shminfo.shmid, IPC_RMID, 0);
    XCloseDisplay(display);
    return 1;
  }

  // Cleanup (never reached in normal loop).
  XShmDetach(display, &shminfo);
  XDestroyImage(ximage);
  shmdt(shminfo.shmaddr);
  shmctl(shminfo.shmid, IPC_RMID, 0);
  XCloseDisplay(display);
  return 0;
}
