# DoorControlling
Dieses Repository beinhaltet eine Software für eine Arduino basierte Automatiktür.

## Sensoren und Aktoren
1. Neopixelring mit 12LED's
2. 2 Sensoren von Omron E3X DA6-Sund
3. 2 Endschalter von Cherry D429 V3AA


## Funktion:
Wenn Tür geschlossen:
  Switch 1 Endschalter wird getrittert.
  Neopixel Ring zeigt auf allen LED's blau.
  
Während es öffnens: 
  Neopixelring gegen den Uhrzeigersinn aufleuchten, blau. (simultan mit Motor)
  
  
Wenn Tür offen:
  Switch 2 Endschalter wird getriggert.
  Neopixelring zeigt Farbe rot.
  
 Tür soll für 10Sekunden offen bleiben, wenn ein neues Signal erkannt wird fängt der Countdown von Vorne an.
 
 Sicherheitsparameter:
  1. Wenn die Tür beim Zufahren länger als 4Sekunden braucht, fahre wieder auf und warte 10Sekunden.
  
