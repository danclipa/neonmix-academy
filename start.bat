@echo off
setlocal
cd /d "%~dp0"

if not exist "build\Release\neonmix.exe" (
    echo Nu gasesc build\Release\neonmix.exe.
    echo Compileaza intai cu: cmake --build build --config Release
    pause
    exit /b 1
)

curl.exe --silent --fail http://127.0.0.1:8080/healthz 2>nul | findstr /x /c:"ok" >nul
if not errorlevel 1 goto open

start "" /b "build\Release\neonmix.exe"
for /L %%i in (1,1,20) do (
    timeout /t 1 /nobreak >nul
    curl.exe --silent --fail http://127.0.0.1:8080/healthz 2>nul | findstr /x /c:"ok" >nul
    if not errorlevel 1 goto open
)

echo Serverul nu a pornit. Verifica mesajele de mai sus si portul 8080.
pause
exit /b 1

:open
start "" http://127.0.0.1:8080/
echo NeonMix Academy ruleaza la http://127.0.0.1:8080/
echo Lasa aceasta fereastra deschisa cat timp folosesti aplicatia.
pause
