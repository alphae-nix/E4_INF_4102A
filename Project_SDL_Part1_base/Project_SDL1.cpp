// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>

/*
    Initialisation de SDL + chargement image
    Normalement a ne pas toucher
*/
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

/*
    Définition de certaines fonctions
*/
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

/*
    Mouvement linéaire, Permet de faire bouger les x et y
*/
void constrained_linear_move_(double& x, double& y, double& vx, double& vy){
  // Ajoute a la position actuelle la vitesse x le frame time
  x += (frame_time * vx);
  y += (frame_time * vy);

  // Enforce boundaries || Bord de l'écran
  constexpr double h_m = frame_boundary;
  constexpr double w_m = frame_boundary;
  constexpr double h_M = frame_height - frame_boundary;
  constexpr double w_M = frame_width - frame_boundary;


  /*
    Si la nouvelle position sort des bordures :
        Met la position au niveau de la bordure
        Change la vitesse
  */
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

//mouvement du shpherd limité par la bordure
void constrained_linear_move_key_(double& x, double& y) {
    // Ajoute a la position actuelle la vitesse x le frame time
    

    // Enforce boundaries || Bord de l'écran
    constexpr double h_m = frame_boundary;
    constexpr double w_m = frame_boundary;
    constexpr double h_M = frame_height - frame_boundary;
    constexpr double w_M = frame_width - frame_boundary;
    SDL_Event e;


    /*
      Si la nouvelle position sort des bordures :
          Met la position au niveau de la bordure
          Change la vitesse
    */
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_z:
                y = y + 1;
            case SDLK_s:
                y = y - 1;
            case SDLK_d:
                x = x + 1;
            case SDLK_q:
                x = x + 1;
            }
        }
        

        if (x < w_m) {
            x = w_m;
        }

        if (y < w_m) {
            y = w_m;
        }

        if (x > w_M) {
            x = w_M;
        }

        if (y > h_M) {
            y = h_M;
        }
        
    }


    
}

} // namespace

/////////////////////////////////////////////////////////////////////////////////
// ANIMAL
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe animal
    Param : - file_path, string contenant l'emplacement de l'image
            - window_surface_ptr, pointeur vers la surface
*/
animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr)
  : window_surface_ptr_{window_surface_ptr}, 
    pos_x_{0}, pos_y_{0}, vel_x_{0}, vel_y_{0} //Initialise les positions + vitesse à 0
{

  image_ptr_ = load_surface_for(file_path, window_surface_ptr_);  //Charge l'image
  if (!image_ptr_) //Si l'image n'a pas chargé throw une erreur
    throw std::runtime_error("animal::animal(): "
                             "Could not load " +
                             file_path +
                             "\n Error: " + std::string(SDL_GetError()));
}

/*
    Destructeur de la classe animal
    Free la Surface et remet le pointeur sur null
*/
animal::~animal() {
  SDL_FreeSurface(image_ptr_);
  image_ptr_ = nullptr;
}

/*
    Permet de dessiner l'animal sur la surface
*/
void animal::draw() {
  SDL_Rect pos;
  pos.x = (int) pos_x();
  pos.y = (int) pos_y();
  pos.w = image_ptr_->w;
  pos.h = image_ptr_->h;
  SDL_BlitScaled(image_ptr_, NULL, window_surface_ptr_, &pos);
}


/////////////////////////////////////////////////////////////////////////////////
// HUMAN
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe human
    Param : - file_path, string contenant l'emplacement de l'image
            - window_surface_ptr, pointeur vers la surface
*/

human::human(const std::string& file_path, SDL_Surface* window_surface_ptr)
    : window_surface_ptr_{ window_surface_ptr },
    pos_x_human{ 0 }, pos_y_human{ 0 } //Initialise les positions + vitesse à 0
{

    image_ptr_ = load_surface_for(file_path, window_surface_ptr_);  //Charge l'image
    if (!image_ptr_) //Si l'image n'a pas chargé throw une erreur
        throw std::runtime_error("human::human(): "
            "Could not load " +
            file_path +
            "\n Error: " + std::string(SDL_GetError()));
}

/*
    Destructeur de la classe human
    Free la Surface et remet le pointeur sur null
*/
human::~human() {
    SDL_FreeSurface(image_ptr_);
    image_ptr_ = nullptr;
}

void human::draw() {
    SDL_Rect pos;
    pos.x = (int)pos_x();
    pos.y = (int)pos_y();
    pos.w = image_ptr_->w;
    pos.h = image_ptr_->h;
    SDL_BlitScaled(image_ptr_, NULL, window_surface_ptr_, &pos);
}

/////////////////////////////////////////////////////////////////////////////////
// SHEPHERD
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe shepherd
    Param : - window_surface_ptr, pointeur vers la surface
*/
shepherd::shepherd(SDL_Surface* window_surface_ptr)
    : human("media/shepherd.png", window_surface_ptr) /*Appel le constructeur de animal avec le chemin de l'image*/ {
    // Spawn sheep randomly 
    pos_x() = frame_boundary + std::rand() % (frame_width - 2 * frame_boundary);
    pos_y() = frame_boundary + std::rand() % (frame_height - 2 * frame_boundary);
}

void shepherd::move() {
    constrained_linear_move_key_(pos_x(), pos_y());
}

/////////////////////////////////////////////////////////////////////////////////
// SHEEP
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe sheep
    Param : - window_surface_ptr, pointeur vers la surface
