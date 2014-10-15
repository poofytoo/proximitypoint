# python firebase serverless implementation

#!/usr/bin/python

import serial, urllib2, time, GPS, json


#try: Xbee = serial.Serial(port='/dev/ttyUSB0',baudrate=9600,timeout=1000)
#except: Xbee = serial.Serial(port='/dev/ttyUSB1',baudrate=9600,timeout=1000)

Xbee = None
i = 0

while not Xbee:
    try: Xbee = serial.Serial(port='/dev/ttyUSB{}'.format(i),baudrate=9600, timeout=1000)
    except: i += 1
# Xbee = serial.Serial(port='/dev/tty.usbserial-FTG97NL6',baudrate=9600,timeout=1000)


def readCoords(phone_id, split_char):
    r = urllib2.urlopen('https://poofytoo.firebaseio.com/proximitypoint/phone{}.json'.format(phone_id)).read()
    # print '-----------'
    # print (json.loads(r)['lat'], json.loads(r)['lon'])
    return (json.loads(r)['lat'], json.loads(r)['lon'])
    # return (t_lat1, t_lon1)
    #return urllib2.urlopen('http://smallerpackage4me.ru/proximitywatch/GPSdata_{}'.format(phone_id)).read().split(split_char)

def XbeeWrite(lat1,lon1,lat2,lon2):
    theta = GPS.getDir(lat1,lon1,lat2,lon2)
    dist = GPS.haversine(lat1,lon1,lat2,lon2)
    print "theta={} dist={}".format(theta,dist),
    Xbee.write('{},{}\n'.format(str(theta)[:5],str(dist)[:5]))

t_lat1, t_lon1 = readCoords(1,'|')
t_lat2, t_lon2 = readCoords(2,'|')


if __name__ == '__main__':
    while 1:
        try: 
            lat1,lon1 = map(float,readCoords(1,'|'))
            lat2,lon2 = map(float,readCoords(2,'|'))
            time.sleep(.05)
            if (lat1 != t_lat1 and lon1 != t_lon1) or (lat2 != t_lat2 and lon2 != t_lon2):
                t_lat1, t_lon1, t_lat2, t_lon2 = (lat1,lon1,lat2,lon2)
                XbeeWrite(lat1,lon1,lat2,lon2)
                print "\tNew coords: Phone1={},{}\tPhone2={},{}".format(lat1,lon1,lat2,lon2)
        except: print "You done fucked up"

