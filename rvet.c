/**
 * Código base (incompleto) para implementação de relógios vetoriais.
 * Meta: implementar a interação entre três processos ilustrada na figura
 * da URL: 
 * 
 * https://people.cs.rutgers.edu/~pxk/417/notes/images/clocks-vector.png
 * 
 * Compilação: mpicc -o rvet rvet.c
 * Execução:   mpiexec -n 3 ./rvet
 */
 
#include <stdio.h>
#include <string.h>  
#include <mpi.h>     


typedef struct Clock { 
   int p[3];
} Clock;


void Event(int pid, Clock *clock){
   clock->p[pid]++;   
}


void Send(int pid, Clock *clock){
   // TO DO
 MPI_Status status;
   int rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
   // Atualiza o relógio local
   clock->p[rank]++;
   
   // Envia mensagem
   MPI_Send(&clock, sizeof(Clock), MPI_BYTE, pid_destino, 0, MPI_COMM_WORLD);
}

void Receive(int pid, Clock *clock){
   // TO DO
 MPI_Status status;
   int rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
   // Recebe mensagem
   MPI_Recv(clock, sizeof(Clock), MPI_BYTE, pid_origem, 0, MPI_COMM_WORLD, &status);
   
   // Atualiza o relógio local
   clock->p[rank]++;
   
   // Atualiza o relógio do processo que enviou a mensagem
   clock->p[pid_origem]++;
}

// Representa o processo de rank 0
void process0(){
   Clock clock = {{0,0,0}};
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);

   // TO DO
 // Envia mensagem para processo 1
   Send(1, &clock);
   
   // Recebe mensagem de processo 2
   Receive(2, &clock);
   
   // Imprime relógio final
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);

}

// Representa o processo de rank 1
void process1(){
   Clock clock = {{0,0,0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);

   // TO DO
 // Recebe mensagem de processo 0
   Receive(0, &clock);
   
   // Atualiza relógio local
   Event(1, &clock);
   
   // Envia mensagem para processo 2
   Send(2, &clock);
   
   // Imprime relógio final
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
}

// Representa o processo de rank 2
void process2(){
   Clock clock = {{0,0,0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   
   // TO DO
 // Envia mensagem para processo 0
   Send(0, &clock);
   
   // Recebe mensagem de processo 1
	Receive(1, &clock);

	// Atualiza relógio local
	Event(2, &clock);

	// Imprime relógio final
	printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
}

int main(void) {
   int my_rank;               

   MPI_Init(NULL, NULL); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
      process0();
   } else if (my_rank == 1) {  
      process1();
   } else if (my_rank == 2) {  
      process2();
   }

   /* Finaliza MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
