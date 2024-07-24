#!/bin/bash
#SBATCH --job-name=AmBe
#SBATCH --output=output.log
#SBATCH --error=error.log
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

cd /home/amslivar/work/AmBe_v2/AmBe_GeSim_efficiency/AmBe.slurm

source /usr/local/geant4.10.04.p02/bin/geant4.sh
source /usr/local/geant4.10.04.p02/share/Geant4-10.4.2/geant4make/geant4make.sh
export AllowForHeavyElements=1

date
./GeSim AmBe.mac
date
