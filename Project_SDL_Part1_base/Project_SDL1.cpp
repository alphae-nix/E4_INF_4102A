// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>

void init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
    throw std::runtime_error("init():" + std::string(SDL_GetError()));

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
    throw std::runtime_error("init(): SDL_image could not initialize! "
                             "SDL_image Error: " +
                             std::string(IMG_GetError()));
}

namespace {
// Defining a namespace without a name -> Anonymous workspace
// Its purpose is to indicate to the compiler that everything
// inside of it is UNIQUELY used within this source file.

// Helper function to load a png for a specific surface
// See SDL_ConvertSurface

SDL_Surface* load_surface_for(const std::string& path,
                              SDL_Surface* window_surface_ptr) {
  // The final optimized image
  SDL_Surface* optimizedSurface = NULL;

  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (!loadedSurface)
    throw std::runtime_error("load_surface_for() :"
                             "Unable to load image " +
                             path + ".\n Error: " + IMG_GetError());

  // Convert surface to screen format
  optimizedSurface =
      SDL_ConvertSurface(loadedSurface, window_surface_ptr->format, 0);
  //  optimizedSurface = SDL_ConvertSurfaceFormat(loadedSurface,
  //  SDL_GetWindowPixelFormat(window_ptr), 0);
  if (!optimizedSurface)
    throw std::runtime_error("load_surface_for(): Unable to optimize image," +
                             std::string(SDL_GetError()));

  // Get rid of old loaded surface
  SDL_FreeSurface(loadedSurface);

  return optimizedSurface;
}

void constrained_linear_move_(double& x, double& y, double& vx, double& vy){
  x += (frame_time * vx);
  y += (frame_time * vy);

  // Enforce boundaries
  constexpr double h_m = frame_boundary;
  constexpr double w_m = frame_boundary;
  constexpr double h_M = frame_height - frame_boundary;
  constexpr double w_M = frame_width - frame_boundary;

  if (x < w_m) {
    x = w_m;
    vx = std::abs(vx);
  }

  if (y < w_m) {
    y = w_m;
    vy = std::abs(vy);
  }

  if (x > w_M) {
    x = w_M;
    vx = -std::abs(vx);
  }

  if (y > h_M) {
    y = h_M;
    vy = -std::abs(vy);
  }
}

} // namespace

/////////////////////////////////////////////////////////////////////////////////
// ANIMAL
/////////////////////////////////////////////////////////////////////////////////

animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr)
  : window_surface_ptr_{window_surface_ptr}, 
    pos_x_{0}, pos_y_{0}, vel_x_{0}, vel_y_{0} {

  image_ptr_ = load_surface_for(file_path, window_surface_ptr_);
  if (!image_ptr_)
    throw std::runtime_error("animal::animal(): "
                             "Could not load " +
                             file_path +
                             "\n Error: " + std::string(SDL_GetError()));
}

animal::~animal() {
  SDL_FreeSurface(image_ptr_);
  image_ptr_ = nullptr;
}

void animal::draw() {
  SDL_Rect pos;
  pos.x = (int) pos_x();
  pos.y = (int) pos_y();
  pos.w = image_ptr_->w;
  pos.h = image_ptr_->h;
  SDL_BlitScaled(image_ptr_, NULL, window_surface_ptr_, &pos);
}

/////////////////////////////////////////////////////////////////////////////////
// SHEEP
/////////////////////////////////////////////////////////////////////////////////

sheep::sheep(SDL_Surface* window_surface_ptr)
  : animal("/home/alphae/Cours/C++/E4_INF_4102A/media/sheep.png", window_surface_ptr) {
  // Spawn sheep randomly
  pos_x() = frame_boundary + std::rand() % (frame_width - 2 * frame_boundary);
  pos_y() = frame_boundary + std::rand() % (frame_height - 2 * frame_boundary);
  vel_x() = 40 - std::rand() % 80;
  vel_y() = 40 - std::rand() % 80;
}

void sheep::move() {
  constrained_linear_move_(pos_x(), pos_y(), vel_x(), vel_y());
}

