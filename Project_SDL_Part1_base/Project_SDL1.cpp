
#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>

animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr){ //Initialisation du constructeur de la class animal
    animal_frame_rect_.x = 0;
    animal_frame_rect_.y = 0;
    this->image_ptr_ = IMG_Load(file_path.c_str());
    animal_frame_rect_.h = image_ptr_ -> h;
    animal_frame_rect_.w = image_ptr_ -> w;
    this->window_surface_ptr_ = window_surface_ptr;
}

animal::~animal(){  //Initialisation du destructeur de la classe animal
    SDL_FreeSurface(image_ptr_);
}

void animal::draw() {
    SDL_BlitSurface(this->image_ptr_, NULL, this->window_surface_ptr_, &animal_frame_rect_);
}
// ----- Sheep class -----
sheep::sheep(SDL_Surface* window_surface_ptr)
    : animal("./media/sheep.png",window_surface_ptr){};

sheep::~sheep(){};

void sheep::move(){

        int surface_width = image_ptr_->clip_rect.w; //on récupère la largeur de l'image de l'animal
        int surface_height = image_ptr_->clip_rect.h; //on récupère la hauteur de l'image de l'animal

        // ----- Vérification que la position ne dépasse pas les limite en hauteur ----
        if (animal_frame_rect_.y >= frame_height - frame_boundary - surface_height)
            vertical_direction_ = false;

        if (animal_frame_rect_.y <= frame_boundary)
            vertical_direction_ = true;

        if (animal_frame_rect_.x >= frame_width - frame_boundary - surface_width)
            horizontal_direction_ = false;

        if (animal_frame_rect_.x <= frame_boundary)
            horizontal_direction_ = true;

        if (vertical_direction_)
            animal_frame_rect_.y+=1;
        if (!vertical_direction_)
            animal_frame_rect_.y-=1;

        if (horizontal_direction_)
            animal_frame_rect_.x+=1;

        if (!horizontal_direction_)
            animal_frame_rect_.x-=1;

}
// ---------- Wolf -----------
//wolf(const std::string& file_path, SDL_Surface* window_surface_ptr){
//    this->image_ptr_ = "Project_SDL_Part1_base/media/wolf.png";
//    this->window_surface_ptr_ = window_surface_ptr;
//};

// ---------- Ground -----------
ground::ground(SDL_Surface *window_surface_ptr){
    window_surface_ptr_ = window_surface_ptr;
    SDL_FillRect(window_surface_ptr_,NULL,SDL_MapRGB(window_surface_ptr_->format,255, 0, 0));
}

ground::~ground(){
    SDL_FreeSurface(window_surface_ptr_);
}

void ground::add_animal() {
    animalAdded_.push_back(std::make_shared<sheep>(window_surface_ptr_));
}

void ground::update(){
    SDL_FillRect(window_surface_ptr_,NULL,SDL_MapRGB(window_surface_ptr_->format,255, 0, 0));
    for (const auto& a : animalAdded_) {
        a->move();
        a->draw();
    }
}

// ---------- Application -----------
application::application(unsigned int n_sheep, unsigned int n_wolf)
    : window_ptr_{SDL_CreateWindow("alphae game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,frame_width,frame_height,0)}{
    window_surface_ptr_ = SDL_GetWindowSurface(window_ptr_);
    SDL_FillRect(window_surface_ptr_ ,nullptr, SDL_MapRGB(window_surface_ptr_->format,0,127,0));
    SDL_UpdateWindowSurface(window_ptr_);
};

application::~application(){
    SDL_FreeSurface(window_surface_ptr_);
    SDL_DestroyWindow(this->window_ptr_);

};

int application::loop(unsigned int period) {
    Uint32 currentTime = 0;
    while (period >= currentTime){
        SDL_FillRect(window_surface_ptr_ ,nullptr, SDL_MapRGB(window_surface_ptr_->format,0,127,0));
        groundApp_.update();
        currentTime = SDL_GetTicks();
        SDL_UpdateWindowSurface(window_ptr_);
        SDL_Delay((Uint32)frame_time);
    }
}
// ---------- Init -----------
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
