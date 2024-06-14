//---------------------------------------------------------------------
// Arquivo      : evalenum.c
// Conteudo     : avaliacao de desempenho de algoritmos de ordenacao 
// Autor        : Wagner Meira Jr. (meira@dcc.ufmg.br)
// Historico    : 2023-11-04 - arquivo criado
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define ALGINSERTION 1
#define ALGSELECTION 2
#define ALGQSORT     3
#define ALGQSORT3    4
#define ALGQSORTINS  5
#define ALGQSORT3INS 6
#define ALGSHELLSORT 7
#define ALGRECSEL    8
#define ALGBOLHA     9
#define ALGMERGESORT 10  
#define ALGCOUNTINGSORT 11  
#define ALGBUCKETSORT 12  
#define ALGRADIXSORT 13  

typedef struct alg{
  int num;
  char * name;
} alg_t;

alg_t algvet[]={
  {ALGINSERTION,"i"},
  {ALGSELECTION,"s"},
  {ALGQSORT,"q"},
  {ALGQSORT3,"q3"},
  {ALGQSORTINS,"qi"},
  {ALGQSORT3INS,"q3i"},
  {ALGSHELLSORT,"h"},
  {ALGRECSEL,"rs"},
  {ALGBOLHA,"b"},
  {ALGMERGESORT, "m"}, 
  {ALGCOUNTINGSORT, "c"}, 
  {ALGBUCKETSORT, "bu"}, 
  {ALGRADIXSORT, "rx"}  
};

int name2num(char * name){
  int i=0;
  while (algvet[i].num){
    if (!strcmp(algvet[i].name,name)) return algvet[i].num;
    i++;
  }
  return 0;
}

char * num2name(int num){
  int i=0;
  while (algvet[i].num){
    if (algvet[i].num==num) return algvet[i].name;
    i++;
  }
  return 0;
}

typedef struct opt{
  int size;
  int seed;
  int alg;
  int type;
} opt_t;

typedef struct sortperf{
  int cmp;
  int move;
  int calls;
} sortperf_t;

void resetcounter(sortperf_t * s){
// Descricao: inicializa estrutura
// Entrada: 
// Saida: s
  s->cmp = 0;
  s->move = 0;
  s->calls = 0;
}

void inccmp(sortperf_t * s, int num){
// Descricao: incrementa o numero de comparacoes em num 
// Entrada: s, num
// Saida: s
  s->cmp += num;
}

void incmove(sortperf_t * s, int num){
// Descricao: incrementa o numero de movimentacoes de dados em num 
// Entrada: s, num
// Saida: s
  s->move += num;
}

void inccalls(sortperf_t * s, int num){
// Descricao: incrementa o numero de chamadas de função em num 
// Entrada: s, num
// Saida: s
  s->calls += num;
}

char * printsortperf(sortperf_t * s, char * str, char * pref){
// Descricao: gera string com valores de sortperf 
// Entrada: s, pref
// Saida: str

 sprintf(str,"%s cmp %d move %d calls %d", pref, s->cmp, s->move, s->calls); 
 return str;
}


void initVector(int * vet, int size, int tipoDeVetor){
// Descricao: inicializa vet com valores aleatorios ou ordenados
// 0: Aleatório   1: Crescente  2: Decrescente
// Entrada: vet
// Saida: vet
  int i;
  // inicializa a parte alocada da vetor com valores aleatorios
  if(tipoDeVetor == 0){
    for (i=0; i<size; i++){
      vet[i] = (int)(drand48()*size);
    }
  }
  else if(tipoDeVetor == 1){
   for (i=0; i<size; i++){
      vet[i] = i;
   }
  }
  else if(tipoDeVetor == 2){
   for (i=0; i<size; i++){
      vet[i] = size-i;
   }
  }
}
void printVector(int * vet, int size){
// Descricao: inicializa vet com valores aleatorios
// Entrada: vet
// Saida: vet
  int i;
  for (i=0; i<size; i++){
    printf("%d ",vet[i]);
  }
  printf("\n");
}

void swap(int *xp, int *yp, sortperf_t *s){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
    incmove(s,3);
}
//bubble sort

void bolha(int *v, int n, sortperf_t *s) {
  inccalls(s,1);
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
          inccmp(s,1);
            if (v[j] > v[j + 1]) {
                swap(&v[j], &v[j + 1],s);
            }
        }
    }
}
//mergesort
void merge(int arr[], int l, int m, int r, sortperf_t *s)
{
    inccalls(s,1);
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    int L[n1], R[n2];

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++) {
        L[i] = arr[l + i];
        incmove(s, 1); // Increment move count for copying data
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
        incmove(s, 1); // Increment move count for copying data
    }

    // Merge the temp arrays back into arr[l..r]
    i = 0;
    j = 0;
    k = l;
  
    while (i < n1 && j < n2) {
        inccmp(s, 1); // Increment comparison count
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
        incmove(s, 1); // Increment move count for merging
    }

    // Copy the remaining elements of L[], if there are any
   
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
        incmove(s, 1); // Increment move count for copying remaining elements
    }

    // Copy the remaining elements of R[], if there are any
 
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        incmove(s, 1); // Increment move count for copying remaining elements
    }
}

