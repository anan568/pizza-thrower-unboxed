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
protected:
  // whwat is this?
  // its a static funnction that Godot will call to find out which methods can be called and which properties it exposes.
  static void _bind_methods();  

public:
  GDExample();
  ~GDExample();

  // whats this? apparently its "the same as the _process() function in GDScript"
  void _process(double delta) override;
};
}




#endif