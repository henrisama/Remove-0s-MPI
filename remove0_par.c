/* 
Aluno: Pedro Henrique Pires Flores
Aluno: Matheus Lopo Borges
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int 
  n, 
  m,
  idProcesso, 
  nProcessos,
  tam_proc,
  *vIn,
  *vOut,
  tamanho_vOut;

double 
  tIni,
  tFim;

void remove0()
{
  MPI_Status status;

  if(idProcesso == 0)
  {
    int 
      i,
      j,
      out[tam_proc],
      out_len;

    tamanho_vOut = 0;

    for (i = 0; i < tam_proc; i++)
    {
      if(vIn[i] != 0)
      {
        vOut[tamanho_vOut] = vIn[i];
        tamanho_vOut++;
      }
    }
    
    for (i = 1; i < nProcessos; i++)
    {
      MPI_Recv(out, 10, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status, MPI_INT, &out_len);
 
      if(out_len > 0){
        for (j = 0; j < out_len; j++)
        {
          vOut[tamanho_vOut] =  out[j];
          tamanho_vOut++;
        }
      }
    }
    
  }
  else
  {
    int
      indice, 
      i,
      c,
      len, 
      out[tam_proc]; 

    indice = idProcesso * tam_proc;
    len = indice+tam_proc;
    c = 0;

    for (i = indice; i < len; i++)
    {
      if(vIn[i] != 0)
      {
        out[c] = vIn[i];
        c++;
      }
    }

    MPI_Send(out, c, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
}

void inicializa(char** argv)
{
  FILE *arqIn;
  // Abre arquivo de entrada
  arqIn = fopen(argv[1], "rt");

  if (arqIn == NULL)
	{
		printf("\nArquivo texto de entrada não encontrado\n");
		exit(1);
	}

  // Lê tamanho do vetor de entrada
  fscanf(arqIn, "%d", &n);

  // n / p : aloca o tamanho do vetor de cada processo
  tam_proc = n / 10;

  // Aloca vetores de entrada e saída
	vIn  = (int *) malloc(n * sizeof(int));
	vOut = (int *) malloc(n * sizeof(int));

  if ((vIn == NULL) || (vOut == NULL))
	{
		printf("\nErro na alocação de estruturas\n");
		exit(1) ;
	}

  // Lê vetor do arquivo de entrada
  int i;
	for (i = 0; i < n; i++)
	{
		fscanf(arqIn, "%d", &(vIn[i]));
	}

	// Fecha arquivo de entrada
	fclose(arqIn);
}

void liberaVetor()
{
  free(vIn);
  free(vOut);
}

int main(int argc, char **argv)
{
  FILE *arqOut;

  if(argc != 3)
	{
		printf("O programa foi executado com argumentos incorretos.\n") ;
		printf("Uso: ./remove0_seq arquivo_entrada arquivo_saida\n") ;
		exit(1) ;
	}

  MPI_Init(&argc, &argv);
  
  inicializa(argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &idProcesso);
  MPI_Comm_size(MPI_COMM_WORLD, &nProcessos);

  tIni = MPI_Wtime();
  remove0();
  tFim = MPI_Wtime();
  

  if(idProcesso == 0)
  {
    // Cria arquivo de saída
    arqOut = fopen(argv[2], "wt");

    // Escreve tamanho do vetor de saída
	  fprintf(arqOut, "%d\n", tamanho_vOut);

    int i;
    for (i = 0; i < tamanho_vOut; i++)
    {
      fprintf(arqOut, "%d ", vOut[i]);
    }
    fprintf(arqOut, "\n");

    // Fecha arquivo de saída
	  fclose(arqOut);

    printf("Tempo=%lf\n", tFim - tIni);
  }

  liberaVetor();
  MPI_Finalize();
  return 0;
}
