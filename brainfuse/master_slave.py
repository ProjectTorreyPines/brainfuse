# run_ml_mpi takes a function func() and runs it, in parallel, on the inputs in the argument 'data', returning the results of each parallel run in the 'outputs' object
from __future__ import division
from __future__ import print_function

from mpi4py import MPI
import numpy as np

def enum(*sequential, **named):
    """Handy way to fake an enumerated type in Python
    http://stackoverflow.com/questions/36932/how-can-i-represent-an-enum-in-python
    """
    enums = dict(zip(sequential, range(len(sequential))), **named)
    return type('Enum', (), enums)

def master_slave(tasks = [], func = None):

    # Define MPI message tags
    tags = enum('READY', 'DONE', 'EXIT', 'START')

    # Initializations and preliminaries
    comm = MPI.COMM_WORLD   # get MPI communicator object
    size = comm.size        # total number of processes
    rank = comm.rank        # rank of this process
    status = MPI.Status()   # get MPI status object

    if rank == 0:
        # Master process executes code below
        #tasks = ['hello','ciao','aloha']*10
        results = [None]*len(tasks)
        task_index = 0
        num_workers = size - 1
        closed_workers = 0
        print("Master starting with %d workers" % num_workers)
        while closed_workers < num_workers:
            data = comm.recv(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=status)
            source = status.Get_source()
            tag = status.Get_tag()
            if tag == tags.READY:
                # Worker is ready, so send it a task
                if task_index < len(tasks):
                    comm.send([tasks[task_index],task_index], dest=source, tag=tags.START)
                    print("Sending task %d to worker %d" % (task_index, source))
                    task_index += 1
                else:
                    comm.send(None, dest=source, tag=tags.EXIT)
            elif tag == tags.DONE:
                results[data[-1]] = data[0]
                #results.append( data )
                print("Got data from worker %d" % source)
            elif tag == tags.EXIT:
                print("Worker %d exited." % source)
                closed_workers += 1

        print("Master finishing")
        print(results)
        return results
    else:
        # Worker processes execute code below
        name = MPI.Get_processor_name()
        print("I am a worker with rank %d on %s." % (rank, name))
        while True:
            comm.send(None, dest=0, tag=tags.READY)
            task = comm.recv(source=0, tag=MPI.ANY_TAG, status=status)
            tag = status.Get_tag()

            if tag == tags.START:
                # Do the work here
                indx = task[-1] # keep track of order of inputs
                task = task[0]
                result = func(task)
                comm.send([result,indx], dest=0, tag=tags.DONE)
            elif tag == tags.EXIT:
                break

        comm.send(None, dest=0, tag=tags.EXIT)

if __name__ == '__main__':
    # master_slave.py executed as script
    master_slave()
