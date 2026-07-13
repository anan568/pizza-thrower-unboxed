#ifndef _GD_EXAMPLE_H_
#define _GD_EXAMPLE_H_

#include <godot_cpp/classes/sprite2d.hpp>

namespace godot{
class GDExample : public Sprite2D{
  // a macro that sets up internal things for us. trust!
  // do this for every class that inherits
  GDCLASS(GDExample, Sprite2D)

private:
  double time_passed;
  double amplitude;
  double speed;
protected:
  // whwat is this?
  // its a static funnction that Godot will call to find out which methods can be called and which properties it exposes.
  static void _bind_methods();  

public:
  GDExample();
  ~GDExample();

  // godot will pass delta_time into this function every frame...
  void _process(double delta) override;

  void set_amplitude (const double p_amplitude);
  double get_amplitude() const;

  void set_speed(const double p_speed);
	double get_speed() const;
};
}




#endif