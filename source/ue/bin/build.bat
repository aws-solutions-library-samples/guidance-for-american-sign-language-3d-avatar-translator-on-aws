call variables.bat

del "%PROJECT_ROOT%\Saved\StagedBuilds\Windows\%PROJECT_NAME%\Config\ASLMetaHuman.ini"

call "%UE5DIR%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%PROJECT_FULL_FILENAME%" -clientconfig=Shipping -utf8output -platform=Win64 -targetplatform=Win64 -build -cook -allmaps -stage

rem call "%UE5DIR%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%PROJECT_FULL_FILENAME%" -clientconfig=DebugGame -utf8output -platform=Win64 -targetplatform=Win64 -build -cook -allmaps -stage

del "%PROJECT_ROOT%\Saved\StagedBuilds\Windows\%PROJECT_NAME%\Config\ASLMetaHuman.ini"
del "%PROJECT_ROOT%\Saved\StagedBuilds\Windows\%PROJECT_NAME%\Config\DefaultEngine.ini"
del "%PROJECT_ROOT%\Saved\StagedBuilds\Windows\%PROJECT_NAME%\Config\DefaultGame.ini"

copy "%PROJECT_ROOT%\Config\ASLMetaHuman.ini" "%PROJECT_ROOT%\Saved\StagedBuilds\Windows\%PROJECT_NAME%\Config\ASLMetaHuman.ini"
copy "%PROJECT_ROOT%\Config\DefaultEngine.ini" "%PROJECT_ROOT%\Saved\StagedBuilds\Windows\%PROJECT_NAME%\Config\DefaultEngine.ini"
copy "%PROJECT_ROOT%\Config\DefaultGame.ini" "%PROJECT_ROOT%\Saved\StagedBuilds\Windows\%PROJECT_NAME%\Config\DefaultGame.ini"
