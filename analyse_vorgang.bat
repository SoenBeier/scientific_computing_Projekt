::Umrechnungsfaktor 1000:1 für alle Variablen 
setlocal EnableDelayedExpansion
set /A steps=50
set /A k_S=50
set /A k_D=0
set /A w_S=1000
set /A friction=0
echo %k_S%
echo %w_S%
echo %friction%

for /l %%i in (1,1,100) do (
set /A k_S=%k_S%+%steps%*%%i
echo !k_S!
echo Aufruf Nummer:
echo %%i
start SciComp_Projekt.exe !k_S! !k_D! !w_S! !friction!
)

pause
