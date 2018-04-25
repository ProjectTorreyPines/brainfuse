# hello_world_MPI.py calls run_ml_mpi.py to output "Hello, world!" strings from parallel processes
 
from run_ml_mpi import *

def say_this(statement='Nothing to say'):
    return statement

things_to_say = ['Hello, world! This is process 1.','Hello, world! This is process 2.']

outputs = run_ml_mpi(say_this,things_to_say)

print(say_this(outputs[0]))
print(say_this(outputs[1]))

    



