<a name="oben"></a>

# RoboCar
ESP32 Car - WEB-Server - feste IP-Adresse - Speed-Controll

## Story
Diese Anleitung beschreibt ein, von einem WEB-Server gesteuertes Fahrzeug. Der Antrieb des RoboCars erfolgt über vier Getriebe-Motore, als Spannungversorgung dienen zwei Akkus. Das Herzstrück ist ein ESP32 NodeMCU. Die Träger-Elemente werden aus zwei 4mm ABS-Platten gelasert. Optional kann das RoboCar um eine Hindernis-Erkennung und eine IR-Fernsteuerung erweitert werden. Die Montage eines Servos, eines Ultraschall-Sensors und eines IR-Empfängers ist vorgesehen.
Wird kein Ultraschallsensor oder IR-Empfänger montiert, reicht eine Bodenplatte für die Montage der Bauteile aus. Für die Bodenplatte kann auch eine 4mm Sperrholzplatte verwendet werden.

## Hardware

| Stück | Beschreibung | 
| -------- | -------- | 
| 2       | ABS Platte DINA4 4mm oder       | 
| 1        |Sperrholzplatte DINA4 4mm       |
| 1        | ESP32 NodeMCU        | 
| 8        | Platinenhalter (3D-Druck)        | 
| 4        | Getriebemotore mit Räder       | 
| 1        | Dual-H-Brücke-DC-Motorantrieb L298N        |
| 2       | 18650 Akkus mit Akkucase        | 
| 6       | Abstandshalter 40mm, M3       | 
| 1         | ESP32-Shield      | 
| 1        | Ultraschallmodul HC-SR04 (optional)       |
| 1        | IR-Empfänger mit Fernbedienung (optional)        | 
|        | Kleinmaterial, Schrauben, Muttern, Klemmen, Platinenstecker, Schaltdraht        | 
|    ---    | ---      | 


## Aufbau und Montage

Der Aufbau und die Montage des RoboCar ist in mehrere Schritte unterteilt.

- Schritt 1: Die ABS-Platten lasern (alternativ eine Sperrholzplatte)
Verwende die Lasercutter Vorlage, bestehend aus:
1 x Deckplatte.SVG (ABS)
1 x Bodenplatte.SVG (ABS)
oder
1 x 4mm Sperrholzplatte (Lasercutter Vorlage)


- Schritt 2: Acht Platinenhalter drucken.
Die Platinenhalter tragen den ESP32 und die H-Brücke. Sie werden auf die Bodenplatte geklebt.

![Bild](/pic/BM01.png)




- Schritt 3: Die vier Getriebemotore auf die Grundplatte montieren

![Bild](/pic/BM02.png)

![Bild](/pic/BM03.png)

- Schritt 4: Den Motor-Treiber L298N und den ESP32 mit Platinenhalter auf die Bodenplatte montieren.

Der ESP32 wird auf eine Lochraster-Platine mit Steckverbinder montiert.
Die Anschlüsse zum ESP32 mit Platinenstecker verdrahten.

![Bild](/pic/BM04.png)

- Schritt 5: Die Akku-Box im hinteren Teil auf die Bodenplatte montieren (M3x10).

![Bild](/pic/BM05.png)

- Schritt 6: Für die Verdrahtung der Bauelemente die Klemmen (WAGO-Verbindungsklemmen) auf die Bodenplatte kleben.

![Bild](/pic/BM06.png)

- Schritt 7: Die Bauelemente nach Schaltplan verdrahten

![Bild](/pic/BM07.png)

![Bild](/pic/BM11.png)

- Schritt 8: Das Programm in den ESP32 laden.

- Schritt 9: Erster Funktionstest.
Mit dem Seriellen Monitor der Arduino IDE die IP-Adresse des WEB-Servers auslesen und im Browser den WEB-Server aufrufen

- Schritt 10: War der Funktionstest erfolgreich, wird die Bodenplatte und die Deckplatte mit 40mm Abstandshalter miteinander verbunden

![Bild](/pic/BM08.png)

- Schritt 11: Die Räder auf die Getriebemotore stecken und verschrauben.

- Schritt 12: Zweiter Funktionstest.
Mit dem Seriellen Monitor der Arduino IDE die IP-Adresse des WEB-Servers auslesen und im Browser den WEB-Server aufrufen.

![Bild](/pic/BM09.png)

- Fertig!
Dieses RoboCar ist mit einen Solar-Panel für die Akku-Ladung ausgestattet,
sowie einen Servomotor zur Steuerung der Hinderniserkennung (Ultraschallsensor) und einen IR-Empfänger.

![Bild](/pic/BM10.png)


<div style="position:absolute; left:2cm; ">   
<ol class="breadcrumb" style="border-top: 2px solid black;border-bottom:2px solid black; height: 45px; width: 900px;"> <p align="center"><a href="#oben">nach oben</a></p></ol>
</div>
