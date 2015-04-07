# Fixer.py
# Take a C++ file run astyle and remove any badness done by astyle
import sys
import string
import os
import glob
import stat
import shutil
import cStringIO

tempname = "FixStyle.tmp"
recurse = 1

def fixCode(code):
	ocode = code
	#code = string.replace(code, "if(", "if (")
	code = code.replace("return ;", "return;")
	code = code.replace("( -", "(-")
	#~ if code != ocode:
		#~ print "!", ocode, ">", code
	return code

def fixLine(line, inComment):
	line = line.rstrip()
	if inComment:
		if line.find("*/") != -1:
			inComment = 0
	else:
		if line.find("#include") == 0:
			line = line.replace(" / ", "/")
			line = line.replace("< ", "<")
			line = line.replace(" >", ">")
		elif line.find("/*") != -1:
			inComment = 1
			if line.find("*/") != -1:
				# Both start and end comment on one line
				inComment = 0
		elif line.find("//") != -1:
			pos = line.find("//")
			code = line[:pos]
			comment = line[pos:]
			line = fixCode(code) + comment
		else:
			line = fixCode(line)
	return line, inComment

def contents(filename):
	f = file(filename)
	t = f.read()
	f.close()
	return t

def fixFile(filename):
	#print filename
	inText = contents(filename)
	tmpFileName = os.path.splitext(filename)[0] + ".fix"
	origFileName = tmpFileName + ".orig"
	shutil.copyfile(filename, tmpFileName)
	os.system("astyle -tapO " + tmpFileName)
	cfile = file(tmpFileName)
	lastLine = 1
	#~ print "processing", filename
	outList = []
	inComment = 0
	for line in cfile.readlines():
		line, inComment = fixLine(line, inComment)
		if line or lastLine:
			outList.append(line)
		lastLine = line
	cfile.close()
	outText = "\n".join(outList)
	if outText != inText:
		print len(inText), len(outText)
		#print len(os.path.commonprefix([outText,inText]))
		print "Replacing", filename
		os.unlink(filename)
		outFile = file(filename, "wt")
		outFile.write(outText)
		outFile.close()
	os.unlink(tmpFileName)
	#os.unlink(origFileName)

def fixDir(dir, extensions):
	print "dir", dir
	for filename in os.listdir(dir):
		for ext in extensions:
			if not filename.count(".orig") and filename.count(ext):
				fixFile(dir + os.sep + filename)
	if recurse:
		for filename in os.listdir(dir):
			dirname =  dir + os.sep + filename
			#print ":", dirname
			if stat.S_ISDIR(os.stat(dirname)[stat.ST_MODE]):
				fixDir(dirname, extensions)

#os.chdir("\\os\\Updates\\SciTE-1.36+pl01")
if len(sys.argv) > 1:
	fixFile(sys.argv[1])
else:
	fixDir(os.getcwd(), [".cxx", ".h"])
