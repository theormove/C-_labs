// Grundlagen der Bioinformatik (Praktikum)
// Aufgabenblatt 05


#include <iostream>     // for cout, cin
#include <stdio.h>      // for NULL
#include <unistd.h>
#include <mpi.h>


int main(int argc, char *argv[])
{
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Get the rank of the process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// //printf("Rang %d aus %d\n", rank, size);

	MPI_Status status;
	const int TAG = 1;
	int message;
	const int ITERATIONS = size -1; 
	int * sum = new int[ITERATIONS+1];
	for (int j = 0; j < size; j++){
		sum[j] = 0;
	}
	int send_buffer = rank;	
		if (rank != size -1)
		{
			//process sends its rank number  to the one with rank number + 1
			MPI_Send(&send_buffer, 1, MPI_INT, rank+1, TAG, MPI_COMM_WORLD);
			//printf(" Rank %d sent %d .\n", rank, rank);
		}
		else 
		{	
			//one with rank number equal to n-1 sends its rank number to the process with rank 0 
			MPI_Send(&send_buffer, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
			//printf(" Rank %d sent %d .\n", rank, rank);
		}
		if (rank != 0)
		{
			//every process receives message from the one with rank number - 1
			MPI_Recv(&message, 1, MPI_INT, rank-1, TAG, MPI_COMM_WORLD, &status);
			//printf(" Rank %d received %d .\n", rank, message);
		}
		else
		{
			//one with rank number equal to 0 receives message from the process with rank n-1
			MPI_Recv(&message, 1, MPI_INT, size-1, TAG, MPI_COMM_WORLD, &status);
			//printf(" Rank %d received %d .\n", rank, message);
		}

		sum[rank] += message;		
	
	for (int i = 0; i < ITERATIONS; i++)
	{	
		send_buffer = message;
		if (rank != 0 && rank != size-1)
		{
			//process sends its buffer to the one with rank number + 1
			MPI_Send(&send_buffer, 1, MPI_INT, rank+1, TAG, MPI_COMM_WORLD);
			//printf(" Rank %d sent %d .\n", rank, rank);
		}
		else if(rank == size-1)
		{	
			//one with rank number equal to n-1 sends its buffer to the process with rank 0 
			MPI_Send(&send_buffer, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
			//printf(" Rank %d sent %d .\n", rank, rank);
		}
		if (rank != 1)
		{
			//every process receives message from the one with rank number - 1
			MPI_Recv(&message, 1, MPI_INT, rank-1, TAG, MPI_COMM_WORLD, &status);
			//printf(" Rank %d received %d .\n", rank, message);
		}
		else if(rank == 0)
		{
			//one with rank number equal to 0 receives message from the process with rank n-1
			MPI_Recv(&message, 1, MPI_INT, size-1, TAG, MPI_COMM_WORLD, &status);
			//printf(" Rank %d received %d .\n", rank, message);
		}
		sum[rank] += message;
		
				
		
	}

	std::cout<<"Process with rank "<< rank << "  "<<sum[rank]<<"\n";



	MPI_Finalize();

	return 0;
}
