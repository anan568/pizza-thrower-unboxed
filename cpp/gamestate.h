#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

namespace godot {

class GameState : public Node {
  GDCLASS(GameState, Node)
public:
  GameState() = default;
  ~GameState() = default;

  // no _process()... this is handled by gamestatemanager...

  // you already know grok helped find me GDVIRTUAL and GDVIRTUALCALL
  // Real C++ Virtual Functions (Public Interface)
  virtual void _on_enter();
  virtual void _on_exit();
  virtual void _on_update(double delta_time);

  // GDVirtual bindings (internal script hooks)
  GDVIRTUAL0(_gd_on_enter);
  GDVIRTUAL0(_gd_on_exit);
  GDVIRTUAL1(_gd_on_update, double);

  void set_update_underneath(bool p_update_underneath) { update_underneath = p_update_underneath;}
  bool get_update_underneath() const {return update_underneath;}
private:
  bool update_underneath = false;
  

protected:
  static void _bind_methods();
};

} // namespace godot

#endif // _GAME_STATE_H_
