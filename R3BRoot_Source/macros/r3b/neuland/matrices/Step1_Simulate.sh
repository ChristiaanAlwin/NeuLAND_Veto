#!/bin/bash

# Set output directory from script arguments or use default
OUTDIR=${1:-output}
# Create a folder for root files, so this directory stays clean
mkdir -p ${OUTDIR}

NEVENTS=10000
DISTANCE=1400

# Kill background jobs if script is terminated
trap 'kill $(jobs -pr) 2>/dev/null' SIGINT SIGTERM EXIT

# Run all simulations in "parallel" in background
COUNT=0
for NDOUBLEPLANES in $(seq 4 50); do
	for NNEUTRONS in $(seq 1 4); do
		INPUT="$((132-${NNEUTRONS}))Sn_${NNEUTRONS}n_600AMeV_500keV.dat"
		COMMAND="Step1_Simulate.C(${NEVENTS}, \"${OUTDIR}\", \"${DISTANCE}cm_${NDOUBLEPLANES}dp_${NNEUTRONS}n\", \"${INPUT}\", \"v2_${DISTANCE}cm_${NDOUBLEPLANES}dp\")"
		# Note: The root call is extemely sensitive to the usage of ' and "
		nice -n 19 root -l -q -b -e 'gInterpreter->AddIncludePath("'${VMCWORKDIR}'")' "${COMMAND}" &> "${OUTDIR}/${DISTANCE}cm_${NDOUBLEPLANES}dp_${NNEUTRONS}n.sim.log" &
		echo ${COMMAND}

		# Only spawn so many processes at once
		COUNT=$((${COUNT}+1))
		if (( ${COUNT} % 30 == 0 )); then
			wait
		fi
	done
done

# Wait for all background jobs to finish
wait

# Remove junk
rm -f calor.out
rm -f flukaerr.dat
rm -f gphysi.dat
