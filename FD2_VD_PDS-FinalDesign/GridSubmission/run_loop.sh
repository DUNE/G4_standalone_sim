#!/bin/bash
i_x=0.125
i_y=-2.875
i_z=18.00

f_x=6.625
f_y=2.875
f_z=20.5

step_x=0.25
step_y=0.25
step_z=0.25

for x in $(LC_ALL=en_US.UTF-8 seq $i_x $step_x $f_x) ; do
    for y in $(LC_ALL=en_US.UTF-8 seq $i_y $step_y $f_y) ; do
	    for z in $(LC_ALL=en_US.UTF-8 seq $i_z $step_z $f_z) ; do
            jobsub_submit -G dune -Q --role=Analysis -N 1 --expected-lifetime=2h --memory=500MB --disk=1GB --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --OS=SL7 file:///pnfs/dune/scratch/users/smanthey/vdriftAr/layouts/extended_4x20/run_point.sh $x $y $z
	    done
    done
done
