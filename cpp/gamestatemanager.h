#ifndef _GAME_STATE_MANAGER_H_
#define _GAME_STATE_MANAGER_H_

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <vector>

#include "gamestate.h"

namespace godot {

class GameStateManager : public Node {
  GDCLASS(GameStateManager, Node)
public:
  GameStateManager();
  ~GameStateManager();

  // the constructor here actually
  void _ready() override;

  // loops through game states, updates the top one with delta_time (updates lower ones if the gamestate allows it), 
  // touches all underneath ones
  // then clears all the states in states to clear
  void _process(double delta_time) override;

  void push_state(const Ref<PackedScene>& p_scene);
  void push_state(GameState* p_state);
  void pop_state();
  void change_state(const Ref<PackedScene>& p_scene);
  void change_state(GameState* p_state);
  void clear_stack();

  GameState* get_current_state() const;
  int get_stack_depth() const;
  // TypedArray<GameState> get_active_states() const;

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
  // LocalVector and Vector both didn't have the interface I wanted...
  std::vector<GameState*> state_stack;
  std::vector<GameState*> states_to_clear;

  // for optimisation.
  uint16_t expected_stack_depth = 4;
  // bool pause_lower_states = true;
  // bool process_background_states = false;

protected:
  static void _bind_methods();
};

} // namespace godot

#endif // _GAME_STATE_MANAGER_H_