import minimalmodbus
import serial.tools.list_ports
import time
import random

minimalmodbus.TIMEOUT = 5
minimalmodbus.PARITY = 'N'
slave_address = 10

MODBUS_READ_INPUT = 0x4

REG_INPUT_START = 0x300
REG_INPUT_TEMP_L  = 0
REG_INPUT_TEMP_H  = 1
REG_INPUT_HUMI_L  = 2
REG_INPUT_HUMI_H  = 3
REG_INPUT_PRES_L  = 4
REG_INPUT_PRES_H  = 5
REG_INPUT_GAS_L   = 6
REG_INPUT_GAS_H   = 7
REG_INPUT_SIZE = 8

mb = minimalmodbus.Instrument('/dev/ttyUSB0', slave_address, mode='rtu')
if not mb.serial.is_open:
  mb.serial.open()

mb.serial.baudrate = 115200

while(1):
  data = mb.read_registers(REG_INPUT_START, REG_INPUT_SIZE, functioncode=MODBUS_READ_INPUT)

  raw_temp = (data[REG_INPUT_TEMP_H] << 16) | data[REG_INPUT_TEMP_L]
  raw_humi = (data[REG_INPUT_HUMI_H] << 16) | data[REG_INPUT_HUMI_L]
  raw_pres = (data[REG_INPUT_PRES_H] << 16) | data[REG_INPUT_PRES_L]
  raw_gas  = (data[REG_INPUT_GAS_H]  << 16) | data[REG_INPUT_GAS_L]

  temp = raw_temp / 100.0
  humi = raw_humi / 1000.0
  pres = raw_pres / 100.0
  gas = raw_gas

  #print('T: {0}, P: {1}, H: {2}, G: {3}'.format(raw_temp, raw_pres, raw_humi, raw_gas))
  print('T: {0} degC, P: {1} hPa, H: {2} %%rH, G: {3} ohms'.format(temp, pres, humi, gas))

  time.sleep(1)


