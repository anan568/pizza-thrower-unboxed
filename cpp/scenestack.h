#ifndef _GAME_STATE_MANAGER_H_
#define _GAME_STATE_MANAGER_H_

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <vector>

#include "gamescene.h"

namespace godot {

class SceneStack : public Node {
  GDCLASS(SceneStack, Node)
public:
  SceneStack();
  ~SceneStack();

  // the constructor here actually
  void _ready() override;

  // loops through game states, updates the top one with delta_time (updates lower ones if the gamescene allows it), 
  // touches all underneath ones
  // then clears all the states in states to clear
  void _process(double delta_time) override;

  void push_state(Variant const& state_variant);
  void push_state(String const& filename);
  void push_state(const Ref<PackedScene>& p_scene);
  void push_state(GameScene* p_state);

  void change_state(Variant const& state_variant);
  void change_state(String const& filename);
  void change_state(const Ref<PackedScene>& p_scene);
  void change_state(GameScene* instancedstate);

  void reload_state();

  void pop_state();
  void pop_this_state();
  void clear_stack();

  GameScene* current_state() const;
  int get_stack_size() const;
  GameScene* state_at_depth(size_t depth) const;
  // TypedArray<GameScene> get_active_states() const;

  void set_expected_stack_depth(int p_depth);
  int get_expected_stack_depth() const { return expected_stack_depth; }

  /*!***********************************************************************
    \brief 
    check if there are no states in the stack
    \return true 
    if there are no states in the stack, otherwise returns
    \return false 
  *************************************************************************/
  bool is_empty();

private:
  void internal_pop_state();

  // LocalVector and Vector both didn't have the interface I wanted...
  using GS_Stack = std::vector<GameScene*>;
  GS_Stack state_stack;
  GS_Stack states_to_clear;

  // for optimisation.
  uint16_t expected_stack_depth = 4;
  uint16_t current_stack_depth = 0;
  // bool pause_lower_states = true;
  // bool process_background_states = false;

protected:
  static void _bind_methods();
};

} // namespace godot

#endif // _GAME_STATE_MANAGER_H_