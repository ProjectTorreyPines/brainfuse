#!/bin/bash
# A simple job submission script called hello.sbatch 
# It runs a parallel hello world job to demonstrate 
# submitting mpi jobs to the Slurm scheduler.
#
# Submit this to the Slurm scheduler this using the command:
#$ sbatch hello_world.sbatch

# NO bash commands can appear before the #SBATCH directives
# notice that we are setting our environment using modules below

## Specify the queue (a.k.a. the "p"artition)
#SBATCH -p short

## Specify the number of cores
#SBATCH -n 12

## further restrict your code to a specific number of nodes (computers)
## Only really necessary if you need shared memory, e.g. for OpenMP
## and a detriment if using MPI, b/c MPI can spread out over many nodes.
##SBATCH -N 1

####
## With time and memory, the less you request, the higher its priority 
## in the queue, but if you exceed your allotment, the job will be killed.
####

## Specify the amount of time for the job (in minutes)
#SBATCH -t 2
## or as -t days-HH:MM:SS, e.g. -t 7-00:00:00 for the long queue

## Specify the amount of memory required (M|G|T) per cpu:
#SBATCH --mem-per-cpu=100M

## Specify an output file -- Bash environment variables WILL NOT WORK HERE
## only a limited number or replacers are available, such as 
## %j = jobID 
## %u = username 
## %N = (first) node name 
#SBATCH -o hello_world-%j.out

## Optionally export your environment variables to the job,
## generally a good idea
#SBATCH --export=ALL

## Now run the actual job

## to make sure you have the correct environment loaded
## you can load relevant modules, otherwise be sure to have 
## the correct environment BEFORE running: $ sbatch efit.sbatch
module purge
module load conda/omfit
## module load gcc-4.7.2
##module load mpich/gnu

# mpi jobs are run with the $srun command instead of mpiexec
#
# Some legacy codes require the --mpi=pmi2 flag, it is probably not 
# necessary if your code has been recently compiled, but I have yet 
# to see it cause any problems if included. It is required for efit.
#
# be sure to run your MPI or OpenMP
mpirun -n 4 python hello_world_parallel.py
