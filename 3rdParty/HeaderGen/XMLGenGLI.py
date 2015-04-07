#!/usr/bin/env python
#-*- coding: ascii -*-

from __future__ import print_function
try:
  from StringIO import StringIO
except:
  from io import StringIO
    
def open_ensure_dir(f,flags):
  d = os.path.dirname(f)
  if not os.path.exists(d):
    os.makedirs(d)
  return open(f,flags)

  
def get_xml_text(nodelist):
  rc = []
  for node in nodelist:
    if node.nodeType == node.TEXT_NODE:
      rc.append(node.data)
    if node.childNodes:
      rc.append(get_xml_text(node.childNodes))
                   
  return ''.join(rc)

  
class Enum:
  def __init__(self, name, value):
    self.name = name
    self.value = value
    self.used = False
  
  
class Command:
  def __init__(self, name, value):
    self.name = name
    self.value = value
    self.used = False

  
class Extension:
  def __init__(self, name):
    self.name = name
    self.enums = []
    self.commands = []

    
def create_include_string(extension, enumLookup, commandLookup):

  header_str = StringIO()
  header_str.write("#define GLI_INCLUDE_%s\n\n" % (extension.name.upper()))

  enums = extension.enums;
  commands = extension.commands;

  # Abort if no enums / commands
  if len(enums) == 0 and len(commands) == 0:
    return ""

  if len(enums) > 0:
    header_str.write("enum Main {\n\n")
    
    enumLength  = 0
    for enum in enums:
      if enumLength < (len(enum) + 5):
        enumLength = (len(enum) + 5)
    
    for enum in enums:
      enumValue = enumLookup[enum].value
      if enumValue[0:2] != "0x" and enumValue != "0" and enumValue != "1":
        header_str.write("//");
      header_str.write("  %s  = %s,\n" % (enum.ljust(enumLength), enumValue))  
      
    header_str.write("\n};\n\n")    
    
  for command in commands:
    commandStr = commandLookup[command].value
    header_str.write("%s\n" % (commandStr))
    
  return header_str.getvalue()
  
  
def create_extension_header(enums, commands, extensions, base_dir):

  for extension in extensions.items():
    includeFileStr = create_include_string(extension[1], enums, commands)
    if len(includeFileStr) > 0:
      ext = extension[1].name
      prefix = ext[0 : ext.find("_")]

      extNoPreFix = ext[ext.find("_")+1 : ]
      postPrefix = extNoPreFix[0 : extNoPreFix.find("_")]
      postPost = extNoPreFix[extNoPreFix.find("_")+1 : ]
      
      if prefix.lower() == "gl" :
        if postPrefix.lower() == "version":
          header_file = open_ensure_dir("%s/gli/GLCore/GLCore%s_Include.h" % (base_dir, postPost.lower()), "w")      
        elif postPrefix.lower() == "es":
          header_file = open_ensure_dir("%s/gli/GLCore/GLES_%s_Include.h" % (base_dir, postPost.lower()), "w")      
        elif postPrefix.upper() == "ARB" or postPrefix.upper() == "ATI" or postPrefix.upper() == "NV" :
          header_file = open_ensure_dir("%s/gli/%s/%s_%s_Include.h" % (base_dir, postPrefix.upper(), postPrefix.upper(), postPost.lower()), "w")
        else:  
          header_file = open_ensure_dir("%s/gli/Misc/%s_%s_Include.h" % (base_dir, postPrefix.upper(), postPost.lower()), "w")      
      elif prefix.lower() == "glx" :
        header_file = open_ensure_dir("%s/gli/XGL/XGL_%s_%s_Include.h" % (base_dir, postPrefix.upper(), postPost.lower()), "w")
      else :
        header_file = open_ensure_dir("%s/gli/%s/%s_%s_%s_Include.h" % (base_dir, prefix.upper(), prefix.upper(), postPrefix.upper(), postPost.lower()), "w")
      
      header_file.write(includeFileStr)
      header_file.close()

  
def get_extension(extensionTag, enums, commands) :    
  extension = Extension(extensionTag.getAttribute("name"))

  for requireTag in extensionTag.getElementsByTagName("require"):  
    apiStr = requireTag.getAttribute("api")
    if ((apiStr == "") or (apiStr == "gl")) : 
      for enumTag in requireTag.getElementsByTagName("enum"):  
        enumName = enumTag.getAttribute("name")
        
        # Only allow an enum to belong to one extension
        if enums[enumName].used == False: 
          enums[enumName].used = True
          extension.enums.append(enumName)
        
      for commandTag in requireTag.getElementsByTagName("command"):  
        commandName = commandTag.getAttribute("name")
        
        # Only allow an command to belong to one extension      
        if commands[commandName].used == False:
          commands[commandName].used = True
          extension.commands.append(commandName)    
      
  return extension  

  
def parse_xml(dom,base_dir) :

  # Get the  enums 
  enum_set = {}  
  for enumerationsTag in dom.documentElement.getElementsByTagName("enums"):
    for enumTag in enumerationsTag.getElementsByTagName("enum"):
      enumName = enumTag.getAttribute("name")
      enumValue = enumTag.getAttribute("value")
      enum_set[enumName] = Enum(enumName,enumValue);

  # Get the commands
  command_set = {}  
  for commandsTag in dom.documentElement.getElementsByTagName("commands"):
    for commandTag in commandsTag.getElementsByTagName("command"):
      
      # Get the command name and return type
      protoTag = commandTag.getElementsByTagName("proto")[0]
      commandName = protoTag.getElementsByTagName("name")[0].firstChild.data
      commandStr = get_xml_text(protoTag.childNodes);
      
      commandStr += "("
      
      # Get the parameters
      i = 0
      paramList = commandTag.getElementsByTagName("param");
      for i, paramTag in enumerate(paramList):
        commandStr += get_xml_text(paramTag.childNodes)
        if i != len(paramList) - 1:
          commandStr += ', '
          
      if commandStr == "(" :  
        commandStr += "void"
      commandStr += ");"        
      
      # Use main enum scope for all enums
      commandStr = commandStr.replace("GLenum", "GLenum[Main]")

      # Put the command in the lookup set      
      command_set[commandName] = Command(commandName, commandStr)
  
  # Get the extensions / features
  extension_set = {}
  for featuresTag in dom.documentElement.getElementsByTagName("feature"):
    apiStr = featuresTag.getAttribute("api")
    if ((apiStr == "") or (apiStr == "gl")) :
      extension = get_extension(featuresTag, enum_set, command_set)
      extension_set[extension.name] = extension

  for extensionsTag in dom.documentElement.getElementsByTagName("extension"):
    apiStr = extensionsTag.getAttribute("api")
    if ((apiStr == "") or (apiStr == "gl")) :
      extension = get_extension(extensionsTag, enum_set, command_set)
      extension_set[extension.name] = extension
        
  create_extension_header(enum_set, command_set, extension_set, base_dir)  
 
      
def auto_gen_header_files(base_dir):
  from xml.dom.minidom import parse
  parse_xml(parse(base_dir + "/gl.xml"), base_dir) 
  parse_xml(parse(base_dir + "/wgl.xml"), base_dir) 
  #parse_xml(parse(base_dir + "/glx.xml"), base_dir) 
  
if __name__ == "__main__":
  import os
  import sys
  auto_gen_header_files(os.path.dirname(sys.argv[0]))
