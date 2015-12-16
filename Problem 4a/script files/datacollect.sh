#!/bin/bash
timeout 11015 ./heavyiodatacollect.sh
timeout 11015 ./heavycpudatacollect.sh
timeout 11015 ./heavycpuiodatacollect.sh
