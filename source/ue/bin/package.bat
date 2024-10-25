rem Generate UE shipping package (in %ARCHIVE_DIR%)

call variables.bat

set ARCHIVE_DIR=%PROJECT_ROOT%\PKG
rem set BUILD_TYPE=DebugGame
set BUILD_TYPE=Shipping

mkdir "%ARCHIVE_DIR%"

call "%UE5DIR%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%PROJECT_FULL_FILENAME%" -nocompileeditor -installed -nop4 -project="%PROJECT_FULL_FILENAME%" -cook -stage -archive -archivedirectory="%ARCHIVE_DIR%" -package -ue5exe="%UE5DIR%\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" -pak -prereqs -nodebuginfo -targetplatform=Win64 -build -target=%PROJECT_NAME% -clientconfig=%BUILD_TYPE% -utf8output

set CONFIG_DIR=%ARCHIVE_DIR%\Windows\%PROJECT_NAME%\Config
mkdir "%CONFIG_DIR%"
copy "%PROJECT_ROOT%\Config\%PROJECT_NAME%.ini" "%CONFIG_DIR%"