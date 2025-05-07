# esp32-serial2-mega.py
# ESP32 D1 R32 uart2 Arduino Mega Serial1()
# GPIO16 / U2RX / IO16 <--- black <--- yellow <--- TX1 / D18
# GPIO17 / U2TX / IO17 --->  green ---> green ---> RX1 / D19

import machine, time, sys

# uart2 = machine.UART(2, 115200)
uart2 = machine.UART(2, 9600)
uart2.init()

ende: bool = False
print( "Bytes im Puffer: ", uart2.any() )
print( "Leere UART2" )

while uart2.any():
  uart2.read()
  print("Start")
while not ende:
  command: str = input("Enter command\n")
  uart2.write(command)
  
  time.sleep(1.1)
  
  # while uart2.any():
  # print(uart2.read().decode())
