# esp32-serial2-mega.py
# Kommunikation vom ESP32 D1 R32 (UART2) zum Arduino Mega (Serial1)
# Verkabelung:
# GPIO16 (U2RX) <-- Mega TX1 (D18)
# GPIO17 (U2TX) --> Mega RX1 (D19)

import machine
import time
import sys
from typing import Optional

def initialize_uart(uart_number: int, baudrate: int = 9600) -> machine.UART:
    """Initialisiert die UART-Schnittstelle mit gegebener Baudrate."""
    uart = machine.UART(uart_number, baudrate)
    uart.init()
    return uart

def clear_uart_buffer(uart: machine.UART) -> None:
    """Löscht alle Daten aus dem UART-Puffer durch Auslesen."""
    print("Bytes im Puffer:", uart.any())
    print("Leere UART2...")
    while uart.any():
        uart.read()  # Verwerfe alle vorhandenen Daten

def send_command(uart: machine.UART, command: str) -> None:
    """Sendet einen Befehl über UART und gibt ggf. eine Antwort aus."""
    uart.write(command + "\n")  # Hänge Zeilenumbruch an
    time.sleep(1.1)  # Kurze Pause zur Verarbeitung

    if uart.any():
        response: Optional[bytes] = uart.read()
        if response:
            try:
                print("Response:", response.decode().strip())
            except UnicodeDecodeError:
                print("Nicht decodierbare Daten empfangen:", response)

def main() -> None:
    """Hauptschleife zur Benutzereingabe und Kommunikation per UART2."""
    uart2: machine.UART = initialize_uart(2, 9600)
    clear_uart_buffer(uart2)

    while True:
        try:
            command: str = input("Enter command (or 'exit' to quit):\n").strip()
            if command.lower() == "exit":
                print("Beende Programm...")
                break
            send_command(uart2, command)
        except KeyboardInterrupt:
            print("\nVom Benutzer abgebrochen.")
            break
        except Exception as e:
            print("Fehler:", e)

if __name__ == "__main__":
    main()
