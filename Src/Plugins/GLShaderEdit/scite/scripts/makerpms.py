# makerpms.py
# Copy files all over the place build RPMs and copy to top level directory

import os
import shutil
srcRoot = "../../"
rpmRoot = "/usr/src/redhat/SOURCES/"
rpmBin = "/usr/src/redhat/RPMS/i386/"
rpmSource = "/usr/src/redhat/SRPMS/"

verFileName = srcRoot + "scintilla/version.txt"
vers = open(verFileName)
#139
vFull = vers.read().strip()
vers.close()

#1.39
vPoint = vFull[0] + "." + vFull[1:]

#1, 3, 9, 0
vComma = vFull[0] + ", " + vFull[1] + ", " + vFull[2] + ", 0"

print "[", vFull, "|", vPoint, "|", vComma, "]"

tgzV = "scite" + vFull + ".tgz"
tgzFileName = srcRoot + "scite.tgz"
tgzVFileName = srcRoot + tgzV

print "[", tgzFileName, "|", tgzVFileName, "]"

if not os.access(tgzFileName, os.F_OK):
	print "Base file '" + tgzFileName + "' does not exist."
else:
	shutil.copyfile(tgzFileName, tgzVFileName)
	os.unlink(tgzFileName)
	
	rpmVFileName = rpmRoot + tgzV
	
	shutil.copyfile(tgzVFileName, rpmVFileName)
	
	# Run the rpm build command
	os.system("rpm -ba scite.spec")
	
	rpmB = "scite-" + vPoint + "-1.i386.rpm"
	shutil.copyfile(rpmBin + rpmB, srcRoot + rpmB)
	rpmS = "scite-" + vPoint + "-1.src.rpm"
	shutil.copyfile(rpmSource + rpmS, srcRoot + rpmS)
	
