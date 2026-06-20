@echo off
setlocal
echo Do you want to install nexus-rtapi? (Y/N)
set /p "choice=> "
if /I "%choice%"=="Y" (
    echo Installing nexus-rtapi...
    git submodule add https://github.com/RaidcoreGG/GW2-RealTime-API-Releases.git submodules/nexus-rtapi
    echo Done.
) else (
    echo Installation cancelled.
)
pause
