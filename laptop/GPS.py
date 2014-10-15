#!/usr/bin/python
import math


def getDir(lat1, long1, lat2, long2):
    dy = lat1 - lat2
    dx = math.cos(math.pi/180.*lat1)*(long1 - long2)
    return math.atan2(dy,dx)

def haversine(lat1, lon1, lat2, lon2):
    """
    Calculate the great circle distance between two points 
    on the earth (specified in decimal degrees)
    """
    # convert decimal degrees to radians 
    lon1, lat1, lon2, lat2 = map(math.radians, [lon1, lat1, lon2, lat2])

    # haversine formula 
    dlon = lon2 - lon1 
    dlat = lat2 - lat1 
    a = math.sin(dlat/2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon/2)**2
    c = 2 * math.asin(math.sqrt(a)) 

    # 6367000 m is the radius of the Earth
    m = 6367000 * c
    return m