*/
sheep::sheep(SDL_Surface* window_surface_ptr)
  : animal("media/sheep.png", window_surface_ptr) /*Appel le constructeur de animal avec le chemin de l'image*/ {
  // Spawn sheep randomly 
  pos_x() = frame_boundary + std::rand() % (frame_width - 2 * frame_boundary);
  pos_y() = frame_boundary + std::rand() % (frame_height - 2 * frame_boundary);
  vel_x() = 40 - std::rand() % 80;
  vel_y() = 40 - std::rand() % 80;
}

/*
    Bouge le mouton à l'aide de la fonction constrained_linear_move_()
*/
void sheep::move() {
  constrained_linear_move_(pos_x(), pos_y(), vel_x(), vel_y());
}

/////////////////////////////////////////////////////////////////////////////////
// WOLF
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe wolf
    Param : - window_surface_ptr, pointeur vers la surface
*/
wolf::wolf(SDL_Surface* window_surface_ptr)
  : animal("media/wolf.png", window_surface_ptr)  /*Appel le constructeur de animal avec le chemin de l'image*/ {
  // Spawn wolf randomly
  pos_x() = frame_boundary + std::rand() % (frame_width - 2 * frame_boundary);
  pos_y() = frame_boundary + std::rand() % (frame_height - 2 * frame_boundary);
  vel_x() = 40 - std::rand() % 80;
  vel_y() = 40 - std::rand() % 80;
}

/*
    Bouge le loup à l'aide de la fonction constrained_linear_move_()
*/
void wolf::move() {
  constrained_linear_move_(pos_x(), pos_y(), vel_x(), vel_y());
}

/////////////////////////////////////////////////////////////////////////////////
// GROUND
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe ground
    Param : - window_surface_ptr, pointeur vers la surface
*/
ground::ground(SDL_Surface* window_surface_ptr)
  : window_surface_ptr_{window_surface_ptr_} {};

/*
    Set le pointer de window_surface_ptr_ 
    Param : - window_surface_ptr, pointeur vers la surface
*/
void ground::set_ptr(SDL_Surface* window_surface_ptr) {
  window_surface_ptr_ = window_surface_ptr;
}

/*
    Ajoute un nouvel animal dans le vector
*/
void ground::add_animal(const std::shared_ptr<animal>& new_animal) {
  this->all_animals_.push_back(new_animal);
}
/*
    Ajoute un nouvel human dans le vector
*/
void ground::add_human(const std::shared_ptr<human>& new_human) {
    this->all_human_.push_back(new_human);
}

/*
    Update le ground
*/
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

  //Pour chaque animal : Move et Draw
  for (const auto& a : all_animals_) {
    a->move();
    a->draw();
  }
  for (const auto& h : all_human_) {
      h->move();
      h->draw();
  }
}


/////////////////////////////////////////////////////////////////////////////////
// APPLICATION
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe application
    Param : - n_sheep nombre de mouton
            - n_wolf nombre de loups
*/
application::application(unsigned n_sheep, unsigned n_wolf)
  : window_ptr_{nullptr}, window_surface_ptr_ {nullptr}, //Initialise les ptr à null
    window_event_(),  //Initialise le window_event
    g_(), //Initialise le ground
    last_ticks_(SDL_GetTicks()) {

 // Crée une nouvelle fenetre
  window_ptr_ =
      SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, frame_width, frame_height, 0);
  if (!window_ptr_) //Si erreur
    throw std::runtime_error("application::application():" + std::string(SDL_GetError()));


  window_surface_ptr_ = SDL_GetWindowSurface(window_ptr_);

  if (!window_surface_ptr_)//Si erreur
    throw std::runtime_error("application::application():" + std::string(SDL_GetError()));

  //Set le ptr dans ground
  g_.set_ptr(window_surface_ptr_);

    // Add some sheep
    for (unsigned i = 0; i < n_sheep; ++i)
        g_.add_animal(std::make_shared<sheep>(window_surface_ptr_));
    // Add some wolves
    for (unsigned i = 0; i < n_wolf; ++i)
        g_.add_animal(std::make_shared<wolf>(window_surface_ptr_));

    g_.add_human(std::make_shared<shepherd>(window_surface_ptr_));

}

/*
    Destructeur de la classe application
    Libère les surfaces
*/
application::~application() {
  SDL_FreeSurface(window_surface_ptr_);
  SDL_DestroyWindow(window_ptr_);
}

/*
    Boucle principale
*/
int application::loop(unsigned period) {

  //Initialise le nombre de Ticj
  unsigned start_ticks = SDL_GetTicks();
  last_ticks_ = SDL_GetTicks();
  bool keep_window_open = true;

  //Boucle principale, tant que la fenetre est ouverte ou que la période n'est pas dépassé
  while (keep_window_open && ((SDL_GetTicks()-start_ticks) < period*1000)) {
    while (SDL_PollEvent(&window_event_) > 0) {
      switch (window_event_.type) {
      case SDL_QUIT:
        keep_window_open = false;
        break;
      }
    }
    //Clear l'écran + remplis de vert
    SDL_FillRect(window_surface_ptr_, NULL,
                 SDL_MapRGB(window_surface_ptr_->format, 0, 255, 127));
    //Update le ground
    g_.update();
    unsigned nt = SDL_GetTicks();
    if ((nt - last_ticks_) < frame_time * 1000.)
      SDL_Delay((unsigned)((frame_time * 1000.) - (nt - last_ticks_)));
    SDL_UpdateWindowSurface(window_ptr_);
    last_ticks_ = SDL_GetTicks();
  }
  return 0;
}