void mergeSort(int arr[], int l, int r, sortperf_t *s)
{
   inccalls(s, 1); 
    if (l < r) {
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m, s);
        mergeSort(arr, m + 1, r, s);

        merge(arr, l, m, r, s);

    }
}

//coutingsort
int pegarmaximoimo(int *vetor, int tamanho)
{
  int maximo = vetor[0];
  for (int i = 1; i < tamanho; i++)
    if (vetor[i] > maximo)
      maximo = vetor[i];
  return maximo;
}

void countingsort(int *values, int n,sortperf_t *s) {
  inccalls(s,1);
  int y=pegarmaximoimo(values, n);
int *counts = (int *) calloc(y, sizeof(int));
int i, j;
for (i = 0; i < n; i++)
counts[values[i]]++;
i = 0;
for(j = 0; j < y; j++)
while(counts[j] > 0) {
values[i++] = j;
counts[j]--;
incmove(s, 1);
}
free(counts);
}
//bucketsort
void bucketSort(int *vetor, int tamanho,sortperf_t *s)
{
  inccalls(s,1);
  int balde[tamanho+1];

  const int maximo = pegarmaximoimo(vetor, tamanho);
  for (int i = 0; i <= maximo; i++)
  {
    balde[i] = 0;
  }
  for (int i = 0; i < tamanho; i++)
  {
    incmove(s, 1);
    balde[vetor[i]]++;
  }
  for (int i = 0, j = 0; i <= maximo; i++)
  {
    while (balde[i] > 0)
    {
      incmove(s, 1);
      vetor[j++] = i;
      balde[i]--;
    }
  }
}
//radixsort
int getMax(int array[], int n) {
  int max = array[0];
  for (int i = 1; i < n; i++)
    if (array[i] > max)
      max = array[i];
  return max;
}
void radixsort(int array[], int size, sortperf_t *s) {
    inccalls(s,1);
    int max = getMax(array, size);
    countingsort(array, size, s); // Chama countingsort uma vez
}



// shellsort
void shellSort(int *A, int n, sortperf_t * s) {
  inccalls(s, 1);
  for (int h=n/2; h>0; h/=2){
    for (int i=h; i<n; i++){
      int aux=A[i];
      int j;
      incmove(s, 1);
      inccmp(s,1);
      for (j=i; j>=h && A[j-h]>aux; j-=h){
        A[j]=A[j-h];
        incmove(s, 1);
        inccmp(s,1);
      }
      A[j]=aux;
      incmove(s, 1);
    }
  }
  return;
}


// recursive selection sort
void recursiveSelectionSort(int arr[], int l, int r, sortperf_t * s)
{
    // find the minimum element in the unsorted subarray `[i…n-1]`
    // and swap it with `arr[i]`
    int min = l;
    inccalls(s,1);
    for (int j = l + 1; j <= r; j++)
    {
        // if `arr[j]` is less, then it is the new minimum
	inccmp(s,1);
        if (arr[j] < arr[min]) {
            min = j;    // update the index of minimum element
        }
    }

    // swap the minimum element in subarray `arr[i…n-1]` with `arr[i]`
    if (min!=l)
      swap(&arr[min], &arr[l], s);

    if (l + 1 < r) {
        recursiveSelectionSort(arr, l + 1, r, s);
    }
}

// selection sort
void selectionSort(int arr[], int l, int r, sortperf_t * s) { 
  inccalls(s, 1);
  for(int i=l; i<r-1; i++){
  int min=i;
    for (int j=i+1; j<r; j++){
      inccmp(s,1);
      if(arr[j]<arr[min]){
        min=j;
      }
    } 
   if(min!=i){
      swap(&arr[i], &arr[min], s);
    }
  }
  return;
}

//insertion sort

void insertionSort(int v[], int l, int r, sortperf_t * s) {
  inccalls(s, 1);
  int j,aux;
for(int i=l+1; i<=r; i++){
  aux= v[i];
  j=i-1;
  incmove(s, 1);
  inccmp(s,1);
  while(j>=0 && aux<v[j]){
    v[j+1]=v[j];
    j--;
    incmove(s, 1);
    inccmp(s,1);
  }
  v[j+1]=aux;
  incmove(s, 1);
}
return;
}

  
// median of 3 integers
int median (int a, int b, int c) {
    if ((a <= b) && (b <= c)) return b;  // a b c
    if ((a <= c) && (c <= b)) return c;  // a c b
    if ((b <= a) && (a <= c)) return a;  // b a c
    if ((b <= c) && (c <= a)) return c;  // b c a
    if ((c <= a) && (a <= b)) return a;  // c a b
    return b;                            // c b a
}

