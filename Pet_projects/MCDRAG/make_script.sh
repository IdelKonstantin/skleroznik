#!/bin/bash

gcc MC_DRAG.c -lm -o app && python3 serial_test.py
