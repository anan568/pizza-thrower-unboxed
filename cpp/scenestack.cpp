#if 0
#include "scenestack.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
//@todo investigate if a preloader approach is possible...
#include <godot_cpp/classes/resource_loader.hpp>

godot::SceneStack::SceneStack() = default;

godot::SceneStack::~SceneStack() = default;

void godot::SceneStack::_ready()
{
  state_stack.reserve(expected_stack_depth);
  states_to_clear.reserve(expected_stack_depth);
}

void godot::SceneStack::_process(double delta_time)
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

void godot::SceneStack::push_state(Variant const &state_variant)
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
  GameScene *gs = Object::cast_to<GameScene>(obj);
  if (gs) {
    push_state(gs);
    return;
  } 
}

void godot::SceneStack::push_state(String const &filename)
{
  Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(filename);
  CRASH_COND_MSG(!scene.is_valid(), "You passed in a filename to an invalid scene!");
  push_state(std::move(scene));
}

void godot::SceneStack::push_state(Ref<PackedScene> const& p_scene)
{
  //throw std::invalid_argument("you passed in a null PackedScene!!!"); 
  CRASH_COND_MSG(p_scene.is_null(), "You passed in a null PackedScene!");

  Node* instance = p_scene->instantiate();
  GameScene* new_state = Object::cast_to<GameScene>(instance);
  if (!new_state) 
  {
    
    instance->queue_free();
    //throw std::invalid_argument("you passed in a Scene root node that doesn't extend GameScene!!!");
    CRASH_COND_MSG(p_scene.is_null(), "you passed in a Scene root node that doesn't extend GameScene!!!");
    CRASH_COND_MSG(!new_state, "Scene root node does not extend GameScene!");
    return;
  }  

  if (!is_empty()){
    state_stack.back()->call_deferred("set_process_mode", PROCESS_MODE_DISABLED);
  }
  call_deferred("add_child", new_state);
  state_stack.push_back(new_state);

  new_state->_on_enter();
  ++current_stack_depth;
}

void godot::SceneStack::push_state(GameScene *p_state)
{
  if (!is_empty()){
    state_stack.back()->call_deferred("set_process_mode", PROCESS_MODE_DISABLED);
  }
  call_deferred("add_child", p_state);
  state_stack.push_back(p_state);
  p_state->_on_enter();
  ++current_stack_depth; // might be wrong about this one
}

void godot::SceneStack::pop_state()
{
  if (is_empty()) return;
  --current_stack_depth;
  internal_pop_state();
}

void godot::SceneStack::pop_this_state()
{
  // set number to one more than current depth, and iterate through it
  ++current_stack_depth;
  while(current_stack_depth){
    --current_stack_depth;
    internal_pop_state();
  }
}

void godot::SceneStack::change_state(Variant const &state_variant)
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
  GameScene *gs = Object::cast_to<GameScene>(obj);
  if (gs) {
    change_state(gs);
    return;
  } 
}

void godot::SceneStack::change_state(String const &filename)
{
  Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(filename);
  ERR_FAIL_COND_MSG(!scene.is_valid(), "You passed in a filename to an invalid scene!");
  change_state(std::move(scene)); // harmless std::move even though it technically doesn't do anything... if i ever decide the change the function signature in the future...
}

void godot::SceneStack::change_state(Ref<PackedScene> const &p_scene)
{
  if (!is_empty()) {
    internal_pop_state(); 
  }
  push_state(std::move(p_scene));
}

void godot::SceneStack::change_state(GameScene* instancedscene)
{
  if (!is_empty()) {
    internal_pop_state(); 
  }
  push_state(instancedscene);
}

void godot::SceneStack::reload_state()
{
  ERR_FAIL_COND_MSG(is_empty(), "Cannot reload state: Stack is empty!");

  GameScene* current = current_state();
  String scene_path = current->get_scene_file_path();
  change_state(scene_path);
  
}

void godot::SceneStack::clear_stack()
{
  while (!is_empty()){
    internal_pop_state();
  }
}

godot::GameScene *godot::SceneStack::current_state() const
{
  return state_stack.back();
}

int godot::SceneStack::get_stack_size() const
{
  return state_stack.size();
}

