#!/bin/bash

# This script generates pictures of produced secondary particles. Since this code
# is rougly written in the same way as the analysis code, this script, that
# controls this code, is also written according to the principle of the
# Full_Analysis.sh script.

############################################################################
# first check for the input:
if [[ -z "$1" ]]
then
echo '### NOTE: The commando sec requires a second input: a number to specify'
echo '### NOTE: which run you want to make pictures of. Hence call the script'
echo '### NOTE: like ./foto sec 3 or something, but not empty!'
echo ''
else

# re-generate the Inputs.root file:
cd ../../UI/
root -l -q SetParameters.cpp

# Now write our information into the graphics source code:
cd ../Source/Graphics/
sed -i "26 a Counter = $1;" ./GetSecondaryGraphics.cpp
sed -i '28 d' ./GetSecondaryGraphics.cpp

# But now we also need to put in the type of series.
# fortunately the analysis has produced a file AnalysisTag.txt
# in the Inputs.root-file which consists of exactly the right number:
cd ../../Data/Inputs/
if [ -f "./AnalysisTag.txt" ]
then
read Series < ./AnalysisTag.txt
cd ../../Source/Graphics/
sed -i "30 a Series = ${Series};" ./GetSecondaryGraphics.cpp
sed -i '32 d' ./GetSecondaryGraphics.cpp

# And now we can actually run the code:
find ./ -type f ! -name "*.*" -exec rm -rf {} \;
rm -rf ./*.o
cp ../../Compile/Graphics/./GetSecondaryGraphics ./Makefile
make clean
make -j 4
./GetSecondaryGraphics

else
echo ''
echo '### ERROR: You did not yet perform an analysis! Without that'
echo '### ERROR: this picture cannot be created! Run an analysis first!'
echo ''
fi

# Done!
fi
