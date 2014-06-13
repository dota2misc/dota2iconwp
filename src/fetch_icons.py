#!/usr/bin/python

import sys
import urllib2
import re
import os
import errno
from sets import Set

icon_blacklist = Set(['Invoker_empty1.png', 'Invoker_empty2.png', 'Stats.png', 'Doom_bringer_empty1.png', 'Doom_bringer_empty2.png']);

if len(sys.argv) < 2:
    print "Expected one argument (save destination folder)"

if not os.path.isdir(sys.argv[1]):
    os.makedirs(sys.argv[1])

response = urllib2.urlopen('http://wiki.teamliquid.net/dota2/api.php?format=xml&action=query&list=categorymembers&cmtype=file&cmlimit=max&cmtitle=Category:Spell_Icons')
xml = response.read()
pageids = re.findall('(?<=pageid=")[0-9]*', xml)
for id in pageids:
    response = urllib2.urlopen('http://wiki.teamliquid.net/dota2/api.php?format=xml&action=query&prop=info&pageids=' + id + '&prop=imageinfo&iiprop=url')
    xml = response.read()
    imageurl = re.search('(?<=ii url=")[^"]*', xml)
    if imageurl != None:
        imagename = os.path.basename(imageurl.group(0))
        if imagename in icon_blacklist:
            continue
        impath = os.path.join(sys.argv[1], imagename);
        imdata = urllib2.urlopen(urllib2.Request(imageurl.group(0))).read()
        imfile = open(impath, 'wb')
        imfile.write(imdata)
        imfile.close()
        print impath


