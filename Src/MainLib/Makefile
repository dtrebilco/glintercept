include Makefile.config
TARGET=../Bin/libGL.so.1
SOURCES = \
BuiltInFunction.cpp \
ConfigData.cpp \
DisplayListManager.cpp \
DisplayListSaveManager.cpp \
DLLLoader.cpp \
EnumData.cpp \
ErrorLog.cpp \
ExtensionFunction.cpp \
FunctionData.cpp \
FunctionParser.cpp \
FunctionTable.cpp \
GLContext.cpp \
GLCore1_1.cpp \
GLCore1_2.cpp \
GLDriver.cpp \
GLIntercept.cpp \
GLStubs.cpp \
GLStubsWGL.cpp \
GLWindows.cpp \
ImageManager.cpp \
ImageSaveManager.cpp \
InterceptData.cpp \
InterceptDisplayList.cpp \
InterceptFrame.cpp \
InterceptImage.cpp \
InterceptLog.cpp \
InterceptLogText.cpp \
InterceptLogXML.cpp \
InterceptPluginDLLInstance.cpp \
InterceptPluginInstance.cpp \
InterceptPluginManager.cpp \
InterceptShader.cpp \
InterceptShaderGLSL.cpp \
MovieMaker.cpp \
ParameterData.cpp \
ShaderGLSLManager.cpp \
ShaderGLSLSaveManager.cpp \
ShaderManager.cpp \
ShaderSaveManager.cpp \
stdafx.cpp

OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

HEADERS=\
BuiltInFunction.h \
CommonErrorLog.h \
ConfigData.h \
DisplayListManager.h \
DisplayListSaveManager.h \
DLLLoader.h \
EnumData.h \
ErrorLog.h \
ExtensionFunction.h \
FunctionArgs.h \
FunctionData.h \
FunctionParser.h \
FunctionRetValue.h \
FunctionTable.h \
GLContext.h \
GLCore1_1.h \
GLCore1_2.h \
GLDriver.h \
gl.h \
GLInterceptConfig.h \
GLIntercept.h \
GLStubs.h \
GLStubsWGL.h \
GLWindows.h \
ImageManager.h \
ImageSaveManager.h \
InterceptData.h \
InterceptDataManager.h \
InterceptDisplayList.h \
InterceptFrame.h \
InterceptImage.h \
InterceptLog.h \
InterceptLogText.h \
InterceptLogXML.h \
InterceptPluginDLLInstance.h \
InterceptPluginInstance.h \
InterceptPluginInterface.h \
InterceptPluginManager.h \
InterceptShaderGLSL.h \
InterceptShader.h \
MovieMaker.h \
ParameterData.h \
ShaderGLSLManager.h \
ShaderGLSLSaveManager.h \
ShaderManager.h \
ShaderSaveManager.h \
stdafx.h

CXXFLAGS=$(CORONA_CFLAGS) -DGLI_BUILD_LINUX -I./ -I./Common -O0 -g3 -Wall -fmessage-length=0
LDFLAGS=-Wl,-Bstatic $(CORONA_LIBS) $(X11_LIBS) -lstdc++ -ldl -lpthread -Wl,-Bdynamic -lgcc_s

all:	common-all $(TARGET)

$(TARGET):	$(OBJECTS)
	$(CXX) -shared $(LDFLAGS) $(OBJECTS) Common/libCommon.a -o $@

$(OBJECTS):%.o: %.cpp $(HEADERS)
	$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

clean:	common-clean
	$(RM) $(OBJECTS) $(TARGET)

common-all:
	make -C Common all

common-clean:
	make -C Common clean
