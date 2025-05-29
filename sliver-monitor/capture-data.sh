#!/bin/zsh

DATE=$(date +%Y_%m_%d_%I_%M_%p)
tail -f /dev/cu.usbmodem14101 | tee sliver_$DATE.csv
