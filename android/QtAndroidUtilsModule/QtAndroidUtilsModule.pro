TARGET = QtAndroidUtilsModule
TEMPLATE = lib

isEmpty(PREFIX) : !isEmpty(INSTALL_ROOT) : PREFIX=$$INSTALL_ROOT
isEmpty(PREFIX) {
    qnx : PREFIX = /tmp/$${TARGET}/bin
    else : android : PREFIX = /.
    else : PREFIX = $$OUT_PWD/../install
}

include(QtAndroidUtilsModule.pri)

# Default rules for deployment.
target.path = $$PREFIX
INSTALLS += target

headers_file.target = headers_file
headers_file.path = $$PREFIX/include
headers_file.files = $$INSTALL_HEADERS
INSTALLS += headers_file
