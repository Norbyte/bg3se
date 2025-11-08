cd ..\BG3PublishingRoot\Release\PDB
for /D %%i in (*) do "C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\symstore.exe" add /f %%i /s C:\ProgramData\dbg\sym /t ""

cd ..\..\Devel\PDB
for /D %%i in (*) do "C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\symstore.exe" add /f %%i /s C:\ProgramData\dbg\sym /t ""
pause
