#!/usr/bin/env python

import shutil,sys,os
from xml.etree.cElementTree import iterparse
from xml.etree.cElementTree import SubElement

kanjidic = "/usr/local/share/kp/kanjidic2.xml"
encoding = sys.getfilesystemencoding()
context = iterparse(kanjidic, events=("start","end"))

for event, elem in context:
	if event == "end" and elem.tag == "character":
		if (os.path.exists(elem[0].text+".svg")):
			shutil.move(elem[0].text.encode(encoding)+".svg","japanese/"+elem[0].text.encode(encoding)+".svg")

