# esp32_mega_uart.py
# UART2 communication from ESP32 D1 R32 to Arduino Mega (Serial1)

import machine
import time
import sys
# from typing import Optional

COMMANDS = {
    "d": "Drive forward",
    "b": "Drive backward",
    "s": "Stop motors",
    "a<angle>": "Set servo angle (30–150)",
    "v<speed>": "Set speed (120–255)",
    "exit": "Exit the program"
}

def initialize_uart(uart_number: int, baudrate: int = 9600) -> machine.UART:
    uart = machine.UART(uart_number, baudrate)
    uart.init()
    return uart

def clear_uart_buffer(uart: machine.UART) -> None:
    print("Clearing UART buffer...")
    while uart.any():
        uart.read()

def print_command_list() -> None:
    print("\nAvailable commands:")
    for cmd, desc in COMMANDS.items():
        print(f"  {cmd:10} - {desc}")
    print()

def send_command(uart: machine.UART, command: str) -> None:
    uart.write(command + "\n")
    time.sleep(1.1)

    if uart.any():
        response: Optional[bytes] = uart.read()
        if response:
            try:
                print("Response:", response.decode().strip())
            except UnicodeDecodeError:
                print("Received undecodable data:", response)

def main() -> None:
    uart2: machine.UART = initialize_uart(2, 9600)
    clear_uart_buffer(uart2)

    print("ESP32 ↔ Arduino Mega UART Communication Started.")
    print_command_list()

    while True:
        try:
            command: str = input("Enter command: ").strip()
            if command.lower() == "exit":
                print("Exiting program...")
                break
            send_command(uart2, command)
        except KeyboardInterrupt:
            print("\nProgram interrupted.")
            break
        except Exception as e:
            print("Error:", e)

if __name__ == "__main__":
    main()
