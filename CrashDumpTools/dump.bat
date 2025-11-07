del dump.txt
for /r %%i in (CrashDumps\*.dmp) do "C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe" -i "C:\Program Files (x86)\Steam\steamapps\common\Baldurs Gate 3\bin\bg3.exe" -z "%%i" -c ".ecxr;k;q" >> dump.txt
python dumpparser.py dump.txt > dump-parsed.txt
pause
