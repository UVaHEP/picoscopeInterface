#!/usr/bin/python

import argparse,sys
import subprocess,time

#######################
# main
#######################

parser = argparse.ArgumentParser(description='Take dark pulse data')

parser.add_argument('-v','--voltage', type=float, default = 0,
                    help="starting voltage")
parser.add_argument('-s','--nsteps', type=int, default=0,
                    help="number of voltage steps")
parser.add_argument('-S','--stepsize', type=float, default = 0.0,
                    help="size of voltage steps")
parser.add_argument('-x', '--vmax', type=float, default = 0,
                    help="max voltage")
parser.add_argument('-0', '--zero', action='store_true',
                    help="zero voltage measure")
parser.add_argument('-o', '--output', type=str, default = "darkBuffers",
                    help="outputfile template")
args = parser.parse_args()

from ROOT import *

if args.voltage: voltage=args.voltage
if not args.voltage and not args.zero:
    print "No voltage given:",args.voltage
    sys.exit()
if args.zero: voltage=0
if  args.voltage<=0:
    print "Invalid voltage given:",args.voltage
    sys.exit()
vmax=args.vmax
outname=args.output

nsteps = args.nsteps
stepsize = args.stepsize


if nsteps>0:
    if vmax>0: stepsize=float(vmax-voltage)/nsteps
    elif stepsize==0:
        print "Can't determine voltage steps"
        sys.exit()
elif stepsize>0 and vmax>0:
    nsteps=int(round(float(vmax-voltage)/stepsize))
    

#loop over voltages
vend=vmax

tg=TGraphErrors()
tc=TCanvas("cgr")
tg.SetTitle("Dark pulse rate vs Voltage;V;MHz")

print "nsteps",nsteps

for i in range(nsteps+1):
    v=round(voltage+i*stepsize,3)
    # set voltage
    subprocess.call(["setVoltage.py","-pv"+str(v)])
    # takepulses
    filename=outname+"_"+str(v)+".root"
    print "Saving data to",filename
    subprocess.call(["darkBuffers","-qb5", "-o"+filename])
    tf=TFile(filename)
    darkRate=tf.Get("hRate").GetBinContent(1);
    error=tf.Get("hRate").GetBinError(1);
    tg.SetPoint(tg.GetN(),v,darkRate);
    tg.SetPointError(tg.GetN()-1,0,error);
    tg.Draw("ALP*")
    tc.Update()
    
subprocess.call(["setVoltage.py"])
tg.Draw("ALP")
time.sleep(2)
tc.SaveAs(outname+".pdf")


