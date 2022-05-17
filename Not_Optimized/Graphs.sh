#!/bin/bash

echo "Cringe 1"
python3 ./src/Histogram.py ./Hash_Research/ Cringe-1

echo "ASCII Hash"
python3 ./src/Histogram.py ./Hash_Research/ ASCII-Hash

echo "Len Hash"
python3 ./src/Histogram.py ./Hash_Research/ Len-Hash

echo "Checksum"
python3 ./src/Histogram.py ./Hash_Research/ Checksum

echo "Ded Hash"
python3 ./src/Histogram.py ./Hash_Research/ Ded-Hash

echo "CRC 32"
python3 ./src/Histogram.py ./Hash_Research/ CRC-32

echo "SHA 256"
python3 ./src/Histogram.py ./Hash_Research/ SHA-256
