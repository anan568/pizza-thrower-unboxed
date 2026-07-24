#include "gamestatemanager.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
//@todo investigate if a preloader approach is possible...
#include <godot_cpp/classes/resource_loader.hpp>

godot::GameStateManager::GameStateManager() = default;

godot::GameStateManager::~GameStateManager() = default;

void godot::GameStateManager::_ready()
{
  state_stack.reserve(expected_stack_depth);
  states_to_clear.reserve(expected_stack_depth);
}

void godot::GameStateManager::_process(double delta_time)
{
  std::for_each(states_to_clear.begin(), states_to_clear.end(), [](auto & gs){ gs->queue_free(); });
  states_to_clear.clear();
  if (is_empty()) return;

  // get to one past the last, then step back once (zero past the last, aka the last)
  int top_gs_id = static_cast<int>(state_stack.size()) - 1;
  state_stack[top_gs_id]->_on_update(delta_time); 

  // slowly bring topgsid down to 0, if applicable
  while (top_gs_id > 0 && state_stack[top_gs_id]->get_update_underneath()) {
    ++current_stack_depth;
    top_gs_id--;
    state_stack[top_gs_id]->_on_update(delta_time);
  }
}

void godot::GameStateManager::push_state(Variant const &state_variant)
{
  /// @todo investigate ways to make this more performant... 
  /// maybe i should set this to only allow PackedScenes so they may be preloaded on GD side...
  if (state_variant.get_type() == Variant::STRING){
    String path = std::move(state_variant);
    push_state(path);
    return;
  }
  // see if this works...
  Ref<PackedScene> scene = state_variant;
  if (scene.is_valid()){
    push_state(scene);
    return;
  }

  // hopefully you never use this, An...
  Object *obj = state_variant;
  GameState *gs = Object::cast_to<GameState>(obj);
  if (gs) {
    push_state(gs);
    return;
  } 
}

void godot::GameStateManager::push_state(String const &filename)
{
  Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(filename);
  ERR_FAIL_COND_MSG(!scene.is_valid(), "You passed in a filename to an invalid scene!");
  push_state(std::move(scene));
}

void godot::GameStateManager::push_state(Ref<PackedScene> const& p_scene)
{
  //throw std::invalid_argument("you passed in a null PackedScene!!!"); 
  ERR_FAIL_COND_MSG(p_scene.is_null(), "You passed in a null PackedScene!");

  Node* instance = p_scene->instantiate();
  GameState* new_state = Object::cast_to<GameState>(instance);
  if (!new_state) 
  {
    instance->queue_free();
    //throw std::invalid_argument("you passed in a Scene root node that doesn't extend GameState!!!");
    ERR_FAIL_COND_MSG(p_scene.is_null(), "you passed in a Scene root node that doesn't extend GameState!!!");
    return;
  }  

  if (!is_empty()){
    state_stack.back()->set_process_mode(PROCESS_MODE_DISABLED);
  }
  add_child(new_state);
  state_stack.push_back(new_state);

  new_state->_on_enter();
  ++current_stack_depth;
}

void godot::GameStateManager::push_state(GameState *p_state)
{
  if (!is_empty()){
    state_stack.back()->set_process_mode(PROCESS_MODE_DISABLED);
  }
  add_child(p_state);
  state_stack.push_back(p_state);
  p_state->_on_enter();
  ++current_stack_depth; // might be wrong about this one
}

void godot::GameStateManager::pop_state()
{
  // set number to one more than current depth, and iterate through it
  ++current_stack_depth;
  while(current_stack_depth){
    --current_stack_depth;
    internal_pop_state();
  }
}

void godot::GameStateManager::change_state(Variant const &state_variant)
{
  if (state_variant.get_type() == Variant::STRING){
    String path = std::move(state_variant);
    change_state(path);
    return;
  }
  // see if this works...
  Ref<PackedScene> scene = state_variant;
  if (scene.is_valid()){
    change_state(scene);
    return;
  }

  // hopefully you never use this, An...
  Object *obj = state_variant;
  GameState *gs = Object::cast_to<GameState>(obj);
  if (gs) {
    change_state(gs);
    return;
  } 
}

void godot::GameStateManager::change_state(String const &filename)
{
  Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(filename);
  ERR_FAIL_COND_MSG(!scene.is_valid(), "You passed in a filename to an invalid scene!");
  change_state(std::move(scene)); // harmless std::move even though it technically doesn't do anything... if i ever decide the change the function signature in the future...
}

void godot::GameStateManager::change_state(Ref<PackedScene> const &p_scene)
{
  if (!is_empty()) {
    internal_pop_state(); 
  }
  push_state(std::move(p_scene));
}

void godot::GameStateManager::change_state(GameState* p_scene)
{
  if (!is_empty()) {
    internal_pop_state(); 
  }
  push_state(p_scene);
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
      D_METHOD("push_state_fast", "packedscene"),
      static_cast<void (GameStateManager::*)(const Ref<PackedScene>&)>(&GameStateManager::push_state)
  );
  ClassDB::bind_method(
      D_METHOD("push_state", "scene_filename"),
      static_cast<void (GameStateManager::*)(Variant const&)>(&GameStateManager::push_state)
  );

  ClassDB::bind_method(
      D_METHOD("change_state_fast", "packedscene"),
      static_cast<void (GameStateManager::*)(const Ref<PackedScene>&)>(&GameStateManager::change_state)
  );
  ClassDB::bind_method(
      D_METHOD("change_state", "scene_filename"),
      static_cast<void (GameStateManager::*)(Variant const&)>(&GameStateManager::change_state)
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
