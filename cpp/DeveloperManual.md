following my tutorial...

# Assuming we stay on this version (4.7) of Godot...

The `godot-cpp` repository contains a copy of all the metadata for the current Godot release!

So all we need to do realistically do is use run our SConstruct file that grok authored (and i messed with)...
(platform might be `linux` or `macos`)
```t
scons platform=windows
```

but if we ever update our version of Godot, we need to "call the Godot executable" (I think this means the godot engine .exe file) from the command line:

```t
#cd to the godot engine .exe folder
godot --dump-extension-api
```

and this will result in an `extension_api.json` file created in the executable's directory. copy this to our project folder and add `custom_api_file=<PATH_TO_FILE>` to the scons command below.

platform might be `linux` or `macos`

```t
cd godot-cpp
# probably
# scons platform=windows custom_api_file=..\extension_api.json
# but tbh we should just ask @grok to author us a .bat or .sh file
scons platform=windows custom_api_file=<PATH_TO_FILE>
cd ..
```
they also say that we might maybe need to add `bits=64` to the command on Windows or Linux. no idea what that means.
