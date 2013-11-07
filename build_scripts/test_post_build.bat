setlocal EnableDelayedExpansion

set TargetDir=%1
set SolutionDir=%2
set Debug=%3

echo.
echo.
echo PostBuild Test

xcopy "%SolutionDir%resources\*" "%TargetDir%resources\" /F /Y /S
echo Resources Complete
echo.

if "%Debug%" EQU "-d" ( 
	for /r %SolutionDir%\lib %%f in (*D.dll) do copy %%f %TargetDir% /Y
	echo DLL's Complete
	echo.
	
	echo Copying PDB files
	for /r %SolutionDir%\lib %%f in (*.pdb) do copy %%f %TargetDir% /Y
	echo.
) else (
	for /r %SolutionDir%\lib %%f in (*.dll) do (
		set "var=%%f"
		if "!var:~-5!" NEQ "D.dll" ( 
			echo copying: %%~nf%%~xf
			copy %%f %TargetDir% /Y )
	)
	echo DLL's Complete
	echo.
)
echo PostBuild copy complete
echo.
echo.