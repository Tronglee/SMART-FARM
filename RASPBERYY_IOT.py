###!/usr/bin/python3
 
# import Adafruit IO REST client.
from Adafruit_IO import Client, Feed, RequestError
from Adafruit_IO import MQTTClient
import time, serial, sys, board, digitalio
from time import sleep
indata = []
updateStateFlag = False

# Set to your Adafruit IO key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
ADAFRUIT_IO_KEY = 'e2cac4fce1de4ebb802250a704153fa0'

# Set to your Adafruit IO username.
# (go to https://accounts.adafruit.com to find your username)
ADAFRUIT_IO_USERNAME = 'trongle'
# Set to the ID of the feed to subscribe to for updates.
Relay_1 = 'relay1'
Relay_2 = 'relay2'
Relay_3 = 'relay3'
Relay_4 = 'relay4'
Relay_5 = 'relay5'
Threhold = 'threhold'
Settime = 'settime'

print("IoT Farm Project - Graduation Project")
print("Adafruit MQTT connected - >>...Start...<< ")
def connected(client):
    # Subscribe to changes on a feed named Counter.
    print('Subscribing to Feed {0}'.format(Relay_1))
    client.subscribe(Relay_1)

    print('Subscribing to Feed {0}'.format(Relay_2))
    client.subscribe(Relay_2)

    print('Subscribing to Feed {0}'.format(Relay_3))
    client.subscribe(Relay_3)

    print('Subscribing to Feed {0}'.format(Relay_4))
    client.subscribe(Relay_4)

    print('Subscribing to Feed {0}'.format(Relay_5))
    client.subscribe(Relay_5)

    print('Subscribing to Feed {0}'.format(Threhold))
    client.subscribe(Threhold)

    print('Subscribing to Feed {0}'.format(Settime))
    client.subscribe(Settime)

def disconnected(client):
    """Disconnected function will be called when the client disconnects."""
    sys.exit(1)

def message(client, feed_id, payload):
    """Message function will be called when a subscribed feed has a new value.
    The feed_id parameter identifies the feed, and the payload parameter has
    the new value.
    """
    print('Feed {0} received new value: {1}'.format(feed_id, payload))

    #Threhold detect value
    if(feed_id == 'threhold'):
        print('Temperature Threhold value: ', payload)
        print('Set Temperature threshold to Device\n')
        ser.write(('threhold'+ payload + '\r').encode())      
        sleep(1)

    if(feed_id == 'settime'):
        print('Timer Threhold value: ', payload)
        print('Set Timer to Device\n')
        ser.write(('timerSet'+ payload + '\r').encode())      
            
    #Relay 1
    if(feed_id == 'relay1'):
        if(payload == '1'):
            print('Turn on Relay 1\n')
            ser.write(('A1\r').encode())
            sleep(1)

        elif(payload == '0'):
            print('Turn off Relay 1\n')
            ser.write(('A0\r').encode())
            sleep(1)

    #Relay 2
    if(feed_id == 'relay2'):
        if(payload == '1'):
            print('Turn on Relay 2\n')
            ser.write(('B1\r').encode())
            sleep(1)

        elif(payload == '0'):
            print('Turn off Relay 2\n')
            ser.write(('B0\r').encode())
            sleep(1)

    #Relay 3
    if(feed_id == 'relay3'):
        if(payload == '1'):
            print('Turn on Relay 3\n')
            ser.write(('C1\r').encode())
            sleep(1)

        elif(payload == '0'):
            print('Turn off Relay 3\n')
            ser.write(('C0\r').encode())
            sleep(1)

    #Relay 4
    if(feed_id == 'relay4'):
        if(payload == '1'):
            print('Turn on Relay 4\n')
            ser.write(('D1\r').encode())
            sleep(1)

        elif(payload == '0'):
            print('Turn off Relay 4\n')
            ser.write(('D0\r').encode())
            sleep(1)
    #Relay 5
    if(feed_id == 'relay5'):
        if(payload == '1'):
            print('Turn on Relay 5\n')
            ser.write(('E1\r').encode())
            sleep(1)

        elif(payload == '0'):
            print('Turn off Relay 5\n')
            ser.write(('E0\r').encode())
            sleep(1)

    
    
# Create an MQTT client instance.
client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Create an MQTT client instance.
client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

##try: # if we have a 'type' feed
Temperature = aio.feeds('temperature')
Humidity = aio.feeds('humidity')
Relay1 = aio.feeds('relay1')
Relay2 = aio.feeds('relay2')
Relay3 = aio.feeds('relay3')
Relay4 = aio.feeds('relay4')
Relay5 = aio.feeds('relay5')
threhold = aio.feeds('threhold')
settime = aio.feeds('settime')
Foodremain = aio.feeds('foodremain')
##except RequestError: # create a type feed
##    #Sensor Feeds 
##    feed = Feed(name="temperature")
##    Temperature = aio.create_feed(feed)
##
##    feed = Feed(name="humidity")
##    Humidity = aio.create_feed(feed)
##
##    feed = Feed(name="gas")
##    Gas = aio.create_feed(feed)
##
##    feed = Feed(name="fire")
##    Fire = aio.create_feed(feed)
##    
##    #Relay Feeds 
##    feed = Feed(name="relay1")
##    Relay1 = aio.create_feed(feed)
##
##    feed = Feed(name="relay2")
##    Relay2 = aio.create_feed(feed)
##
##    feed = Feed(name="relay3")
##    Relay3 = aio.create_feed(feed)
##
##    feed = Feed(name="relay4")
##    Relay4 = aio.create_feed(feed)


