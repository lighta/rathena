::This is to set a link of db etc to bin. since the path in code must be relative to bin
:: we just create a shitton of junction as we may not have the sufficient right for symlink. (windows is that dumb)

set cud_dir=%~dp0
cd %cud_dir%

for %%A in (x86,x64) do (
 if not exist %%A\db mklink /j %%A\db ..\..\db
 if not exist %%A\conf mklink /j %%A\conf ..\..\conf
 if not exist %%A\npc mklink /j %%A\npc ..\..\npc
 if not exist %%A\log mklink /j %%A\log ..\..\log
 if not exist %%A\doc mklink /j %%A\doc ..\..\doc
) 

