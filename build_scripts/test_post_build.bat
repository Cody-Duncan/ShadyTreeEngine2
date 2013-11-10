setlocal EnableDelayedExpansion

set TargetDir=%~1
set SolutionDir=%~2
set Debug=%~3

echo.
echo --------- POSTBUILD Copying Resources and Libs -------------

xcopy "%SolutionDir%resources\*" "%TargetDir%resources\" /F /Y /S
echo Resources Complete
echo.

If "%Debug%" == "-d" (
	set libDir=ShadyTreeEngineD
) Else (
	set libDir=ShadyTreeEngine
)

xcopy "%SolutionDir%lib\!libDir!\*.dll" "%TargetDir%" /F /Y
xcopy "%SolutionDir%lib\!libDir!\*.pdb" "%TargetDir%" /F /Y

xcopy "%SolutionDir%lib\*.dll" "%TargetDir%" /F /Y
xcopy "%SolutionDir%lib\*.pdb" "%TargetDir%" /F /Y


echo --------- END Copying Resources and Libs -------------