loop_delay = 5    
ser = serial.Serial(
	port = '/dev/ttyAMA0',
	baudrate = 115200,
	parity = serial.PARITY_NONE,
	stopbits = serial.STOPBITS_ONE,
	bytesize = serial.EIGHTBITS,
	timeout = 1
)


    
##def findFromStringData(s):
##    for item in indata:
##        if(item.find('temp')) != -1:
##            temp = item[5:]
##            print('Temperature is: ' +temp)
##
##        if(item.find('humi')) != -1:
##            humid = item[5:]
##            print('Humidity is: ' +humid)
##
##        if(item.find('gas ')) != -1:
##            gas = item[5:]
##            print('Gas is: ' +gas)
##
##        if(item.find('fire')) != -1:
##            fire = item[5:]
##            print('Fire is: ' +fire)
##        
def relaySwitch():
            #Relay 1
        relay1Data = aio.receive(Relay1.key)
        relay2Data = aio.receive(Relay2.key)
        relay3Data = aio.receive(Relay3.key)
        relay4Data = aio.receive(Relay4.key)
        if int(relay1Data.value) == 1:
            print('received Relay 1 <- ON\n')
##            string1 = "Relay 1 On" + "\r"
            ser.write(("Relay 1 On\r").encode())
            time.sleep(1.5)
        elif int(relay1Data.value) == 0:
            print('received Relay 1 <- OFF\n')
##            string1 = "Relay 1 Off" + "\r"
            ser.write(("Relay 1 Off\r").encode())
            time.sleep(1.5)
        
        #Relay 2
        
        if int(relay2Data.value) == 1:
            print('received Relay 2 <- ON\n')
##            string2 = "Relay 2 On" + "\r"
            ser.write(("Relay 2 On\r").encode())
            time.sleep(1.5)
        elif int(relay2Data.value) == 0:
            print('received Relay 2 <- OFF\n')
##            string2 = "Relay 2 Off" + "\r"
            ser.write(("Relay 2 Off\r").encode())
            time.sleep(1.5)
        
        #Relay 3
        
        if int(relay3Data.value) == 1:
            print('received Relay 3 <- ON\n')
##            string3 = "Relay 3 On" + "\r"
            ser.write(("Relay 3 On\r").encode())
            time.sleep(1.5)
        elif int(relay3Data.value) == 0:
            print('received Relay 3 <- OFF\n')
##            string3 = "Relay 3 Off" + "\r"
            ser.write(("Relay 3 Off\r").encode())
            time.sleep(1.5)
        
        #Relay 4
       
        if int(relay4Data.value) == 1:
            print('received Relay 4 <- ON\n')
##            string4 = "Relay 4 On" + "\r"
            ser.write(("Relay 4 On\r").encode())
            time.sleep(1.5)
        elif int(relay4Data.value) == 0:
            print('received Relay 4 <- OFF\n')
##            string4 = "Relay 4 Off" + "\r"
            ser.write(("Relay 4 Off\r").encode())
            time.sleep(1.5)
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message

# Connect to the Adafruit IO server.
client.connect()
client.loop_background()


try:
    while (True):
##        ser.write(('SS\r').encode())
        time.sleep(1)
##        time.sleep(0.5)
##        if(updateStateFlag == True):
##            countValue = 6
##        elif(updateStateFlag == False):
##            countValue = 5
        if(ser.in_waiting >0):
            uart = ser.readline()
            data = uart.decode('utf-8')
            data = data.rstrip()
            if data:
                indata.append(data)
            if len(indata) == 5:
                print(indata)
                print('sending data to adafruit io...')
                for item in indata:
                    
                    if(item.find('temp')) != -1:
                        temp = item[5:]
                        print('Temperature: ', temp)
                        aio.send(Temperature.key, temp)
                        
                    if(item.find('humi')) != -1:
                        humid = item[5:]
                        print('humidity: ', humid)
                        aio.send(Humidity.key, humid)
                        
##                    if(item.find('gas ')) != -1:
##                        gas = item[5:]
##                        print('gas: ', gas)
##                        aio.send(Gas.key, gas)


                    if(item.find('food')) != -1:
                        foodremain = item[5:]
                        print('foodremain: ', foodremain)
                        aio.send(Foodremain.key, foodremain)
                        
                    if(item.find('relay1')) != -1:
                        relay1 = item[7:]
                        print('Relay 1 update value: ', relay1)
                        aio.send(Relay1.key, relay1)

                    if(item.find('relay2')) != -1:
                        relay2 = item[7:]
                        print('Relay 2 update value: ', relay2)
                        aio.send(Relay2.key, relay2)

                    if(item.find('relay3')) != -1:
                        relay3 = item[7:]
                        print('Relay 3 update value: ', relay3)
                        aio.send(Relay3.key, relay3)

                    if(item.find('relay4')) != -1:
                        relay4 = item[7:]
                        print('Relay 4 update value: ', relay4)
                        aio.send(Relay4.key, relay4)
                        
                indata = []        
                countValue = False
        time.sleep(4)

except KeyboardInterrupt:
        print('DOne')

finally:
    ser.close()


