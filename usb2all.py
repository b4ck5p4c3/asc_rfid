import minimalmodbus
import serial.tools.list_ports
import time
import random

RETRIES = 7
RETRY_TIMEOUT = 0.1
SAFE_TIMEOUT = 0.0

def safe_reads(mb, address, size):
  for i in xrange(RETRIES):
    try:
      if i > 0:
        print "retry read", i
      return mb.read_bits(address, size)
      time.sleep(SAFE_TIMEOUT)
    except Exception:
      # print "except"
      time.sleep(RETRY_TIMEOUT)

  print "modbus TIMEOUT"

def safe_read_registers(mb, address, size):
  for i in xrange(RETRIES):
    try:
      if i > 1:
        print "retry read", i
      return mb.read_registers(address, size, functioncode=0x4)
      time.sleep(SAFE_TIMEOUT)
    except Exception:
      # print "except"
      time.sleep(RETRY_TIMEOUT)

  print "modbus TIMEOUT"


def safe_write(mb, address, data):
  for i in xrange(RETRIES):
    try:
      if i > 0:
        print "retry write", i
      mb.write_bit(address, data, functioncode=0x05)
      time.sleep(SAFE_TIMEOUT)
      return
    except Exception:
      # print "except"
      time.sleep(RETRY_TIMEOUT)

  print "modbus TIMEOUT"

minimalmodbus.TIMEOUT = 5
minimalmodbus.PARITY = 'N'
slave_address = 2

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

READ_EN_BASE = 0x100
MODE_BASE = 0


mb = minimalmodbus.Instrument('/dev/ttyUSB0', slave_address, mode='rtu', debug=False)
if not mb.serial.is_open:
  mb.serial.open()

mb.serial.baudrate = 115200
# mb.handle_local_echo = True

prev_id = None

while(1):
  id = safe_read_registers(mb, 0x300, 16)
  if id[0] > 0:
    if id != prev_id:
      print id
      prev_id = id
  else:
    prev_id = None

  safe_write(mb, 0, 1)
  time.sleep(0.01)


