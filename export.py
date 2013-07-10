#!/usr/bin/python

import shutil
import os

inputDir="./"
prefix="Z:\Software"
lib=os.path.join(prefix,"lib")
include=os.path.join(prefix,"include")
basename="UART"

for folder in ("UCA0 Release","UCA0 Debug","UCA1 Release","UCA1 Debug"):
    outname=basename+"_"+"_".join(folder.split())+".hza"
    outpath=os.path.join(lib,outname)
    inpath=os.path.join(inputDir,os.path.join(folder,basename+".hza"))
    print("Copying "+inpath+" to "+outpath)
    shutil.copyfile(inpath,outpath)

for file in ("UCA0_uart.h","UCA1_uart.h","UART_queue.h"):
    outpath=os.path.join(include,file)
    inpath=os.path.join(inputDir,file)
    print("Copying "+inpath+" to "+outpath)
    shutil.copyfile(inpath,outpath)

