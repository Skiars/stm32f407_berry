@ echo off

if not exist "../Middlewares/berry/generate" mkdir "../Middlewares/berry/generate"

"map_build.exe" -o ../Middlewares/berry/generate -i ../Middlewares/berry/src ../Src -c ../Inc/berry_conf.h

@ echo on
