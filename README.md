# DoorControlling
Dieses Repository beinhaltet eine Software für eine Arduino basierte Automatiktür.

## Sensoren und Aktoren
1. Neopixelring mit 12LED's
2. 2 Sensoren von Omron E3X DA6-Sund
3. 2 Endschalter von Cherry D429 V3AA
4. Stromsensor für Motor


## Funktion:
### Wenn Tür geschlossen:
  Switch 1 Endschalter wird getrittert.
  Neopixel Ring zeigt auf allen LED's blau.
  
### Während es Öffnens: 
  Neopixelring gegen dem Uhrzeigersinn aufleuchten, blau. (simultan mit Motor)
  
  
### Wenn Tür offen:
  Switch 2 Endschalter wird getriggert.
  Neopixelring zeigt Farbe rot.
  
 Tür soll für 10Sekunden offen bleiben, wenn ein neues Signal erkannt wird fängt der Countdown von vorne an.
 
### Während des Schließens:
  Neopixelring mit dem Uhrzeigersinn aufleuchten, blau. (simultan mit Motor)
 
 #### Sicherheitsparameter:
  1. Wenn die Tür beim Zufahren länger als 4 Sekunden braucht, fahre wieder auf und warte 10 Sekunden.
  2. Wenn der Motor zu viel Strom zieht, fahre wieder auf und warte 10 Sekunden.
  
  
