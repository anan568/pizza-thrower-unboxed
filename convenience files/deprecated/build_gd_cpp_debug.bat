cd ../../godot-cpp
scons platform=windows target=template_debug api_version=4.7 custom_api_file="%~dp0../extension_api.json"
cd ../"convenience files"/deprecated
pause