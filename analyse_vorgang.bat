REM Umrechnungsfaktor 1000:1 für alle Variablen 
REM Damit Variablen während des Programms verändert werden können. Auf veränderte Variablen kann nun mittels ! zugegriffen werden.
setlocal EnableDelayedExpansion 

REM Gibt den Größenschritt an, der pro Aufruf auf die gewünschte Variable dazuaddiert wird um die gewünschte Messreihe in Abhängigkeit zu dieser Variable zu erhalten
set /A steps=50
REM Paramter, die beim Aufruf der Funktionen übergeben werden:
set /A k_S=50
set /A k_D=0
set /A w_S=1000
set /A friction=0

echo %k_S%
echo %k_D%
echo %w_S%
echo %friction%


pause
for /l %%i in (1,1,20) do (
REM Anwendung des Größenschritts:
set /A k_S=%k_S%+%steps%*%%i

echo Parameter k_S k_D w_S friction:
echo !k_S!
echo !k_D!
echo !w_S!
echo !friction!

echo Aufruf Nummer:
echo %%i

REM Aufruf der Simulation, Parameter /wait kann hinzugefügt werden, wenn die Simulationen hintereinander ablaufen sollen:
start SciComp_Projekt.exe !k_S! !k_D! !w_S! !friction!
REM start /wait SciComp_Projekt.exe !k_S! !k_D! !w_S! !friction!
)

pause
