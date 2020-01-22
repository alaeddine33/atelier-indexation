#!/bin/bash

git clone git@github.com:SOCI/soci.git
mkdir build
cd build
cmake -G "Unix Makefiles" -DWITH_ORACLE=ON -DORACLE_INCLUDE_DIR="/opt/oracle/instantclient_12_2/sdk/include" -DORACLE_LIBRARIES="/usr/lib/oracle/12.2/client64" ../
make
sudo make install
