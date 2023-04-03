#!/bin/bash

export CCDB_CONNECTION=sqlite:////work/clas12/rafopar/uRWELL/Readout/TT_Tests/clas12.sqlite
echo "Run = $1"

echo " "
echo " "
echo " ********************************"
echo "Running the Decoder"
#decoder -i /work/clas12/rafopar/uRWELL/Readout/APV25/urwell_00$1.evio.000$2 -o Data/decoded_$1_$2.hipo
#decoder -i /work/clas12/rafopar/uRWELL/Readout/APV25/urwell_00$1.evio_$2 -o Data/decoded_$1_$2.hipo 
#decoder -i /work/clas12/rafopar/uRWELL/Readout/APV25/urwell_00$1.evio.0000$2 -o Data/decoded_$1_$2.hipo
decoder -i /cache/clas12/detectors/uRwell/2023_HallData/urwell_00$1.evio.0000$2 -o Data/decoded_$1_$2.hipo



echo " "
echo " "
echo " ********************************"
echo "Running the Check Decoding"
./CheckDecoding.exe $1 $2

echo " "
echo " "
echo " ********************************"
echo "Running DrawPedestals"


root -l -q "DrawPedestals.cc($1)"


echo " "
echo " "
echo " ********************************"
echo "Running Skim_Cross"
./Skim_Cross.exe $1 $2



echo " "
echo " "
echo " ********************************"
echo "Running Event Selection from EVIO"
./AnalyzeData.exe $1 $2

#./evio_event_select.exe $1
