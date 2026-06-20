@echo off
setlocal
echo Do you want to install nexus-mumble? (Y/N)
set /p "choice=> "
if /I "%choice%"=="Y" (
    echo Installing nexus-mumble...
    git submodule add https://github.com/RaidcoreGG/RCGG-lib-mumble-api.git submodules/nexus-mumble
    echo Done.
) else (
    echo Installation cancelled.
)
pause