godot::GameScene *godot::SceneStack::state_at_depth(size_t depth) const
{
  CRASH_COND_MSG(depth > state_stack.size(), "Accessed out of bounds in scene_stack!!!");
  return state_stack[state_stack.size() - depth];
}

void godot::SceneStack::set_expected_stack_depth(int p_depth)
{
  expected_stack_depth = p_depth;
}

bool godot::SceneStack::is_empty()
{
  return state_stack.empty();
}

void godot::SceneStack::internal_pop_state()
{
  if(is_empty()) return;
  GameScene* state_to_remove {state_stack.back()};

  states_to_clear.push_back(state_to_remove);
  state_stack.pop_back();

  state_to_remove->_on_exit();

  call_deferred("remove_child", state_to_remove);



  if (is_empty()) return;
  // queue this?
  state_stack.back()->set_process_mode(PROCESS_MODE_INHERIT);
}

void godot::SceneStack::_bind_methods() {
  // push state alternatives
  ClassDB::bind_method(
    D_METHOD("push_scene_direct", "instancedscene"),
    static_cast<void (SceneStack::*)(GameScene*)>(&SceneStack::push_state)
  );

  ClassDB::bind_method(
    D_METHOD("push_scene_fast", "packedscene"),
    static_cast<void (SceneStack::*)(const Ref<PackedScene>&)>(&SceneStack::push_state)
  );

  ClassDB::bind_method(
    D_METHOD("push_scene", "scene_filename"),
    static_cast<void (SceneStack::*)(Variant const&)>(&SceneStack::push_state)
  );

  // change state alternatives: pop a state, push a state.
  ClassDB::bind_method(
    D_METHOD("change_scene_direct", "instancedscene"),
    static_cast<void (SceneStack::*)(GameScene*)>(&SceneStack::change_state)
  );

  ClassDB::bind_method(
    D_METHOD("change_scene_fast", "packedscene"),
    static_cast<void (SceneStack::*)(const Ref<PackedScene>&)>(&SceneStack::change_state)
  );

  ClassDB::bind_method(
    D_METHOD("change_scene", "scene_filename"),
    static_cast<void (SceneStack::*)(Variant const&)>(&SceneStack::change_state)
  );

  ClassDB::bind_method(D_METHOD("reload_scene"), &SceneStack::reload_state);

  // pops states until current state is popped
  ClassDB::bind_method(D_METHOD("pop_this_scene"), &SceneStack::pop_this_state);
  // pops one state
  ClassDB::bind_method(D_METHOD("pop_scene"), &SceneStack::pop_state);
  // pops all states
  ClassDB::bind_method(D_METHOD("clear_stack"), &SceneStack::clear_stack);
  
  // helpers, no need to check these...
  ClassDB::bind_method(D_METHOD("is_empty"), &SceneStack::is_empty);
  ClassDB::bind_method(D_METHOD("get_expected_stack_depth"), &SceneStack::get_expected_stack_depth);
  ClassDB::bind_method(D_METHOD("set_expected_stack_depth", "p_depth"), &SceneStack::set_expected_stack_depth);
  ClassDB::bind_method(D_METHOD("get_stack_size"), &SceneStack::get_stack_size);
  ClassDB::bind_method(D_METHOD("get_current_scene"), &SceneStack::current_state);
  ClassDB::bind_method(D_METHOD("get_top_scene"), &SceneStack::current_state);

  ADD_PROPERTY(
    PropertyInfo(Variant::INT, "expected_stack_depth"),
    "set_expected_stack_depth",
    "get_expected_stack_depth"
  );

  ADD_PROPERTY(
    PropertyInfo(Variant::OBJECT, "current_scene", PROPERTY_HINT_NODE_TYPE,  "GameScene", PROPERTY_USAGE_READ_ONLY, "GameScene"),
    "",
    "get_current_scene"
  );

  ADD_PROPERTY(
    PropertyInfo(Variant::OBJECT, "top_scene", PROPERTY_HINT_NODE_TYPE, "GameScene", PROPERTY_USAGE_READ_ONLY, "GameScene"),
    "",
    "get_top_scene"
  );
}
#endif