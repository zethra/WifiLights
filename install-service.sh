#!/bin/sh

USER='mc'
GOBIN='/usr/local/go/bin/go'

if [ -e temp ]; then
    rm -r temp
fi

mkdir temp
cd temp
mkdir bin pkg src
export GOPATH=`pwd`
$GOBIN get github.com/zethra/WifiLights
sed -i "s/'<USER>'/$(printf "'$USER'")/g" src/github.com/zethra/WifiLights/wifiLights.sh

if [ -e /opt/wifiLights ]; then
    if [ "$(ls -A /opt/wifiLights)" ]; then
        rm -r /opt/wifiLights/*
    fi
else
    mkdir /opt/wifiLights
fi

if [ ! -e /opt/wifiLights/bin ]; then
    mkdir /opt/wifiLights/bin
fi

mv bin/WifiLights /opt/wifiLights/bin/wifiLights
cp -r src/github.com/zethra/WifiLights/web /opt/wifiLights/

mv src/github.com/zethra/WifiLights/wifiLights.sh /etc/init.d/wifiLights
chmod +x /etc/init.d/wifiLights
update-rc.d wifiLights defaults

if [ -e /var/log/wifiLights.log ]; then
    rm /var/log/wifiLights.log
fi

cd ..
if [ -e temp ]; then
    rm -r temp
fi
