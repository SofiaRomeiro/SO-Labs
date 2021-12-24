#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#define N 4

typedef struct {
  int saldo;
  int numMovimentos;
  pthread_mutex_t trinco;
  pthread_rwlock_t rwlock;
  /* outras variáveis,ex. nome do titular, etc. */
} conta_t;

//Variaveis globais
conta_t c;


int depositar_dinheiro(conta_t* conta, int valor) {
  if (valor < 0)
    return -1;

  pthread_rwlock_wrlock(&(conta->rwlock));

  //sleep(1);

  conta->saldo += valor;
  conta->numMovimentos ++;

  pthread_rwlock_unlock(&(conta->rwlock));

  return valor;
}

int levantar_dinheiro(conta_t* conta, int valor) {
  if (valor < 0)
    return -1;

  pthread_mutex_lock(&(conta->trinco));

  //sleep(1);

  if (conta->saldo >= valor) {
    conta->saldo -= valor;
    conta->numMovimentos ++;
  }
  else
    valor = -1;

  pthread_mutex_unlock(&(conta->trinco));

  return valor;
}

void consultar_conta(conta_t* conta) {
  int s, n;

  pthread_rwlock_rdlock(&(conta->rwlock));

  //sleep(1);

  s = conta->saldo;
  n = conta->numMovimentos;

  pthread_rwlock_unlock(&(conta->rwlock));
  
  printf("Consulta: saldo=%d, #movimentos=%d\n", s, n);
}


void *fnAlice(void *arg) {
  int m = *((int*)arg);
  int total = 0;
  int r;
  
  for (int i = 0; i<m; i++) {
    r = depositar_dinheiro(&c, 1);
    if (r != -1)
      total += r;
  }

  printf("Alice depositou no total: %d\n", total);
  consultar_conta(&c);
  return NULL;
}


void *fnBob(void *arg) {
  int m = *((int*)arg);
  int total = 0;
  int r;

  for (int i = 0; i<m; i++) {
    r = levantar_dinheiro(&c, 1);
    if (r != -1)
      total += r;
  }

  printf("Bob gastou no total: %d\n", total);
  consultar_conta(&c);
  return NULL;
}


int main(int argc, char** argv) {

  pthread_t tid[2];

  pthread_mutex_init(&(c.trinco), NULL);
  pthread_rwlock_init(&(c.rwlock), NULL);
  
  int m;

  if (argc > 1)
    m = atoi(argv[1]);    // atoi converts string into 'int' value
  else
    m = 0;
  
  c.saldo = 0;
  c.numMovimentos = 0;
  
  if (pthread_create (&tid[0], NULL, fnAlice, (void*)&m) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[1], NULL, fnBob, (void*)&m) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[2], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[3], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[4], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[5], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[6], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[7], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[8], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[9], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE); 
  if (pthread_create (&tid[10], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[11], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[12], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[13], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[14], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create (&tid[15], NULL, (void *)consultar_conta, (void *)&c) != 0)
    exit(EXIT_FAILURE);
  
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
  pthread_join(tid[3], NULL);
  pthread_join(tid[4], NULL);
  pthread_join(tid[5], NULL);
  pthread_join(tid[6], NULL);
  pthread_join(tid[7], NULL);
  pthread_join(tid[8], NULL);
  pthread_join(tid[9], NULL);
  pthread_join(tid[10], NULL);
  pthread_join(tid[11], NULL);
  pthread_join(tid[12], NULL);
  pthread_join(tid[13], NULL);
  pthread_join(tid[14], NULL);
  pthread_join(tid[15], NULL);

  printf("História chegou ao fim\n");
  consultar_conta(&c);

  pthread_mutex_destroy(&(c.trinco));
  pthread_rwlock_destroy(&(c.rwlock));

  exit(EXIT_SUCCESS);
}

