
set -e

wget https://download.qt.io/archive/qt/5.5/5.5.1/single/qt-everywhere-opensource-src-5.5.1.tar.gz
tar xzvf qt-everywhere-opensource-src-5.5.1.tar.gz
cd qt-everywhere-opensource-src-5.5.1

#    -skip qtbase \
#    -skip qtimageformats \
#    -skip qtserialport \
#    -skip qttools \
#    -skip qttranslations \

./configure \
    -release \
    -opensource \
    -shared \
    -nomake examples \
    -nomake tests \
    -confirm-license \
    -prefix /opt/i686-w64-mingw32-qt-5.5.1 \
    -xplatform win32-g++ \
    -device-option CROSS_COMPILE=i686-w64-mingw32- \
    -skip qt3d \
    -skip qtactiveqt \
    -skip qtandroidextras \
    -skip qtcanvas3d \
    -skip qtconnectivity \
    -skip qtdeclarative \
    -skip qtdoc \
    -skip qtenginio \
    -skip qtgraphicaleffects \
    -skip qtlocation \
    -skip qtmacextras \
    -skip qtmultimedia \
    -skip qtquick1 \
    -skip qtquickcontrols \
    -skip qtscript \
    -skip qtsensors \
    -skip qtsvg \
    -skip qtwayland \
    -skip qtwebchannel \
    -skip qtwebengine \
    -skip qtwebkit \
    -skip qtwebkit-examples \
    -skip qtwebsockets \
    -skip qtwinextras \
    -skip qtx11extras \
    -skip qtxmlpatterns \
    -no-sql-db2 \
    -no-sql-ibase \
    -no-sql-mysql \
    -no-sql-oci \
    -no-sql-odbc \
    -no-sql-psql \
    -no-sql-sqlite2 \
    -no-sql-tds \
    -qt-sql-sqlite \
    -qt-zlib \
    -qt-libpng \
    -qt-libjpeg \
    -verbose

make -j5
sudo make install
