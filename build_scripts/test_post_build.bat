setlocal EnableDelayedExpansion

set TargetDir=%~1
set SolutionDir=%~2
set Debug=%~3

echo.
echo.
echo PostBuild Test

xcopy "%SolutionDir%resources\*" "%TargetDir%resources\" /F /Y /S
echo Resources Complete
echo.

echo "%SolutionDir%lib\ShadyTreeEngineD\*.dll"

if "%Debug%" EQU "-d" (
	
	xcopy %SolutionDir%lib\ShadyTreeEngineD\*.dll "%TargetDir%" /F /Y /S
	xcopy "%SolutionDir%lib\ShadyTreeEngineD\*.pdb" "%TargetDir%" /F /Y /S
	xcopy "%SolutionDir%lib\ShadyTreeEngineD\*.lib" "%TargetDir%" /F /Y /S
 
) else (
	xcopy "%SolutionDir%lib\ShadyTreeEngine\*.dll" "%TargetDir%" /F /Y /S
	xcopy "%SolutionDir%lib\ShadyTreeEngine\*.pdb" "%TargetDir%" /F /Y /S
	xcopy "%SolutionDir%lib\ShadyTreeEngine\*.lib" "%TargetDir%" /F /Y /S
)
echo PostBuild copy complete
echo.
echo.