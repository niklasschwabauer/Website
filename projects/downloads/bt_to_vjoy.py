import serial
import pyvjoy
import time

# vJoy initialisieren (vJoy-Gerät 1)
j = pyvjoy.VJoyDevice(1)

# Serielle Verbindung einrichten
# Ersetze 'COM5' durch den COM-Port, den dein HC-05 verwendet
ser = serial.Serial('COM6', 9600, timeout=1)

# Warte kurz, bis die serielle Verbindung bereit ist
time.sleep(2)

print("Warte auf Daten...")

while True:
    try:
        # Lese eine Zeile von der seriellen Schnittstelle (vom HC-05)
        line = ser.readline().decode('utf-8').strip()

        # Verarbeite die empfangenen Daten
        if line:
            lenkradWert = int(line)
            
            # Mappe den Wert auf den vJoy-Bereich (0 bis 32767)
            mapped_value = int((lenkradWert / 1023) * 32767)
            
            # Setze den gemappten Wert auf die X-Achse von vJoy (Lenkrad)
            j.data.wAxisX = mapped_value
            j.update()
            
            print(f"Lenkradwert: {lenkradWert} -> vJoy: {mapped_value}")
    
    except ValueError:
        print("Ungültige Daten empfangen")
    
    except serial.SerialException:
        print("Serielle Verbindung unterbrochen")
        break