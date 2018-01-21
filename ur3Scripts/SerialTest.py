import serial, time


def translate(value, leftMin, leftMax, rightMin, rightMax):
    # Figure out how 'wide' each range is
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin

    # Convert the left range into a 0-1 range (float)
    valueScaled = float(value - leftMin) / float(leftSpan)

    # Convert the 0-1 range into a value in the right range.
    return rightMin + (valueScaled * rightSpan)



#ports = list(serial.tools.list_ports.comports())
#for p in ports:
#	print p
#Ouvert max : servo 140
#Ouvert min : servo 13

pince = serial.Serial('/dev/ttyACM0', 115200, timeout=.1, writeTimeout=.1)
interface = serial.Serial('/dev/ttyACM1', 115200, timeout=.1, writeTimeout=.1)
#arduino.parity = serial.PARITY_NONE
#arduino.bytesize = serial.EIGHTBITS
#arduino.stopbits = serial.STOPBITS_ONE
#arduino.xonxoff = False
#arduino.rtscts = False
#arduino.dsrdtr = False
oldVal =0
var =0
i = 0
time.sleep(2) #give the connection a second to settle
while True :
	dataPince = pince.readline()
	dataInterface = interface.readline()
	adapt = dataInterface.split('#')
	if len(adapt) == 4:
		angle = adapt[3]
		try :
			var = translate(float(angle),5.7,6.84,40,165)
		except :
			var = oldVal
		toDisp =(var+oldVal)/2 +50
		temp = str(int(toDisp))
		print(temp)
	try:
		pince.write(temp + "#")
		pince.flush()
		pince.flushInput()
		pince.flushOutput()
	except:
		print("error")
		print(i)
		oldVal = var

	print(dataPince)
	print(dataInterface)

	
arduino.close();
