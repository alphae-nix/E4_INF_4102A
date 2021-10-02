// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>

animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr){ //Initialisation du constructeur de la class animal
    animal_frame_rect.x = 0;
    animal_frame_rect.y = 0;
    this->image_ptr_ = IMG_Load(file_path.c_str());
    animal_frame_rect.h = image_ptr_ -> h;
    animal_frame_rect.w = image_ptr_ -> w;
    this->window_surface_ptr_ = window_surface_ptr;
}

animal::~animal(){  //Initialisation du destructeur de la classe animal
    SDL_FreeSurface(image_ptr_);
}

void animal::draw() {
    SDL_BlitSurface(this->image_ptr_, NULL, this->window_surface_ptr_, &animal_frame_rect);
}
// ----- Sheep class -----
sheep::sheep(SDL_Surface* window_surface_ptr)
    : animal("./media/sheep.png",window_surface_ptr){};

sheep::move(){
    std::cout << "f;onction move de sheep" ;
}
// ----- Wolf -----
//wolf(const std::string& file_path, SDL_Surface* window_surface_ptr){
//    this->image_ptr_ = "Project_SDL_Part1_base/media/wolf.png";
//    this->window_surface_ptr_ = window_surface_ptr;
//};

application::application(unsigned int n_sheep, unsigned int n_wolf)
    : window_ptr_{SDL_CreateWindow("alphae game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,frame_width,frame_height,0)}{};

application::~application(){
    SDL_FreeSurface(window_surface_ptr_);
    SDL_DestroyWindow(this->window_ptr_);

};

ground::ground(SDL_Surface *window_surface_ptr)
    : window_surface_ptr_ = window_surface_ptr{
    SDL_FillRect(window_surface_ptr_,NULL,SDL_MapRGB(window_surface_ptr_->format,255, 0, 0));
}

void ground::add_animal() {
    animalAdded_.push_back(std::make_shared<animal>());
}
//; ----- Code ------
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

SDL_Surface* load_surface_for(const std::string& path,
                              SDL_Surface* window_surface_ptr) {

  // Helper function to load a png for a specific surface
  // See SDL_ConvertSurface
}
} // namespace
