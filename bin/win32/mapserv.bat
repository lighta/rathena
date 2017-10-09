@ECHO OFF
if "%~1" == "" ( set arch="x64" 
) else set arch=%~1

set cud_dir=%~dp0
cd %cud_dir%
CALL serv.bat map-server.exe Map-Server %arch%
