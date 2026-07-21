#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <godot_cpp/classes/node.hpp>

namespace godot {

class GameState : public Node {
  GDCLASS(GameState, Node)
public:
  GameState() = default;
  ~GameState() = default;

  // no _process()... this is handled by gamestatemanager...
  virtual void _on_enter();
  virtual void _on_exit();
  virtual void _on_update(double delta_time);
  // touch is anotherword for background update
  virtual void _on_touch(double delta_time);

  void set_touch_underneath(bool p_touch_underneath) { touch_underneath = p_touch_underneath;}
  bool get_touch_underneath() const {return touch_underneath;}

  void set_update_underneath(bool p_update_underneath) { update_underneath = p_update_underneath;}
  bool get_update_underneath() const {return update_underneath;}

private:
  bool touch_underneath = true;
  bool update_underneath = false;

protected:
  static void _bind_methods();
};

} // namespace godot

#endif // _GAME_STATE_H_