/////////////////////////////////////////////////////////////////////////////////
// WOLF
/////////////////////////////////////////////////////////////////////////////////

wolf::wolf(SDL_Surface* window_surface_ptr)
  : animal("/home/alphae/Cours/C++/E4_INF_4102A/media/wolf.png", window_surface_ptr) {
  // Spawn wolf randomly
  pos_x() = frame_boundary + std::rand() % (frame_width - 2 * frame_boundary);
  pos_y() = frame_boundary + std::rand() % (frame_height - 2 * frame_boundary);
  vel_x() = 40 - std::rand() % 80;
  vel_y() = 40 - std::rand() % 80;
}

void wolf::move() {
  constrained_linear_move_(pos_x(), pos_y(), vel_x(), vel_y());
}

/////////////////////////////////////////////////////////////////////////////////
// GROUND
/////////////////////////////////////////////////////////////////////////////////

ground::ground(SDL_Surface* window_surface_ptr)
  : window_surface_ptr_{window_surface_ptr_} {};

void 
ground::set_ptr(SDL_Surface* window_surface_ptr) {
  window_surface_ptr_ = window_surface_ptr;
}

void ground::add_animal(const std::shared_ptr<animal>& new_animal) {
  all_animals_.push_back(new_animal);
}

void ground::update() {
  // Idee pour proj final
  /*
  for (const auto& a : all_animals_) {
    if (!a->get_prop("alive"))
      continue;
    for (const auto& b : all_animals_) {
      if (a == b)
        continue;
      if (!b->get_prop("alive"))
        continue;
      a->interacts(b);
    }
  }
  //Effacacage de tous les animaux mort
  */
  // Suffisant pour projet 1
  for (const auto& a : all_animals_) {
    a->move();
    a->draw();
  }
}


/////////////////////////////////////////////////////////////////////////////////
// APPLICATION
/////////////////////////////////////////////////////////////////////////////////

application::application(unsigned n_sheep, unsigned n_wolf)
  : window_ptr_{nullptr}, window_surface_ptr_ {nullptr},
    window_event_(), 
    g_(),
    last_ticks_(SDL_GetTicks()) {

  window_ptr_ =
      SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, frame_width, frame_height, 0);
  if (!window_ptr_)
    throw std::runtime_error("application::application():" + std::string(SDL_GetError()));

  window_surface_ptr_ = SDL_GetWindowSurface(window_ptr_);

  if (!window_surface_ptr_)
    throw std::runtime_error("application::application():" + std::string(SDL_GetError()));

  g_.set_ptr(window_surface_ptr_);

  // Add some sheep
  for (unsigned i = 0; i < n_sheep; ++i)
    g_.add_animal(std::make_shared<sheep>(window_surface_ptr_));
  // Add some wolves
  for (unsigned i = 0; i < n_wolf; ++i)
    g_.add_animal(std::make_shared<wolf>(window_surface_ptr_));
}

application::~application() {
  SDL_FreeSurface(window_surface_ptr_);
  SDL_DestroyWindow(window_ptr_);
}

int application::loop(unsigned period) {

  unsigned start_ticks = SDL_GetTicks();
  last_ticks_ = SDL_GetTicks();
  bool keep_window_open = true;
  while (keep_window_open && ((SDL_GetTicks()-start_ticks) < period*1000)) {
    while (SDL_PollEvent(&window_event_) > 0) {
      switch (window_event_.type) {
      case SDL_QUIT:
        keep_window_open = false;
        break;
      }
    }

    SDL_FillRect(window_surface_ptr_, NULL,
                 SDL_MapRGB(window_surface_ptr_->format, 0, 255, 127));
    g_.update();
    unsigned nt = SDL_GetTicks();
    if ((nt - last_ticks_) < frame_time * 1000.)
      SDL_Delay((unsigned)((frame_time * 1000.) - (nt - last_ticks_)));
    SDL_UpdateWindowSurface(window_ptr_);
    last_ticks_ = SDL_GetTicks();
  }
  return 0;
}
