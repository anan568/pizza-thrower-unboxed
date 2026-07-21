#include "gamestatemanager.h"

godot::GameStateManager::GameStateManager()
{}

godot::GameStateManager::~GameStateManager() = default;

void godot::GameStateManager::_ready()
{
  state_stack.reserve(expected_stack_depth);
  states_to_clear.reserve(expected_stack_depth);
}

void godot::GameStateManager::_process(double delta_time)
{
  states_to_clear.clear();
  if (is_empty()) return;

  GameState* gs{state_stack.back()}; // how to compile so that it is noexcept and constexpr when _debug == 0?
  gs->_on_update(delta_time);
}

void godot::GameStateManager::push_state(Ref<PackedScene> const& p_scene)
{
  Node* instance = p_scene->instantiate();
  GameState* new_state = Object::cast_to<GameState>(instance);

  state_stack.push_back(new_state);

  new_state->_on_enter();
}

void godot::GameStateManager::push_state(GameState *p_state)
{
  state_stack.push_back(p_state);
  p_state->_on_enter();
}

void godot::GameStateManager::pop_state()
{
  if (is_empty()) return;
  state_stack.back()->_on_exit();

  states_to_clear.push_back(state_stack.back());
  state_stack.pop_back();
}

void godot::GameStateManager::change_state(const Ref<PackedScene> &p_scene)
{
  if (is_empty()) {
    pop_state(); 
  }
  push_state(p_scene);
}

void godot::GameStateManager::change_state(GameState *p_state)
{
  if (!is_empty()){
    pop_state();
  }
  push_state(p_state);
}

void godot::GameStateManager::clear_stack()
{
  while (!is_empty()){
    // copied the body of pop_state() less the first check...
    (state_stack.back())->_on_exit();

    states_to_clear.push_back(state_stack.back());
    state_stack.pop_back();
  }
}

void godot::GameStateManager::set_expected_stack_depth(int p_depth)
{
  expected_stack_depth = p_depth;
}

bool godot::GameStateManager::is_empty()
{
  return state_stack.empty();
}
