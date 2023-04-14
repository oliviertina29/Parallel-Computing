from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    data = int(input("Enter a value: "))
    comm.send(data, dest=1)
    print(f"Process {rank} sent {data}")
elif rank == size-1:
    data = comm.recv(source=rank-1)
    data += rank
    comm.send(data, dest=0)
    print(f"Process {rank} received {data} from processor {rank-1}")
else:
    data = comm.recv(source=rank-1)
    data += rank
    comm.send(data, dest=rank+1)
    print(f"Process {rank} received {data} from processor {rank-1}")
