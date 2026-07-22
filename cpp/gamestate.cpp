#include "gamestate.h"
#include <godot_cpp/core/class_db.hpp>

void godot::GameState::_on_enter() {
  // leaving the grok comments here because i feel they are useful
  // 1. Default C++ enter logic here (if any)

  // 2. Dispatch to GDScript if overridden in GDScript!
  GDVIRTUAL_CALL(_gd_on_enter);
}

void godot::GameState::_on_exit() {
  GDVIRTUAL_CALL(_gd_on_exit);
}

void godot::GameState::_on_update(double delta_time) {
  GDVIRTUAL_CALL(_gd_on_update, delta_time);
}

void godot::GameState::_bind_methods() {
  // Bind the script virtual hooks with the names GDScript users will see
  GDVIRTUAL_BIND(_gd_on_enter, "_on_enter");
  GDVIRTUAL_BIND(_gd_on_exit, "_on_exit");
  GDVIRTUAL_BIND(_gd_on_update, "_on_update", "delta_time");

  ClassDB::bind_method(D_METHOD("get_update_underneath"), &GameState::get_update_underneath);
  ClassDB::bind_method(D_METHOD("set_update_underneath", "p_update_underneath"), &GameState::set_update_underneath);
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "update_underneath"), "set_update_underneath", "get_update_underneath");
}
