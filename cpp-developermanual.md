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
  // private variables here...
};
}

//@todo: make a corresponding .cpp file to provide definitions... also explained later
```

and quickly add your class to register_types.cpp
```cpp
// in register_types.cpp...
void initialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
  // add all classes you make here!!!
	GDREGISTER_RUNTIME_CLASS(BPClass);
}
```

## adding methods to your class
@todo explain how to add methods, and how to update _bind_methods();

## adding properties to your class
@todo explain how to add properties, also how to update _bind_methods();

## making your class emit signals
@todo explain how to emit signals, also how to update _bind_methods();
