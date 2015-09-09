#!/usr/bin/python
import os
import requests
import urllib2
import json
import xml.etree.ElementTree as ET
txt = open('./cities/input.txt', 'r').read().split('\n')
last = open('./last','r').read().split('\n')
begin = False
print last
if last[0]!='':
  begin=False
else:
  begin=True

for line in txt:
    if len(line.split('\t'))<2: 
	continue
    zip, city = line.split('\t')
    print last[0]+'='+city
    if begin!=True:
      if last[0].strip()==city.strip():
 	begin=True
 	continue	
      continue
    
    baseUrl = 'http://maps.google.com/maps/api/geocode/json?address='+(city.strip())+'&sensor=false&region=RU&language=ru'
    #baseUrl = 'http://nominatim.openstreetmap.org/search/?q='+(city.strip())+'&limit=1&format=xml'
    #baseUrl = 'http://nominatim.openstreetmap.org/search/?postalcode='+zip+'&limit=1&format=xml'
    print baseUrl
    resp = requests.get(baseUrl)
    msg = resp.content
    js= json.loads(msg)
    print js
    if js['results']:
      print u'{:s}: {:.2f}, {:.2f}'.format(
        js['results'][0]['address_components'][0]['long_name'],
        float( js['results'][0]['geometry']['location']['lng'] ),
        float( js['results'][0]['geometry']['location']['lat'] ) )
      f = open('result.txt', 'a')
      c2 = js['results'][0]['address_components'][0]['long_name']
      f.write( u"{:.2f} {:.2f} {:s}\n".format( float( js['results'][0]['geometry']['location']['lng'] ),float( js['results'][0]['geometry']['location']['lat'] ),c2 ).encode('utf-8') )
      f.close()
      l = open('last','w')
      l.write(city)
      l.close()
