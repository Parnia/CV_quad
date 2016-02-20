#!/bin/bash
echo odroid | sudo -S chmod 777 /dev/video*
echo odroid | sudo -S chmod 777 /dev/ttySAC0
/home/odroid/quad_send/build/quad_send
