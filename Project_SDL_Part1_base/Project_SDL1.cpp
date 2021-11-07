// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>
#include <iostream>
#include <stdlib.h>

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
    //std::srand(std::time(nullptr)); //Nouvelle seed random
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
    void constrained_linear_move_(double& x, double& y, double& vx, double& vy) {
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
void  constrained_linear_move_dog(double& x, double& y, double& vx, double& vy, double& x_human, double& y_human) {
    // Ajoute a la position actuelle la vitesse x le frame time
    x += (frame_time * vx);
    y += (frame_time * vy);

    // Enforce boundaries || Bord de l'écran
    constexpr double h_m = frame_boundary;
    constexpr double w_m = frame_boundary;
    constexpr double h_M = frame_height - frame_boundary;
    constexpr double w_M = frame_width - frame_boundary;
    double _x_shepherd = x_human - 40;
    double x_shepherd = x_human + 80;
    double _y_shepherd = y_human - 40;
    double y_shepherd = y_human + 80;


    /*
      Si la nouvelle position sort des bordures :
          Met la position au niveau de la bordure
          Change la vitesse
    */
    
    if (x < _x_shepherd) {
        x = _x_shepherd;
        vx = std::abs(vx);
    }
    if (x > x_shepherd) {
        x = x_shepherd;
        vx = std::abs(vx);
    }
    if (y < _y_shepherd) {
        y = _y_shepherd;
        vx = std::abs(vx);
    }
    if (y > y_shepherd) {
        y = y_shepherd;
        vx = std::abs(vx);
    }
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

/*
* Retourn True si l'animal n'est plus vivant
*/
bool is_dead(std::shared_ptr<moving_object> a) {
    return !a->alive();
}

    //mouvement du shpherd limité par la bordure
    void constrained_linear_move_key_(double& x, double& y) {


        // Enforce boundaries || Bord de l'écran
        constexpr double h_m = frame_boundary;
        constexpr double w_m = frame_boundary;
        constexpr double h_M = frame_height - frame_boundary;
        constexpr double w_M = frame_width - frame_boundary;

        //Vitesse de déplacement
        unsigned speed = 5;

        //Récupère dans un vector l'état de toute les touches
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        //Si la touche Z (W dans le code car SDL inverse les deux), monte le personnage
        if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP])
        {
            y -= speed;
        }
        if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN])
        {
            y += speed;
        }
        if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT])
        {
            x -= speed;
        }
        if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT])
        {
            x += speed;
        }

        //Si les x et y dépassent du bord 
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

    /*
    * Permet de calculer le score avec le vector passé en paramètre
    * Chaque mouton permet d'augmenter le score de 10 points
    */
    unsigned Score(std::vector<std::shared_ptr<moving_object>>& all) {
        unsigned score = 0;
        for (const auto& animal : all) {
            if (animal->get_prop("sheep")) {
                score += 10;
            }
        }
        return score;
    }

    /*
    * Fonction pour dessiner le score sur l'écran
    * Appel la fonction Score() pour calculer le score
    */
    void Draw_Score(std::vector<std::shared_ptr<moving_object>>& all, SDL_Surface* window_surface_ptr) {
        unsigned score = Score(all);
        std::string score_text = "Score: " + std::to_string(score);
        //SDL_Color textColor = { 255, 255, 255, 0 };
       // SDL_Surface* surface = font(font, score_text, textColor);
    }
} // namespace

/////////////////////////////////////////////////////////////////////////////////
// interacting_object
/////////////////////////////////////////////////////////////////////////////////

/*
* Ctor
*/
interacting_object::interacting_object(std::string s) :
    type_{ s }, female_{ false }, alive_{ true }, prey_{ false } {}

/*
* Dtor
*/
interacting_object::~interacting_object(){}

