# run_ml_mpi takes a function func() and runs it, in parallel, on the inputs in the argument 'data', returning the results of each parallel run in the 'outputs' object

from mpi4py import MPI

def run_ml_mpi(func,data):
    
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()
    
    return func(data[rank])

