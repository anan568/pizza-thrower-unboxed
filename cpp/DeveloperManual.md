following my tutorial...

# Assuming we stay on this version (4.7) of Godot...

The `godot-cpp` repository contains a copy of all the metadata for the current Godot release!

So all we need to do realistically do is use run our SConstruct file that grok authored (and i messed with)...
(platform should not be `linux` or `macos`)

```t
# in whatever/pizza-thrower-unboxed/cpp....
scons platform=windows target=template_debug dev_build=yes custom_api_file=extension_api.json
```


note that i (bob) will use this command when running from linux (i have mingw installed (its a compiler in linux that compiles executables for windows))
```t
# in whatever/pizza-thrower-unboxed/cpp....
scons platform=windows use_mingw=yes target=template_debug dev_build=yes custom_api_file=extension_api.json
```

if compiling for other platforms... uh.... ask grok....

but if we ever update our version of Godot, we need to "call the Godot executable" (I think this means the godot engine .exe file) from the command line:


```t
#cd to the godot engine .exe folder
./godot.windows.opt.whatever.the.name.is.exe --dump-extension-api
```

and this will result in an `extension_api.json` file created in the executable's directory. copy this to our project folder and rerun the command!!

they also say that we might maybe need to "add `bits=64` to the command on Windows or Linux." no idea what that means. but its here in case something goes wrong.
