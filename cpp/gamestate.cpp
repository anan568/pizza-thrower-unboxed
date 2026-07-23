#include "gamestate.h"
#include <godot_cpp/core/class_db.hpp>

// skip constructing every single time...
static const godot::StringName sn_on_enter("_on_enter");
static const godot::StringName sn_on_exit("_on_exit");
static const godot::StringName sn_on_update("_on_update");

void godot::GameState::_on_enter() {
  // "optimisation: you HAVE defined on enter, right?"
  //if (has_method("_on_enter")) {
    call(sn_on_enter);
  //}
}

void godot::GameState::_on_exit() {
  //if (has_method("_on_exit")) {
    call(sn_on_exit);
  //}
}

void godot::GameState::_on_update(double delta_time) {
  //if (has_method("_on_update")) {
    call(sn_on_update, delta_time);
  //}
}

void godot::GameState::_bind_methods() {
  // DO NOT bind _on_enter, _on_exit, or _on_update here!
  
  ClassDB::bind_method(D_METHOD("get_update_underneath"), &GameState::get_update_underneath);
  ClassDB::bind_method(D_METHOD("set_update_underneath", "p_update_underneath"), &GameState::set_update_underneath);
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "update_underneath"), "set_update_underneath", "get_update_underneath");
}