#include "lab2.h"
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

pthread_t tid[11];
pthread_mutex_t lock;
sem_t sem_c, sem_d, sem_e, sem_g, sem_h, sem_i, sem_dc, sem_end;
int err;

// 26
unsigned int lab2_thread_graph_id() { return 10; }
const char *lab2_unsynchronized_threads() { return "bcde"; }
const char *lab2_sequential_threads() { return "dghi"; }

void log_t(const char *t) {
  pthread_mutex_lock(&lock);
  std::cout << t << std::flush;
  pthread_mutex_unlock(&lock);
  computation();
}

int start_t(pthread_t *t, void *(*f)(void *)) {
  return pthread_create(t, NULL, f, NULL);
}

void wait_t(pthread_t t) { pthread_join(t, NULL); }

void *thread_a(void *ptr);
void *thread_b(void *ptr);
void *thread_c(void *ptr);
void *thread_d(void *ptr);
void *thread_e(void *ptr);
void *thread_f(void *ptr);
void *thread_g(void *ptr);
void *thread_h(void *ptr);
void *thread_i(void *ptr);
void *thread_k(void *ptr);
void *thread_m(void *ptr);

void *thread_a(void *ptr) {
  for (int i = 0; i < 3; i++) {
    log_t("a");
  }
  return ptr;
}

void *thread_b(void *ptr) {
  for (int i = 0; i < 3; i++) {
    log_t("b");
  }
  return ptr;
}

void *thread_c(void *ptr) {
  for (int i = 0; i < 3; i++) {
    log_t("c");
  }
  wait_t(tid[0]);
  sem_post(&sem_dc);
  sem_wait(&sem_c);
  for (int i = 0; i < 3; i++) {
    log_t("c");
  }
  return ptr;
}

void *thread_d(void *ptr) {
  for (int i = 0; i < 3; i++) {
    log_t("d");
  }
  sem_wait(&sem_dc);
  sem_post(&sem_e);
  sem_wait(&sem_d);
  for (int i = 0; i < 3; i++) {
    log_t("d");
  }
  wait_t(tid[2]);
  wait_t(tid[1]);
  sem_post(&sem_e);
  sem_wait(&sem_d);
  for (int i = 0; i < 3; i++) {
    log_t("d");
  }

  for (int i = 0; i < 3; i++) {
    sem_wait(&sem_d);
    log_t("d");
    sem_post(&sem_g);
  }

  return ptr;
}

void *thread_e(void *ptr) {
  for (int i = 0; i < 3; i++) {
    log_t("e");
  }
  sem_post(&sem_d);
  sem_wait(&sem_e);
  sem_post(&sem_c);

  int err = start_t(&tid[1], thread_b);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

  for (int i = 0; i < 3; i++) {
    log_t("e");
  }
  sem_post(&sem_d);
  sem_wait(&sem_e);

  err = start_t(&tid[5], thread_f);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
  err = start_t(&tid[6], thread_g);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

  for (int i = 0; i < 3; i++) {
    log_t("e");
  }
  return ptr;
}

void *thread_f(void *ptr) {
  for (int i = 0; i < 3; i++) {
    log_t("f");
  }
  return ptr;
}

void *thread_g(void *ptr) {
  for (int i = 0; i < 3; i++) {
    log_t("g");
  }

  wait_t(tid[5]);
  wait_t(tid[4]);

  int err = start_t(&tid[7], thread_h);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
  err = start_t(&tid[8], thread_i);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

  sem_post(&sem_d);

  for (int i = 0; i < 3; i++) {
    sem_wait(&sem_g);
    log_t("g");
    sem_post(&sem_h);
  }
  return ptr;
}

void *thread_h(void *ptr) {
  for (int i = 0; i < 3; i++) {
    sem_wait(&sem_h);
    log_t("h");
    sem_post(&sem_i);
  }
  return ptr;
}

void *thread_i(void *ptr) {
  for (int i = 0; i < 3; i++) {
    sem_wait(&sem_i);
    log_t("i");
    sem_post(&sem_d);
  }

  int err = start_t(&tid[9], thread_k);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

  for (int i = 0; i < 3; i++) {
    log_t("i");
  }

  wait_t(tid[9]);

  err = start_t(&tid[10], thread_m);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

  return ptr;
}

void *thread_k(void *ptr) {
  for (int i = 0; i < 3; i++) {
    log_t("k");
  }
  return ptr;
}

void *thread_m(void *ptr) {
  for (int i = 0; i < 3; i++) {
    log_t("m");
  }
  sem_post(&sem_end);
  return ptr;
}

int lab2_init() {
  if (pthread_mutex_init(&lock, NULL) != 0) {
    std::cerr << "Mutex init failed" << std::endl;
    return 1;
  }

  if (sem_init(&sem_c, 0, 0) != 0) {
    std::cerr << "Semaphore #1 init failed" << std::endl;
    return 1;
  }
  if (sem_init(&sem_d, 0, 0) != 0) {
    std::cerr << "Semaphore #2 init failed" << std::endl;
    return 1;
  }
  if (sem_init(&sem_e, 0, 0) != 0) {
    std::cerr << "Semaphore #3 init failed" << std::endl;
    return 1;
  }
  if (sem_init(&sem_g, 0, 0) != 0) {
    std::cerr << "Semaphore #4 init failed" << std::endl;
    return 1;
  }
  if (sem_init(&sem_h, 0, 0) != 0) {
    std::cerr << "Semaphore #5 init failed" << std::endl;
    return 1;
  }
  if (sem_init(&sem_end, 0, 0) != 0) {
    std::cerr << "Semaphore #6 init failed" << std::endl;
    return 1;
  }
  if (sem_init(&sem_i, 0, 0) != 0) {
    std::cerr << "Semaphore #7 init failed" << std::endl;
    return 1;
  }
  if (sem_init(&sem_dc, 0, 0) != 0) {
    std::cerr << "Semaphore #8 init failed" << std::endl;
    return 1;
  }

  int err;
  err = start_t(&tid[0], thread_a);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
  err = start_t(&tid[2], thread_c);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
  err = start_t(&tid[3], thread_d);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;
  err = start_t(&tid[4], thread_e);
  if (err != 0)
    std::cerr << "Can't create thread. Error: " << strerror(err) << std::endl;

  sem_wait(&sem_end);

  pthread_mutex_destroy(&lock);
  sem_destroy(&sem_c);
  sem_destroy(&sem_dc);
  sem_destroy(&sem_d);
  sem_destroy(&sem_e);
  sem_destroy(&sem_g);
  sem_destroy(&sem_h);
  sem_destroy(&sem_i);
  sem_destroy(&sem_end);

  std::cout << std::endl;

  return 0;
}
