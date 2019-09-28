set -e

wget https://download.qt.io/archive/qt/5.9/5.9.5/single/qt-everywhere-opensource-src-5.9.5.tar.xz
tar xf qt-everywhere-opensource-src-5.9.5.tar.xz
cd qt-everywhere-opensource-src-5.9.5

./configure \
    -shared \
    -opensource \
    -opengl desktop \
    -nomake examples \
    -nomake tests \
    -confirm-license \
    -prefix /opt/i686-w64-mingw32-qt-5.9.5 \
    -xplatform win32-g++ \
    -device-option CROSS_COMPILE=i686-w64-mingw32- \
    -skip qtactiveqt \
    -skip qtandroidextras \
    -skip qtconnectivity \
    -skip qtdatavis3d \
    -skip qtdoc \
    -skip qtgamepad \
    -skip qtgraphicaleffects \
    -skip qtlocation \
    -skip qtmacextras \
    -skip qtnetworkauth \
    -skip qtpurchasing \
    -skip qtquickcontrols2 \
    -skip qtremoteobjects \
    -skip qtscript \
    -skip qtscxml \
    -skip qtsensors \
    -skip qtserialbus \
    -skip qtspeech \
    -skip qtvirtualkeyboard \
    -skip qtwayland \
    -skip qtwebchannel \
    -skip qtwebengine \
    -skip qtwebsockets \
    -skip qtwebview \
    -skip qtwinextras \
    -skip qtx11extras \
    -skip qtxmlpatterns \
    -no-openssl \
    -verbose

make -j5
sudo make install

