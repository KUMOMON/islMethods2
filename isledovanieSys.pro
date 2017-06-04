TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    methods.cpp \


win32 {

    INCLUDEPATH = D:/Work/GitHubProjects/islMethods2/islMethods2/islMethods2/headers
    LIBS += D:/Work/GitHubProjects/islMethods2/islMethods2/islMethods2/libs/libxl.dll

    QMAKE_POST_LINK +=$$quote(cmd /c copy /y D:\Work\GitHubProjects\islMethods2\islMethods2\islMethods2\libs\libxl.dll)

}
