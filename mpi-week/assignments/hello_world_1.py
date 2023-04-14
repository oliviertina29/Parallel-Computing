from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

if rank == 0:
    print("Hello World from controller process")
else:
    print("Hello World from worker process %d" % rank)

MPI.Finalize()
