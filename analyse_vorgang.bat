REM Anleitung zu dieser Datei
REM Ziel der Datei: Damit Messreihen mit einer stetig veränderten Variable aufgenommen werden können wird diese Datei benutzt. 

REM Vor der Aufnahme der Messdaten müssen die Parameter analysis_run.execute und analysis_run.foreign_call aktiviert sein. Die Größe des Grundrisses sowie die Datei müssen richtig ausgewählt und eingestellt sein. 
REM Alle übrigen Variablen, die nicht in dieser Batch Datei vorkommen müssen per Hand in die c++ Datei eingetragen werden.
REM Nun können die Variablen in dieser Datei für die eigenen Zwecke angepasst werden. Wenn ein anderer außer der gerade ausgewählte Paramter während dem Durchlauf varriert werden soll, so muss die Zeile 41 angepasst werden.
REM Die Anzahl der Aufrufe/ Analyseläufe der Simulation kann in Zeile 39 in der For Schleife verändert werden.
REM Sollen die Simulationen hintereinander weg ausgeführt werden, so erhält der Aufruf der Datei in Zeile 56 einen Parameter /wait vor dem Dateinamen. 
REM Nach dem Einstellen der gewünschten Parameter kann diese Datei gestartet werden



REM Damit Variablen während des Programms verändert werden können. Auf veränderte Variablen kann nun mittels ! zugegriffen werden.
setlocal EnableDelayedExpansion 

REM Gibt den Größenschritt an, der pro Aufruf auf die gewünschte Variable dazuaddiert wird um die gewünschte Messreihe in Abhängigkeit zu dieser Variable zu erhalten
set /A steps=100
REM Paramter, die beim Aufruf der Funktionen übergeben werden:
REM Umrechnungsfaktor für die folgeden Variablen 1000:1
REM Negative Werte werden ignoriert und durch das Programm bestimmt.

set /A k_S=5000
set /A k_D=0
set /A w_S=1000
set /A friction=900
REM Umrechnungsfaktor für diese Variablen 1:1
set /A decay=0
set /A diffusion=0

echo %k_S%
echo %k_D%
echo %w_S%
echo %friction%
echo %decay%
echo %diffusion%


pause
for /l %%i in (0,1,49) do (
REM Anwendung des Größenschritts:
set /A k_S=%k_S%-%steps%*%%i

echo Parameter k_S k_D w_S friction:
echo !k_S!
echo !k_D!
echo !w_S!
echo !friction!
echo !decay!
echo !diffusion!

echo Aufruf Nummer:
echo %%i

REM Aufruf der Simulation, Parameter /wait kann hinzugefügt werden, wenn die Simulationen hintereinander ablaufen sollen:
REM start SciComp_Projekt.exe !k_S! !k_D! !w_S! !friction! !decay! !diffusion!
start SciComp_Projekt.exe !k_S! !k_D! !w_S! !friction! !decay! !diffusion!
timeout /T 2
)

pause