// quicksort partition using median of 3
void partition3(int * A, int l, int r, int *i, int *j, sortperf_t *s) {
  inccalls(s,1);
  *i=l;
  *j=r;
  int x= median(A[l], A[(l+r)/2], A[r]);
  do{
     inccmp(s,1);
    while(x>A[*i]){
       inccmp(s,1);
      (*i)++;
    }
    while(x<A[*j]){
       inccmp(s,1);
      (*j)--;
    }
    inccmp(s,1);
    if(*i<=*j){
      swap(&A[*i],&A[*j],s);
      (*i)++;
      (*j)--;
    }
  }while(*i<=*j);

}

// standard quicksort partition
void partition(int * A, int l, int r, int *i, int *j, sortperf_t *s) {
  inccalls(s,1);
  *i=l;
  *j=r;
  int x= A[(l+r)/2];
  do{
    inccmp(s,1);
    while(x>A[*i]){
       inccmp(s,1);
      (*i)++;
    }
    while(x<A[*j]){
       inccmp(s,1);
      (*j)--;
    }
    inccmp(s,1);
     if(*i<=*j){
      swap(&A[*i],&A[*j],s);
      (*i)++;
      (*j)--;
    }
  }while(*i<=*j);
}

// standard quicksort
void quickSort(int * A, int l, int r, sortperf_t *s) {
  inccalls(s,1);
  int i = l; 
  int j = r;
  partition(A,l,r,&i,&j,s);
  if (l<j) quickSort(A,l,j,s);
  if (r>i) quickSort(A,i,r,s);
}

// quicksort with median of 3
void quickSort3(int * A, int l, int r, sortperf_t *s) { 
  inccalls(s,1);
  int i = l; 
  int j = r;
  partition3(A,l,r,&i,&j,s);
  if (l<j) quickSort3(A,l,j,s);
  if (r>i) quickSort3(A,i,r,s);
}


// quicksort with insertion for small partitions
void quickSortIns(int * A, int l, int r, sortperf_t *s) { 
  inccalls(s,1);
  int i; 
  int j;
  partition(A,l,r,&i,&j,s);
  if(l<j){
    if (j-l>50) {
    quickSortIns(A,l,j,s);
    }
    else{
      insertionSort(A,l,j,s);
    }
  }
  if(i<r){
    if (r-i>50) {
    quickSortIns(A,i,r,s);
    }
    else{
      insertionSort(A,i,r,s);
    }
  }
}


// quicksort with insertion for small partitions and median of 3
void quickSort3Ins(int * A, int l, int r, sortperf_t *s) { 
  inccalls(s,1);
  int i; 
  int j;
  partition3(A,l,r,&i,&j,s);
  if(l<j){
    if (j-l>50) {
    quickSort3Ins(A,l,j,s);
    }
    else{
      insertionSort(A,l,j,s);
    }
  }
  if(i<r){
    if (r-i>50) {
    quickSort3Ins(A,i,r,s);
    }
    else{
      insertionSort(A,i,r,s);
    }
  }
}
 

void uso()
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
{
  fprintf(stderr,"sortperf\n");
  fprintf(stderr,"\t-z <int>\t(vector size)\n");
  fprintf(stderr,"\t-s <int>\t(initialization seed)\n");
  fprintf(stderr,"\t-a <s|i|q|q3|qi|q3i|h|rs|b|m|c|bu|rx>\t(algorithm)\n");
  fprintf(stderr,"\t    s\tselection\n");
  fprintf(stderr,"\t    i\tinsertion\n");
  fprintf(stderr,"\t    q\tquicksort\n");
  fprintf(stderr,"\t    q3\tquicksort+median3\n");
  fprintf(stderr,"\t    qi\tquicksort+insertion\n");
  fprintf(stderr,"\t    q3i\tquicksort+median3+insertion\n");
  fprintf(stderr,"\t    h\theapsort\n");
  fprintf(stderr,"\t    rs\trecursive selection\n");
  fprintf(stderr,"\t    b\tbolha\n");
  fprintf(stderr,"\t    m\tmerge\n");
  fprintf(stderr,"\t    c\tcounting\n");
  fprintf(stderr,"\t    bu\tbucket\n");
  fprintf(stderr,"\t    rx\tradix\n");
}

