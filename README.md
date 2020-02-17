# NeuLAND_Veto
Simulation Code for the NeuLAND Veto Detector, a PhD project by C. A. Douma.

This is a version of R3BRoot (https://github.com/R3BRootGroup/R3BRoot) that contains
the software needed to explore the design and efficiency of the NeuLAND VETO detector.

In order to use it, one should first install FairSoft and FairRoot in the usual way
and than install this version of R3BRoot on top of it. To ensure compatibility with
FairRoot and FairSoft, one should use the versions of March 2016 (master branch)
on an Ubuntu 15 OS and use the ROOT version 5 build.

To perform VETO simulations, go to ./VetoSim/Exp/
and follow the instructions of the README.txt there to
perform one of the pre-defined simulations.

To customize your own simulation, go to R3BRoot/VetoSim/UI/
and adapt the parameters in SetParameters.cpp as you like.
Then use the control scripts in this folder to run your
simulation.

Some additional explanation is contained in R3BRoot/VetoSim/Manual.txt


How to cite this code:

DOI:

URL:


Associated Papers:

C. A. Douma et al., "Investigation of background reduction techniques for the NeuLAND neutron detector,"
Nuclear Instruments and Methods in Physics Research Section A, vol. 930, pp. 203 - 209 (2019)
URL: https://www.sciencedirect.com/science/article/pii/S016890021930395X

C. A. Douma, "Measurement of the Gamow-Teller states in 116Sb and 122Sb,"
PhD. Thesis, University of Groningen, Groningen, Netherlands (2019).
URL: https://www.rug.nl/research/portal/files/74454235/Complete_thesis.pdf

C. A. Douma et al., "Design studies for the NeuLAND VETO detector,"
Journal of Physics: Conference Series, vol. 1024, pp. 012027 (2018).
URL: https://iopscience.iop.org/article/10.1088/1742-6596/1024/1/012027


