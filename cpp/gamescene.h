#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

namespace godot {

class GameScene : public Node {
  GDCLASS(GameScene, Node)
public:
  GameScene() = default;
  ~GameScene() = default;

  // no _process()... this is handled by scenestack...

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

#endif // _GAME_SCENE_H_
