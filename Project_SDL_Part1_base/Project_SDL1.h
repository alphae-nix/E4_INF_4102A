// SDL_Test.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>
#include "declarations.hh"
#include <SDL_test.h>

// Defintions
constexpr double frame_rate = 60.0; // refresh rate
constexpr double frame_time = 1. / frame_rate;
constexpr unsigned frame_width = 1400; // Width of window in pixel
constexpr unsigned frame_height = 900; // Height of window in pixel
// Minimal distance of animals to the border
// of the screen
constexpr unsigned frame_boundary = 100;

// Helper function to initialize SDL
void init();

class animal {
private:
  SDL_Surface* window_surface_ptr_; // ptr to the surface on which we want the
                                    // animal to be drawn, also non-owning
  SDL_Surface* image_ptr_; // The texture of the sheep (the loaded image), use
                           // load_surface_for
  // todo: Attribute(s) to define its position
  double pos_x_;
  double pos_y_;
  double vel_x_;
  double vel_y_;

  //timer pour animaux (l'utilisation change pour chaque)
  unsigned timer_;

  //Prop
  bool female_;
  bool alive_;
  bool prey_;

  std::string type_; // Sheep, wolf...
 
  ground* g_;


public:
  animal(const std::string& file_path, SDL_Surface* window_surface_ptr, ground* g);
  // todo: The constructor has to load the sdl_surface that corresponds to the
  // texture
  virtual ~animal(); // todo: Use the destructor to release memory and "clean up
               // behind you"

  void draw(); // todo: Draw the animal on the screen <-> window_surface_ptr.
                 // Note that this function is not virtual, it does not depend
                 // on the static type of the instance

  virtual void move() = 0; // todo: Animals move around, but in a different
                             // fashion depending on which type of animal<

  virtual void interacts(std::shared_ptr<animal> a) = 0; // Intéragit avec l'animal en param

  bool get_prop(std::string); //Retourne un bool selon la string en param

  // Accessors and mutators should be inlined
  double pos_x() const { return pos_x_; };
  double& pos_x() { return pos_x_; };
  double pos_y() const { return pos_y_; };
  double& pos_y() { return pos_y_; };
  double vel_x() const { return vel_x_; };
  double& vel_x() { return vel_x_; };
  double vel_y() const { return vel_y_; };
  double& vel_y() { return vel_y_; };

  unsigned timer() const { return timer_; };
  unsigned& timer() { return timer_; };

  bool female() const { return female_; };
  bool& female() { return female_; };

  bool alive() const { return alive_; };
  bool& alive() { return alive_; };

  bool prey() const { return prey_; };
  bool& prey() { return prey_; };

  std::string type() const { return type_; };
  std::string& type() { return type_; };

  SDL_Surface* surface() const { return window_surface_ptr_; };
  SDL_Surface* image_ptr() const { return image_ptr_; };

  ground* g() { return g_; };
};


class human {

private:
    SDL_Surface* window_surface_ptr_; // ptr to the surface on which we want the
                                    // animal to be drawn, also non-owning
    SDL_Surface* image_ptr_; // The texture of the sheep (the loaded image), use
                             // load_surface_for
    // todo: Attribute(s) to define its position
    double pos_x_human;
    double pos_y_human;

public:
    human(const std::string& file_path, SDL_Surface* window_surface_ptr);
    virtual ~human();

    void draw();

    virtual void move() = 0;

    double pos_x() const { return pos_x_human; };
    double& pos_x() { return pos_x_human; };
    double pos_y() const { return pos_y_human; };
    double& pos_y() { return pos_y_human; };
};

//class du berger
class shepherd : public human {
public:
    //Ctor
    shepherd(SDL_Surface* window_surface_ptr);
    //Dtor
    // implement functions that are purely virtual in base class
    void move() override;
};

// Insert here:
// class sheep, derived from animal
class sheep : public animal {
public:
  // Ctor
  sheep(SDL_Surface* window_surface_ptr, ground* g);
  // Dtor
  // implement functions that are purely virtual in base class
  void move() override;

  void interacts(std::shared_ptr<animal> a) override;
};

// Insert here:
// class wolf, derived from animal
// Use only sheep at first. Once the application works
// for sheep you can add the wolves
class wolf : public animal {
public:
  // Ctor
  wolf(SDL_Surface* window_surface_ptr, ground* g);
  // Dtor
  // implement functions that are purely virtual in base class
  void move() override;

  void interacts(std::shared_ptr<animal> a) override;
};

class dog : public animal {
//private:

    //shepherd my_shepherd;
    
    
public:
    // Ctor
    dog(SDL_Surface* window_surface_ptr, ground* g);
    // Dtor
    // implement functions that are purely virtual in base class
    void move() override;

};


// The "ground" on which all the animals live (like the std::vector
// in the zoo example).
class ground {
private:
  // Attention, NON-OWNING ptr, again to the screen
  SDL_Surface* window_surface_ptr_;

  // Some attribute to store all the wolves and sheep
  // here
  std::vector<std::shared_ptr<animal>> all_animals_;
  std::vector<std::shared_ptr<human>> all_human_;

public:
  ground()=default; // todo: Ctor
  ground(SDL_Surface* window_surface_ptr); // todo: Ctor
  //~ground(){}; // todo: Dtor, again for clean up (if necessary)
  void add_animal(const std::shared_ptr<animal>& new_animal); // todo: Add an animal
  void add_human(const std::shared_ptr<human>& new_human);
  void update(); // todo: "refresh the screen": Move animals and draw them
  // Possibly other methods, depends on your implementation
  void set_ptr(SDL_Surface* window_surface_ptr);
};

// The application class, which is in charge of generating the window
class application {
private:
  // The following are OWNING ptrs
  SDL_Window* window_ptr_;
  SDL_Surface* window_surface_ptr_;
  SDL_Event window_event_;

  // Other attributes here, for example an instance of ground
  ground g_;
  unsigned last_ticks_;

public:
  application(unsigned n_sheep, unsigned n_wolf); // Ctor
  ~application();                                 // dtor

  int loop(unsigned period); // main loop of the application.
                             // this ensures that the screen is updated
                             // at the correct rate.
                             // See SDL_GetTicks() and SDL_Delay() to enforce a
                             // duration the application should terminate after
                             // 'period' seconds
};