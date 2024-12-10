# AmBe_GeSim_efficiency

AmBe simulations of gammas and neutrons in UA's GeIII detector based on GeSim code by Raymond Tsang. Read https://github.com/rhmtsang/GeSim/blob/master/README.md for more information about GeSim. 

Prerequisite: Geant 4.10.04.p02 and ROOT 5.34/07 (later versions are ok)

Current author: Ana Maria Slivar, anamaria.sslivar@gmail.com 
Original author of AmBe_GeSim_efficiency: Mariia Fedkevych, mariia.fedkevych@gmail.com 

1. Source setup scripts for Geant4, e.g.

```
source /usr/local/geant4.10.04.p02/bin/geant4.sh
source /usr/local/geant4.10.04.p02/share/Geant4-10.4.2/geant4make/geant4make.sh
```

2. Go to the GeSim_AmBe_efficiency folder and run `cmake .`:
3. Compile with `make -f Makefile`
4. Before starting the simulations, run `export AllowForHeavyElements=1`
5. The simulations can be executed by running `./GeSim AmBe.mac`
    - depending on the geometry you want to use, you will need to replace the content of src/gesimPrimaryGeneratorAction.cc with the content of
        - gesimPrimaryGeneratorAction.cc_no_outer_W_remote for simulations of AmBe source without the outer tungsten capsule in remote position at GeIII,
        - gesimPrimaryGeneratorAction.cc_with_outer_W_remote for simulations of AmBe source in the outer tungsten capsule in remote position at GeIII,
        - gesimPrimaryGeneratorAction.cc_with_outer_W_on_endcap for simulations of AmBe source in the outer tungsten capsule centered on the endcap face of GeIII
        - gesimPrimaryGeneratorAction.cc_oneE for only one energy peak 
        - gesimPrimaryGeneratorAction.cc for a spectrum of energies, created from step 6 in AmBe_emergent_gammas 
      
      and indicate the correct geometry name in the "AmBe.mac" file in "/gesim/source/setSourcePosition": 
        - LZAmBe2Remote for simulations of AmBe source without the outer tungsten capsule in remote position at GeIII,
        - LZAmBe2inOuterWCapsuleRemote for simulations of AmBe source in the outer tungsten capsule in remote position at GeIII,
        - LZAmBe2inOuterWCapsuleEndcap for simulations of AmBe source in the outer tungsten capsule centered on the endcap face of GeIII.
    - energy of primary gammas can be changed in src/gesimPrimaryGeneratorAction.cc
    - when you need to switch between geometries, or change generated gamma energy in gesimPrimaryGeneratorAction.cc etc, you will need to re-make by running `make clean` and then `make`
    - if you log out, make sure that steps 2 and 5 are execured before rtrying to run simulations again
6. The output root file containing energy distributions of primary and detected gammas will be generated within the same folder (the name is set in AmBe.mac). 
7. If you didn't comment out the visualization options in AmBe.mac, it will generate .wrl file in the same folder. 
8. To visualize origins of the primary gammas, copy the content of Particle_position_VRML.txt produced during the simulation at the end of the .wrl file. Example .wrl files which visualize all 3 geometries are contained within "geometry_vis_example" subfolder
