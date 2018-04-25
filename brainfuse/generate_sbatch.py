def generate_sbatch(python_script, serverPicker=None, sbatch_file=None, **kw):
    '''
    :param python_script: path to the python script to run

    :param serverPicker: server to submit to [if None uses localhost]

    :param nodes: number of nodes to use

    :param ntasks: number of processors per node to use

    :param mem: memory to use in GB

    :param python_command: python executable to use

    :param extra_opts: string with additional options to prepend before execution

    :param environ: environment to load

    :param sbatch_file: string of the path to the sbatch to file to write (if not None)

    :return: string with sbatch script
    '''

    if serverPicker is None:
        import socket
        serverPicker=socket.gethostname()

    import os
    if kw.get('name',None) is None:
        kw['name']=os.path.splitext(os.path.split(python_script)[1])[0]

    #========================
    # IRIS
    #========================
    if 'iris' in serverPicker:
        kw.setdefault('environ','''
module purge
module load conda/omfit
''')
        kw.setdefault('nodes',1)
        kw.setdefault('ntasks',16)
        kw.setdefault('mem',12)
        kw.setdefault('python_command','python -u')
        kw.setdefault('extra_opts','''
#SBATCH --gres=gpu:1
''')
        serverPicker='iris'

    #========================
    # SATURN
    #========================
    elif 'saturn' in serverPicker:
        kw.setdefault('environ','''
module purge
module load conda/omfit
''')
        kw.setdefault('nodes',1)
        kw.setdefault('ntasks',16)
        kw.setdefault('mem',12)
        kw.setdefault('python_command','python -u')

        if serverPicker=='saturn00':
            kw.setdefault('extra_opts','''
#SBATCH --partition=gpus
''')
        elif serverPicker=='saturn11':
            kw.setdefault('extra_opts','''
#SBATCH --partition=gpus
#SBATCH --gres=gpu:kepler:1
#SBATCH --nodelist=saturn11
''')
        elif serverPicker=='saturn12':
            kw.setdefault('extra_opts','''
#SBATCH --partition=gpus
#SBATCH --gres=gpu:kepler:1
#SBATCH --nodelist=saturn12
''')
        elif serverPicker=='saturn13':
            kw.setdefault('extra_opts','''
#SBATCH --partition=theorygpu
#SBATCH --gres=gpu:kepler:2
#SBATCH --nodelist=saturn13
''')
        elif serverPicker=='saturn14':
            kw.setdefault('extra_opts','''
#SBATCH --partition=theorygpu
#SBATCH --gres=gpu:kepler:2
#SBATCH --nodelist=saturn14
''')
        elif serverPicker=='saturn15':
            kw.setdefault('extra_opts','''
#SBATCH --partition=theorygpu
#SBATCH --gres=gpu:pascal:1
#SBATCH --nodelist=saturn15
''')
        elif serverPicker=='saturn16':
            kw.setdefault('extra_opts','''
#SBATCH --partition=theorygpu
#SBATCH --gres=gpu:pascal:1
#SBATCH --nodelist=saturn16
''')
        else:
            raise(ValueError('serverPicker `%s` not recognized'%serverPicker))
        serverPicker='saturn'
    #========================
    # server not recognized
    #========================
    else:
        raise(ValueError('serverPicker `%s` not recognized'%serverPicker))

    #build sbatch script
    kw['extra_opts']=kw['extra_opts'].strip()
    kw['environ']=kw['environ'].strip()
    sbatch_script='''#!/bin/bash -l
#SBATCH --job-name={name}
#SBATCH --output={name}.out
#SBATCH --nodes={nodes}
#SBATCH --ntasks={ntasks}
#SBATCH --mem={mem}GB

{extra_opts}

{environ}

{python_command} {python_script}
'''.format(python_script=python_script,**kw)

    #write sbtacth to file
    if sbatch_file:
        open(sbatch_file,'w').write(sbatch_script)

    return sbatch_script

############################################

if '__main__' in __name__:

    import sys
    print(generate_sbatch(*sys.argv[1:]))
