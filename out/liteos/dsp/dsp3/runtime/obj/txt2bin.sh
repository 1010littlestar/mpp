#!/bin/sh
BIN=./bin
TARGET=$BIN/hi_dsp
DUMPELF=xt-dumpelf

$DUMPELF --base=0x42F08000 --width=8 --little-endian --default=0x00 --full $TARGET --size=0xf00000 >$BIN/hi_sram.txt
$DUMPELF --base=0x15100000 --width=8 --little-endian --default=0x00 --full $TARGET --size=0x40000 >$BIN/hi_dram0.txt
$DUMPELF --base=0x15210000 --width=8 --little-endian --default=0x00 --full $TARGET --size=0x30000 >$BIN/hi_dram1.txt
$DUMPELF --base=0x40000000 --width=8 --little-endian --default=0x00 --full $TARGET --size=0x8000 >$BIN/hi_iram0.txt
./ftxt2bin $BIN/hi_iram0.txt $BIN/hi_iram0.bin
./ftxt2bin $BIN/hi_dram0.txt $BIN/hi_dram0.bin
./ftxt2bin $BIN/hi_dram1.txt $BIN/hi_dram1.bin
./ftxt2bin $BIN/hi_sram.txt $BIN/hi_sram.bin

