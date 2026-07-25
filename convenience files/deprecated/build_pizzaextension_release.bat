REM not useful to us until after we finish everything...
REM move this file out of this deprecated folder before running! it expects to be in the "convenience files" directory...
call clean_cpp.bat
cd ..
scons platform=windows target=template_release
cd "convenience files"
pause