/*
* Retour un boolean en fonction de la String passé en paramètre
*/
bool interacting_object::get_prop(std::string s) {
    //Retourne pour chaque cas les bool des propriétés
    if (s == "alive") {
        return this->alive_;
    }
    else if (s == "prey") {
        return this->prey_;
    }
    else if (s == "female") {
        return this->female_;
    }
    else { //Si la string ne correspond à aucune propriété, retourne true si le type (sheep/wolf...) est égal a s
        return s == this->type_;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// rendered_object
/////////////////////////////////////////////////////////////////////////////////

/*
* Ctor
*/
rendered_object::rendered_object(const std::string& file_path, SDL_Surface* window_surface_ptr, ground* g, std::string s)
    : interacting_object(s), window_surface_ptr_{ window_surface_ptr }, pos_x_{0}, pos_y_{0}, g_{g}
{
    image_ptr_ = load_surface_for(file_path, window_surface_ptr_);  //Charge l'image
    if (!image_ptr_) //Si l'image n'a pas chargé throw une erreur
        throw std::runtime_error("animal::animal(): "
            "Could not load " +
            file_path +
            "\n Error: " + std::string(SDL_GetError()));
}
    
/*
* Dtor
*/
rendered_object::~rendered_object() {
    SDL_FreeSurface(image_ptr_);
    image_ptr_ = nullptr;
}

/*
    Permet de dessiner l'animal sur la surface
*/
void rendered_object::draw() {
    SDL_Rect pos;
    pos.x = (int)pos_x();
    pos.y = (int)pos_y();
    pos.w = image_ptr_->w;
    pos.h = image_ptr_->h;
    SDL_BlitScaled(image_ptr_, NULL, window_surface_ptr_, &pos);
}

/////////////////////////////////////////////////////////////////////////////////
// moving_object
/////////////////////////////////////////////////////////////////////////////////

/*
* Ctor
*/
moving_object::moving_object(const std::string& file_path, SDL_Surface* window_surface_ptr, ground* g, std::string s)
    : rendered_object(file_path, window_surface_ptr, g, s), vel_x_{ 0 }, vel_y_{ 0 }, timer_{ 0 }
{}

/*
* Dtor
*/
moving_object::~moving_object() {}


/////////////////////////////////////////////////////////////////////////////////
// ANIMAL
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe animal
    Param : - file_path, string contenant l'emplacement de l'image
            - window_surface_ptr, pointeur vers la surface
*/
animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr, ground* g, std::string s)
    : moving_object{ file_path, window_surface_ptr, g, s }
{}

/*
 *   Destructeur de la classe animal
 *   Free la Surface et remet le pointeur sur null
*/
animal::~animal() {}


/////////////////////////////////////////////////////////////////////////////////
// playable_character
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe playable_character
    Param : - file_path, string contenant l'emplacement de l'image
            - window_surface_ptr, pointeur vers la surface
*/
playable_character::playable_character(const std::string& file_path, SDL_Surface* window_surface_ptr, ground* g, std::string s)
    : moving_object{ file_path, window_surface_ptr, g, s}
{}

/*
    Destructeur de la classe human
    Free la Surface et remet le pointeur sur null
*/
playable_character::~playable_character() {}

void playable_character::move() {
    constrained_linear_move_key_(pos_x(), pos_y());
}

/////////////////////////////////////////////////////////////////////////////////
// SHEPHERD
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe shepherd
    Param : - window_surface_ptr, pointeur vers la surface
*/
shepherd::shepherd(SDL_Surface* window_surface_ptr, ground* g)
    : playable_character("media\\shepherd.png", window_surface_ptr, g, "shepherd") /*Appel le constructeur de animal avec le chemin de l'image*/ {
    // Spawn shepherd randomly 
    pos_x() = frame_boundary + std::rand() % (frame_width - 2 * frame_boundary);
    pos_y() = frame_boundary + std::rand() % (frame_height - 2 * frame_boundary);
}

shepherd::~shepherd(){}

void shepherd::interacts(std::shared_ptr<moving_object> a) {}

/////////////////////////////////////////////////////////////////////////////////
// SHEEP
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe sheep
    Param : - window_surface_ptr, pointeur vers la surface
*/
sheep::sheep(SDL_Surface* window_surface_ptr, ground* g, double x, double y)
    : animal("media\\sheep.png", window_surface_ptr, g, "sheep") /*Appel le constructeur de animal avec le chemin de l'image*/ {
    // Spawn sheep randomly 
    pos_x() = x;
    pos_y() = y;
    vel_x() = 40 - std::rand() % 80;
    vel_y() = 40 - std::rand() % 80;
    int i = std::rand() % 2;
    prey() = true;
    timer() = SDL_GetTicks();

    //Détermine le genre du mouton
    if (i < 1)
        female() = false;
    else
        female() = true;
}


/*
    Constructeur de la classe sheep
    Param : - window_surface_ptr, pointeur vers la surface
*/
sheep::sheep(SDL_Surface* window_surface_ptr, ground* g)
    : sheep(window_surface_ptr, g, frame_boundary + std::rand() % (frame_width - 2 * frame_boundary), frame_boundary + std::rand() % (frame_height - 2 * frame_boundary) ) /*Appel le constructeur de animal avec le chemin de l'image*/ {
}


/*
    Bouge le mouton à l'aide de la fonction constrained_linear_move_()
*/
void sheep::move() {
    constrained_linear_move_(pos_x(), pos_y(), vel_x(), vel_y());
}

/*
* Intéragit avec l'animal passé en paramètre
*/
void sheep::interacts(std::shared_ptr<moving_object> a) {
    unsigned time = 4; //Timer en s mouton

    if ((a->pos_x() > this->pos_x() && a->pos_x() < this->pos_x() + this->image_ptr()->w) || (a->pos_x() + a->image_ptr()->w > this->pos_x() && a->pos_x() + a->image_ptr()->w < this->pos_x() + this->image_ptr()->w))
        if ((a->pos_y() > this->pos_y() && a->pos_y() < this->pos_y() + this->image_ptr()->h) || (a->pos_y() + a->image_ptr()->h > this->pos_y() && a->pos_y() + a->image_ptr()->h < this->pos_y() + this->image_ptr()->h))
        {
            if (a->get_prop("sheep") && this->get_prop("female") != a->get_prop("female")) { // Si l'animal est un mouton et de genre différent 
                if (this->get_prop("female") && this->timer() + time*1000 < SDL_GetTicks()) {
                    this->timer() = SDL_GetTicks();
                    this->g()->add_moving(std::make_shared<sheep>(this->surface(), this->g(), this->pos_x(), this->pos_y()));
                }

                else if (a->get_prop("female") && a->timer() + time * 1000 < SDL_GetTicks()) {
                    a->timer() = SDL_GetTicks();
                    a->g()->add_moving(std::make_shared<sheep>(a->surface(), a->g(), a->pos_x(), a->pos_y()));
                }
            }
        }
    if (a->get_prop("wolf")) { // Si l'animal est un loup 

        if (a->pos_x() >= this->pos_x() && a->pos_x() - this->pos_x() <= 80) {
            this->vel_x() = -60;
            if (a->pos_y() >= this->pos_y() && this->pos_y() - a->pos_y() <= 80) {
                this->vel_y() = -60;
            }
            if (a->pos_y() <= this->pos_y() && a->pos_y() - this->pos_y() <= 80) {
                this->vel_y() = 60;
            }
        }
        if (a->pos_x() <= this->pos_x() && this->pos_x() - a->pos_x() <= 80) {
            this->vel_x() = 60;
            if (a->pos_y() >= this->pos_y() && this->pos_y() - a->pos_y() <= 80) {
                this->vel_y() = -60;
            }
            if (a->pos_y() <= this->pos_y() && a->pos_y() - this->pos_y() <= 80) {
                this->vel_y() = 60;
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////
// WOLF
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe wolf
    Param : - window_surface_ptr, pointeur vers la surface
*/
wolf::wolf(SDL_Surface* window_surface_ptr, ground* g)
    : animal("media\\wolf.png", window_surface_ptr, g, "wolf")  /*Appel le constructeur de animal avec le chemin de l'image*/ {
    // Spawn wolf randomly
    pos_x() = frame_boundary + std::rand() % (frame_width - 2 * frame_boundary);
    pos_y() = frame_boundary + std::rand() % (frame_height - 2 * frame_boundary);
    vel_x() = 40 - std::rand() % 80;
    vel_y() = 40 - std::rand() % 80;
    type() = "wolf";
    timer() = SDL_GetTicks();
}

/*
    Bouge le loup à l'aide de la fonction constrained_linear_move_()
*/
void wolf::move() {
    unsigned death_time = 8; //In seconds

    if (timer() + death_time * 1000 < SDL_GetTicks()) {
        alive() = false;
        return;
    }
    constrained_linear_move_(pos_x(), pos_y(), vel_x(), vel_y());
}

/*
* Fonction vide pour l'instant
* A faire : Si prey : Avancer vers la cible ou manger si assez proche
*/
void wolf::interacts(std::shared_ptr<moving_object> a) {
    //Si un animal est dans un rayon de 40 depuis les pos_x et pos_y
    if (a->get_prop("sheep")) { // Si l'animal est un mouton A REMPLACER PAR PREY

        if (a->pos_x() >= this->pos_x() && a->pos_x() - this->pos_x() <= 200) {
            this->vel_x() = 40;
            if (a->pos_y() >= this->pos_y() && this->pos_y() - a->pos_y() <= 200) {
                this->vel_y() = 40;
            }
            if (a->pos_y() <= this->pos_y() && a->pos_y() - this->pos_y() <= 200) {
                this->vel_y() = -40;
            }
        }
        if (a->pos_x() <= this->pos_x() && this->pos_x() - a->pos_x() <= 200) {
            this->vel_x() = -40;
            if (a->pos_y() >= this->pos_y() && this->pos_y() - a->pos_y() <= 200) {
                this->vel_y() = 40;
            }
            if (a->pos_y() <= this->pos_y() && a->pos_y() - this->pos_y() <= 200) {
                this->vel_y() = -40;
            }
        }
        

        if ((a->pos_x() > this->pos_x() && a->pos_x() < this->pos_x() + this->image_ptr()->w ) || (a->pos_x()+ a->image_ptr()->w > this->pos_x() && a->pos_x() + a->image_ptr()->w < this->pos_x() + this->image_ptr()->w)   )
            if ((a->pos_y() > this->pos_y() && a->pos_y() < this->pos_y() + this->image_ptr()->h) || (a->pos_y() + a->image_ptr()->h > this->pos_y() && a->pos_y() + a->image_ptr()->h < this->pos_y() + this->image_ptr()->h)) {
                a->alive() = false; //Tue l'animal
                timer() = SDL_GetTicks(); //Reset le timer de faim
            }
        }
}

/////////////////////////////////////////////////////////////////////////////////
// DOG
/////////////////////////////////////////////////////////////////////////////////

dog::dog(SDL_Surface* window_surface_ptr, ground* g, std::weak_ptr<shepherd> s)
    : animal("media\\doggo.png", window_surface_ptr, g, "dog")  /*Appel le constructeur de animal avec le chemin de l'image*/ {
    // Spawn wolf randomly
    
    pos_x() = frame_boundary + std::rand() % (frame_width - 2 * frame_boundary);
    pos_y() = frame_boundary + std::rand() % (frame_height - 2 * frame_boundary);
    vel_x() = 40 - std::rand() % 80;
    vel_y() = 40 - std::rand() % 80;
    type() = "dog";
    timer() = SDL_GetTicks();
    this->berger = s;
    
}

void dog::move() {
    auto b = berger.lock();
    constrained_linear_move_dog(pos_x(), pos_y(), vel_x(), vel_y(), b->pos_x(), b->pos_y());
}

void dog::interacts(std::shared_ptr<moving_object> a){}

/////////////////////////////////////////////////////////////////////////////////
// GROUND
/////////////////////////////////////////////////////////////////////////////////

/*
    Constructeur de la classe ground
    Param : - window_surface_ptr, pointeur vers la surface
*/
ground::ground(SDL_Surface* window_surface_ptr)
    : window_surface_ptr_{ window_surface_ptr_ } {};

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
void ground::add_moving(const std::shared_ptr<moving_object>& new_moving) {
    this->all_moving_.push_back(new_moving);
}

/*
    Update le ground
*/
void ground::update() {
    // Idee pour proj final
    for (int i = 0; i < all_moving_.size(); i++) {
        auto a = all_moving_[i];
        if (!a->get_prop("alive"))
            continue;
        for (int y = 0; y < all_moving_.size(); y++) {
            auto b = all_moving_[y];
            if (a == b)
                continue;
            if (!b->get_prop("alive"))
                continue;
            a->interacts(b);
        }
        a->move();
        a->draw();
    }

    //Remove dead 
    auto remove = std::remove_if(all_moving_.begin(), all_moving_.end(), is_dead);
    all_moving_.erase(remove, all_moving_.end());

    Draw_Score(all_moving_, window_surface_ptr_);

}

int ground::get_score() {
    return Score(all_moving_);
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
    : window_ptr_{ nullptr }, window_surface_ptr_{ nullptr }, //Initialise les ptr à null
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
        g_.add_moving(std::make_shared<sheep>(window_surface_ptr_, &g_));
    // Add some wolves
    for (unsigned i = 0; i < n_wolf; ++i)
        g_.add_moving(std::make_shared<wolf>(window_surface_ptr_, &g_));

    auto d = std::make_shared<shepherd>(window_surface_ptr_, &g_);

    g_.add_moving(d);

    g_.add_moving(std::make_shared<dog>(window_surface_ptr_, &g_, d));
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
    while (keep_window_open && ((SDL_GetTicks() - start_ticks) < period * 1000)) {
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

    std::cout << std::endl << "Your score : " << g_.get_score() << std::endl << std::endl;

    return 0;
}
