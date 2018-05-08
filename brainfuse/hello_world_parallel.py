# hello_world_MPI.py calls run_ml_mpi.py to output "Hello, world!" strings from parallel processes
 
from master_slave import *
import time

def say_this(statement='Nothing to say'):
    return statement + ', world!'

things_to_say = ['Hello','Ciao','Hola','Bonjour','Aloha','Namaste']

results = master_slave(things_to_say,say_this)

