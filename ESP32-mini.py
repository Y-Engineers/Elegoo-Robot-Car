# esp32_uart0_control.py
# Non-blocking UART0 input from USB and UART2 output to Arduino Mega

import machine
import time

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
        response = uart.read()
        if response:
            try:
                print("Response:", response.decode().strip())
            except UnicodeDecodeError:
                print("Received undecodable data:", response)

def check_uart_input(uart: machine.UART) -> str | None:
    if uart.any():
        try:
            data = uart.read().decode().strip()
            return data
        except UnicodeDecodeError:
            print("Invalid input from UART0.")
    return None

def main() -> None:
    uart0 = initialize_uart(0, 115200)  # USB to PC
    uart2 = initialize_uart(2, 9600)    # Arduino Mega

    clear_uart_buffer(uart2)
    print("ESP32 UART0 (USB) ↔ UART2 (Arduino) Communication Started.")
    print_command_list()

    while True:
        try:
            command = check_uart_input(uart0)
            if command:
                print("Received:", command)
                if command.lower() == "exit":
                    print("Exiting program...")
                    break
                send_command(uart2, command)
            time.sleep(0.1)  # Short delay to avoid high CPU usage
        except KeyboardInterrupt:
            print("\nProgram interrupted.")
            break
        except Exception as e:
            print("Error:", e)

if __name__ == "__main__":
    main()
