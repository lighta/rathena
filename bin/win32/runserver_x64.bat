@echo off
rem This is and auto-restart script for the rAthena Ragnarok Online Server Emulator.
rem It will also keep the map server OPEN after it crashes to that errors may be
rem more easily identified
rem Writen by Jbain

set cud_dir=%~dp0
cd %cud_dir%

start cmd /k logserv.bat x64
start cmd /k charserv.bat x64
start cmd /k mapserv.bat x64
