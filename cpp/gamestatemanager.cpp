#include "gamestatemanager.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>

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
  depth_to_pop = current_stack_depth = 0;

  std::for_each(states_to_clear.begin(), states_to_clear.end(), [](auto & gs){ gs->queue_free(); });
  states_to_clear.clear();
  if (is_empty()) return;

  // get to one past the last, then step back once (zero past the last, aka the last)
  int top_gs_id = static_cast<int>(state_stack.size()) - 1;
  state_stack[top_gs_id]->_on_update(delta_time); // doesn't exist anymore!!!

  // slowly bring topgsid down to 0, if applicable
  while (top_gs_id > 0 && state_stack[top_gs_id]->get_update_underneath()) {
      top_gs_id--;
      state_stack[top_gs_id]->_on_update(delta_time);
  }

  // process the pop queue
  for (; depth_to_pop != 0 ; --depth_to_pop){
    internal_pop_state();
  }
  
}

void godot::GameStateManager::push_state(Ref<PackedScene> const& p_scene)
{
  if (p_scene.is_null())

  { 
#ifdef _DEBUG
    //throw std::invalid_argument("you passed in a null PackedScene!!!"); 
    ERR_FAIL_COND_MSG(p_scene.is_null(), "You passed in a null PackedScene!");
#endif
    return;
  }

  Node* instance = p_scene->instantiate();
  GameState* new_state = Object::cast_to<GameState>(instance);
  if (!new_state) 

  {
    instance->queue_free();
#ifdef _DEBUG
    //throw std::invalid_argument("you passed in a Scene root node that doesn't extend GameState!!!");
    ERR_FAIL_COND_MSG(p_scene.is_null(), "you passed in a Scene root node that doesn't extend GameState!!!");
#endif
    return;
  }  
  if (!is_empty()){
    state_stack.back()->set_process_mode(PROCESS_MODE_DISABLED);
  }
  add_child(new_state);
  state_stack.push_back(new_state);
  new_state->_on_enter();
}

void godot::GameStateManager::push_state(GameState *p_state)
{
  if (!is_empty()){
    state_stack.back()->set_process_mode(PROCESS_MODE_DISABLED);
  }
  add_child(p_state);
  state_stack.push_back(p_state);
  p_state->_on_enter();
}

void godot::GameStateManager::pop_state()
{
  depth_to_pop = current_stack_depth + 1;
}

void godot::GameStateManager::change_state(const Ref<PackedScene> &p_scene)
{
  if (!is_empty()) {
    internal_pop_state(); 
  }
  push_state(p_scene);
}

void godot::GameStateManager::change_state(GameState *p_state)
{
  if (!is_empty()){
    internal_pop_state();
  }
  push_state(p_state);
}

void godot::GameStateManager::clear_stack()
{
  while (!is_empty()){
    internal_pop_state();
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

void godot::GameStateManager::internal_pop_state()
{
  // do i need these checks now? it should be guaranteed that i cant pop these.... right?
  // if (is_empty()) return; // lets try removing this check...
  state_stack.back()->_on_exit();

  states_to_clear.push_back(state_stack.back());
  state_stack.pop_back();

  if (is_empty()) return;
  state_stack.back()->set_process_mode(PROCESS_MODE_INHERIT);
}

void godot::GameStateManager::_bind_methods() {
  ClassDB::bind_method(
      D_METHOD("push_state_scene", "p_scene"),
      static_cast<void (GameStateManager::*)(const Ref<PackedScene>&)>(&GameStateManager::push_state)
  );
  ClassDB::bind_method(
      D_METHOD("push_state", "p_state"),
      static_cast<void (GameStateManager::*)(GameState*)>(&GameStateManager::push_state)
  );

  ClassDB::bind_method(
      D_METHOD("change_state_scene", "p_scene"),
      static_cast<void (GameStateManager::*)(const Ref<PackedScene>&)>(&GameStateManager::change_state)
  );
  ClassDB::bind_method(
      D_METHOD("change_state", "p_state"),
      static_cast<void (GameStateManager::*)(GameState*)>(&GameStateManager::change_state)
  );

  ClassDB::bind_method(D_METHOD("pop_state"), &GameStateManager::pop_state);
  ClassDB::bind_method(D_METHOD("clear_stack"), &GameStateManager::clear_stack);
  ClassDB::bind_method(D_METHOD("is_empty"), &GameStateManager::is_empty);

  ClassDB::bind_method(D_METHOD("get_expected_stack_depth"), &GameStateManager::get_expected_stack_depth);
  ClassDB::bind_method(D_METHOD("set_expected_stack_depth", "p_depth"), &GameStateManager::set_expected_stack_depth);

  ADD_PROPERTY(
      PropertyInfo(Variant::INT, "expected_stack_depth"),
      "set_expected_stack_depth",
      "get_expected_stack_depth"
  );
}
