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

typedef struct Clock
{
   int p[3];
} Clock;

void Event(int pid, Clock *clock)
{
   clock->p[pid]++;
}

void Send(int pid, Clock *clock)
{
   /* Envio da mensagem para o processo */
   MPI_Send(clock->p, 3, MPI_INT, pid, 0, MPI_COMM_WORLD);
}

void Receive(int pid, Clock *clock)
{
   /* Recepção da mensagem do processo*/
   int proc[3];
   proc[0] = clock->p[0];
   proc[1] = clock->p[1];
   proc[2] = clock->p[2];

   MPI_Recv(clock->p, 3, MPI_INT, pid, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   for (int i = 0; i < 3; i++)
   {
      if (proc[i] > clock->p[i])
      {
         clock->p[i] = proc[i];
      }
   }
}

// Representa o processo de rank 0
void process0()
{
   Clock clock = {{0, 0, 0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Send(1, &clock);
   Event(0, &clock);
   Receive(1, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Send(2, &clock);
   Event(0, &clock);
   Receive(2, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Send(1, &clock);
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
}

// Representa o processo de rank 1
void process1()
{
   Clock clock = {{0, 0, 0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   Event(1, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   Send(0, &clock);
   Event(1, &clock);
   Receive(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   Event(1, &clock);
   Receive(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
}

// Representa o processo de rank 2
void process2()
{
   Clock clock = {{0, 0, 0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Event(2, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Event(2, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Send(0, &clock);
   Event(2, &clock);
   Receive(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
}

int main(void)
{
   int my_rank;

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   if (my_rank == 0)
   {
      process0();
   }
   else if (my_rank == 1)
   {
      process1();
   }
   else if (my_rank == 2)
   {
      process2();
   }

   /* Finaliza MPI */
   MPI_Finalize();

   return 0;
} /* main */
