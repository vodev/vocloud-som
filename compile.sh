#!/bin/bash
g++ src/SOM.cpp -o bin/som -fopenmp -O3 -Wno-unused-result
g++ src/errorCounter.cpp -o bin/errorCounter -O3 -Wno-unused-result
g++ src/neuronsDrawer.cpp -o bin/neuronsDrawer -O3 -Wno-unused-result
g++ src/neuronsDrawer2.cpp -o bin/neuronsDrawer2 -O3 -Wno-unused-result
g++ src/divider.cpp -o bin/divider -O3 -Wno-unused-result
