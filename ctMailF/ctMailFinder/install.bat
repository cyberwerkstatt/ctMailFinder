@echo off
cd /d %~dp0
color a
echo *********** Willkommen! Die Installation wird nun durchgefuehrt! ***********
pause

echo installiere Python...
python-3.11.9-amd64.exe

echo installiere pip.....
python get-pip.py

echo Installiere Abhaengigkeiten aus requirements.txt...
pip install -r requirements.txt

echo Wechsle in den Ordner holehe und installiere setup.py...
cd holehe
python setup.py install

echo Zurueck zum urspruenglichen Verzeichnis wechseln
cd ..
cls
echo Fertig!
pause
