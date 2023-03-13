#!/bin/bash

export CCDB_CONNECTION=sqlite:////work/clas12/rafopar/uRWELL/Readout/TT_Tests/clas12.sqlite
echo "Run = $1"

echo " "
echo " "
echo " ********************************"
echo "Running the Decoder"
decoder -i /work/clas12/rafopar/uRWELL/Readout/APV25/urwell_00$1.evio.00000 -o Data/decoded_$1.hipo -n 2000

echo " "
echo " "
echo " ********************************"
echo "Running the Check Decoding"
./CheckDecoding.exe $1

echo " "
echo " "
echo " ********************************"
echo "Running Event Selection from EVIO"
#./evio_event_select.exe $1