void parse_args(int argc, char ** argv, opt_t * opt)
// Descricao: le as opcoes da linha de comando e inicializa variaveis
// Entrada: argc, argv, opt
// Saida: opt
{
     // variaveis externas do getopt
     extern char * optarg;
     extern int optind;

     // variavel auxiliar
     int c;

     // inicializacao variaveis globais para opcoes
     opt->seed = 1;
     opt->size = 10;
     opt->alg = 1;
     opt->type=0;

     // getopt - letra indica a opcao, : junto a letra indica parametro
     // no caso de escolher mais de uma operacao, vale a ultima
     while ((c = getopt(argc, argv, "z:s:a:t:h")) != EOF){
       switch(c) {
         case 'z':
	          opt->size = atoi(optarg);
                  break;
         case 's':
	          opt->seed = atoi(optarg);
                  break;
         case 'a':
		  opt->alg = name2num(optarg);
                  break;
          case 't':
		  opt->type = atoi(optarg);
                  break;
         case 'h':
         default:
                  uso();
                  exit(1);

       }
     }
     if (!opt->alg) {
       uso();
       exit(1);
     }
}
void init_args(int seed, int size, char * alg, int type, opt_t * opt)
// Descricao: inicia as variáveis de acorodo com as entradas
// Entrada: seed, size, alg, argc, argv, opt
// Saida: opt
{
     // inicializacao variaveis globais para opcoes
     opt->seed = seed;
     opt->size = size;
     opt->alg = name2num(alg);
     opt->type= type;

}

void clkDiff(struct timespec t1, struct timespec t2,
                   struct timespec * res)
// Descricao: calcula a diferenca entre t2 e t1, que e armazenada em res
// Entrada: t1, t2
// Saida: res
{
  if (t2.tv_nsec < t1.tv_nsec){
    // ajuste necessario, utilizando um segundo de tv_sec
    res-> tv_nsec = 1000000000+t2.tv_nsec-t1.tv_nsec;
    res-> tv_sec = t2.tv_sec-t1.tv_sec-1;
  } else {
    // nao e necessario ajuste
    res-> tv_nsec = t2.tv_nsec-t1.tv_nsec;
    res-> tv_sec = t2.tv_sec-t1.tv_sec;
  }
}


int main (int argc, char ** argv){
  sortperf_t s;
  int * vet;
  char buf[200];
  char pref[100];
  opt_t opt;
  struct timespec inittp, endtp, restp;
  int retp;

  // parse_args
  parse_args(argc,argv,&opt);

  // malloc with opt.size+1 for heapsort
  vet = (int *) malloc((opt.size+1)*sizeof(int));
  // initialize
  resetcounter(&s);
  srand48(opt.seed);
  initVector(vet, opt.size,opt.type);
  vet[opt.size] = vet[0]; // for heapsort
  //if (opt.size < 100) printVector(vet, opt.size);

  retp = clock_gettime(CLOCK_MONOTONIC, &inittp);
//printVector(vet, opt.size);
//printf("\n");
  // execute algorithm
  switch (opt.alg){
    case ALGINSERTION:
         insertionSort(vet, 0, opt.size-1, &s);
         break;
    case ALGSELECTION:
         selectionSort(vet, 0, opt.size, &s);
         break;
    case ALGQSORT:
         quickSort(vet, 0, opt.size-1, &s);
         break;
    case ALGQSORT3:
         quickSort3(vet, 0, opt.size-1, &s);
         break;
    case ALGQSORTINS:
         quickSortIns(vet, 0, opt.size-1, &s);
         break;
    case ALGQSORT3INS:
         quickSort3Ins(vet, 0, opt.size-1, &s);
         break;
    case ALGSHELLSORT:
         shellSort(vet, opt.size, &s);
         break;
    case ALGRECSEL:
         recursiveSelectionSort(vet, 0, opt.size-1, &s);
         break;
    case ALGBOLHA:
         bolha(vet, opt.size, &s);
         break;
    case ALGMERGESORT:
        mergeSort(vet, 0, opt.size - 1, &s);
        break;
    case ALGCOUNTINGSORT:
        countingsort(vet, opt.size, &s);
        break;
    case ALGBUCKETSORT:
        bucketSort(vet, opt.size, &s);
        break;
    case ALGRADIXSORT:
        radixsort(vet, opt.size, &s);
        break;
  }
  retp = clock_gettime(CLOCK_MONOTONIC, &endtp);
  clkDiff(inittp, endtp, &restp);


  //if (opt.size<100) printVector(vet, opt.size);

  // print stats
//printVector(vet, opt.size);
//printf("\n");
 
  sprintf(pref,"alg %s seed %d size %d time %ld.%.9ld",
          num2name(opt.alg), opt.seed,opt.size,restp.tv_sec,restp.tv_nsec);

  printsortperf(&s,buf,pref);
  printf("%s\n",buf);

  exit(0);
}