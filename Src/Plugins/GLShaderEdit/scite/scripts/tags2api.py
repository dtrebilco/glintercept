#! /usr/bin/env python

# Produces a .api file for SciTE's identifier completion and calltip features.
# invoke as 
# 	python tags2api.py tags >x.api
# before running this program, create a tags file with
# 	ctags --excmd=number --c-types=pcdgstu <header files>

import fileinput
import string
import time

# Definitions that start with _ are often used for infrastructure
# like include guards and can be removed with
removePrivate=1

# The Windows headers include both ANSI and UniCode versions
# of many functions with macros that default to one or the other.
# When winMode is on, these 4 lines are replaced with either the
# ANSI or UniCode prototype.
winMode = 1
include="A"	# Set to "W" if you want the UniCode prototypes.

class FileCache:
	'''Caches the contents of a set of files.
	Avoids reading files repeatedly from disk by holding onto the
	contents of each file as a list of strings.
	'''

	def __init__(self):
		self.filecache = {}
		
	def grabFile(self, filename):
		'''Return the contents of a file as a list of strings.
		New line characters are removed.
		'''
		if not self.filecache.has_key(filename):
			contents=[]
			f = open(filename)
			for line in f.readlines():
				if line[-1:] == '\n': line = line[:-1]
				contents.append(line)
			f.close()
			self.filecache[filename] = contents
		return self.filecache[filename]

def bracesDiff(s):
	''' Counts the number of '(' and ')' in a string and returns the difference between the two.
	Used to work out when a function prototype is complete.
	'''
	diff = 0
	mode=0 # 0 <=> default, 1 <=> comment, 2 <=> string
	for i in range(len(s)):
		if mode == 0: # default mode
			if s[i]=='(':
				diff += 1
			elif s[i]==')':
				diff -= 1
			elif s[i]=='"':
				mode=2
			elif i>0 and s[i-1]=='/' and s[i]=='/':
				return diff
			elif i>0 and s[i-1]=='/' and s[i]=='*':
				mode=1
		elif mode == 1: # comment
			if i>0 and s[i-1]=='*' and s[i]=='/':
				mode=0
		elif mode == 2: # string
			if s[i]=='"':
				mode=0
	return diff

fc = FileCache()
prev=""	# For filtering out some duplicates.
for line in fileinput.input():
	if line[0] != '!':	# Not a comment.
		(entityName, fileName, lineNo, tagType) = string.split(line, "\t")[:4]
		curLineNo = string.atoi(lineNo[:-2]) - 1	# -1 because line numbers in tags file start at 1.
		contents = fc.grabFile(fileName)
		if not removePrivate or entityName[0] != '_':
			if tagType[0] == "p":	# Function prototype.
				try:
					braces = bracesDiff(contents[curLineNo])
					curDef = contents[curLineNo]
					while braces > 0:	# Search for end of prototype.
						curLineNo = curLineNo + 1
						braces = braces + bracesDiff(contents[curLineNo])
						curDef = curDef + contents[curLineNo]
					# Normalise the appearance of the prototype.
					curDef = string.strip(curDef)
					# Replace whitespace sequences with a single space character.
					curDef = string.join(string.split(curDef))
					# Remove space around the '('.
					curDef = string.replace(string.replace(curDef, " (", '('), "( ", '(')
					# Remove trailing semicolon.
					curDef = string.replace(curDef, ";", '')
					# Remove return type.
					curDef = curDef[string.find(curDef, entityName):]
					if winMode:
						if string.find(curDef, "A(") >= 0:
							if "A" in include:
								print string.replace(curDef, "A(", '(')
						elif string.find(curDef, "W(") >= 0:
							if "W" in include:
								print string.replace(curDef, "W(", '(')
						else:	# A character set independent function.
							print curDef
					else:
						print curDef
				except IndexError:
					pass
			elif tagType[0] == 'd':	# Macro definition.
				curDef = contents[curLineNo]
				if (not winMode) or (curDef[-1] not in "AW"):
					if entityName <> prev:
						print entityName
				prev =	entityName
			else:
				print entityName

