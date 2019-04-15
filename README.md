# Arduino-serial-struct

Programme de démo qui montre la possibilité de transférer des données sous forme de structs C++ en binaire via le port série de l'arduino.

## Utilisation
```
cat /dev/ttyACM0 #ouvre le port série pour affichage
./run.sh l 13 1 #écrit HIGH dans le GPIO 13 (led)
./run.sh r 255 0 #lit l'adresse 255 de l'EEPROM
./run.sh w 255 83 #écrit 83 en binaire dans l'adresse 255 de l'EEPROM
```
