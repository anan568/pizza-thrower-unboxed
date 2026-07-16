# So you wanna write some C++, huh, Lowzy...
This file will serve as a sort of tutorial for writing your custom classes to work in Godot!

## Setup: (prerequisite for everyone!!!)
- follow the C++ setup guide in `README.md`

## Making a class for the extension
- Let's say you wanna make a class that can be used to make objects in the Godot project...

Here's your starting boilerplate:

```cpp
/*!************************************************************************
  \file boilerplate.h
  \author Bob
  \date 13-7-2026
  \brief
  This class is a template for GDClasses
**************************************************************************/
// include relevant files here
// e.g.
// #include <godot_cpp/classes/sprite2d.hpp>

namespace godot{ // optional, for convenience so i dont have to write godot:: everywhere

// replace T with whatever class you wanna inherit the interface of... Node2D, Sprite2D, Node, RefCounted... Object, if you love managing memory on your own??
class BPClass : public T{ 
  // critical
  GDCLASS(BPClass, T)
public:
  // godot will pass delta_time into this function every frame...
  void _process(double delta) override;

protected:
  // a function that Godot will call to find out what methods can be called and which properties it exposes.
  // for now, the function definition will be empty. as you add methods, 
  // you will update this function, explained in a later section...
  static void _bind_methods();  

private:
  // data members here...
};
}

//@todo: make a corresponding .cpp file to provide definitions... also explained later
```

and quickly add your class to register_types.cpp
```cpp
// in register_types.cpp...
void initialize_pizza_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
  // add all classes you make here!!!
	GDREGISTER_RUNTIME_CLASS(BPClass);
}
```

## sending your class member functions to godot
1. Add your method to your class
```cpp
// in boilerplate.h...
class BPClass : Public T{
  // stuff above...
  // notice the default value
  ReturnType method_name(T param1, U param2 = "2", V param3 = "3"); 
  // stuff below...
};
```

2. Add your method to BPClass::_bind_methods()...
```cpp
// in boilerplate.cpp...
#include "BPClass.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot; // for classdb to be easy to read... be careful...
void BPClass::_bind_methods() {
  // notice the default value
	ClassDB::bind_method(D_METHOD("method_name", // method name
                      "param1", "param2", "param3", // param list
                      &BPClass::method_name, // pointer to member function
                      DEFVAL("2"), DEFVAL("3"))); // default values
}
```

Wanted to make variadic templates? sorry, you gotta google this one on your own... I have no idea how to do that...

## adding properties to your class
Add properties so they can be controlled easily in the inspector in Godot!

1. make a getter and setter method...
```cpp
// in boilerplate.h...
class BPClass : Public T{
  // stuff above...
  // the names dont matter
  void set_stat (const double new_stat);
  double get_stat() const;
  
  private:
  double mystat;
  // stuff below...
};
```

2. add the methods to _bind_methods()...
```cpp
// in boilerplate.cpp...
// imagine all the same stuff
void BPClass::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_stat"), &BPClass::get_stat);
	ClassDB::bind_method(D_METHOD("set_stat", "p_stat"), &BPClass::set_stat);

  // type, name, setter name, getter name
  // hint: in PropertyInfo, there are A LOT of options. check it out... heres just the bare minimum
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mystat"), "set_stat", "get_stat");
}
```

## making your class emit signals
Godot already has event driven programming! lets go!!!
Heres a dummy signal setup:

1. add your signal to _bind_methods()...
```cpp
void BPClass::_bind_methods(){
  // Register your signal layout
  ADD_SIGNAL(MethodInfo("stat_changed", PropertyInfo(Variant::FLOAT, "new_stat")));
}
```

2. you're done!!! call it with global function `emit_signal(name, property);`...
```cpp
void BPClass::set_stat(double new_stat){
    stat = new_stat;
    emit_signal("stat_changed", stat)
}
```

and someone like ananan may use it like:
```gdscript
extends Node2D

func _ready():
  var my_node = BPClass.new()
  my_node.stat_changed.connect(_on_speed_changed)
  add_child(my_node)

func _on_speed_changed(new_value: float):
  print("BPClass::set_stat(new_stat) called, new_stat: ", new_value)
```