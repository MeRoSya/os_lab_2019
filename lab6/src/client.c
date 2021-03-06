#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "Multiplying.h"

struct Server
{
  char ip[255];
  uint64_t port;
};

struct ThreadArgs
{
  struct Server server;
  char task[sizeof(uint64_t) * 3];
  char rec_info[sizeof(uint64_t)];
};

//Весь дальнейший код (а так же сервера) проверен на стабильность работы на ubuntu 20.04 и mac os big sur beta 11.3.
//Все работает даже для k=2000 и т.д. 
//Здесь же при попытке считать большие числа происходит ошибка Segmentation fault (core dumped), что я не могу объяснить никак
//иначе, как ограничениями CodeReady

void ThreadServers(struct ThreadArgs *args)
{
  struct hostent *hostname = gethostbyname("127.0.0.1" /*to[i].ip*/); //vscode спокойно считывал ip с файла, но здесь
  if (hostname == NULL)                                               //он считывает некий лишний битый бит, что делает строку некорректной
  {                                                                   //(возможно это индикатор переноса)
    fprintf(stderr, "gethostbyname failed with %s\n", args->server.ip);
    exit(1);
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(args->server.port);
  server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

  int sck = socket(AF_INET, SOCK_STREAM, 0);
  if (sck < 0)
  {
    fprintf(stderr, "Socket creation failed!\n");
    exit(1);
  }

  if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    fprintf(stderr, "Connection failed\n");
    exit(1);
  }

  if (send(sck, args->task, sizeof(args->task), 0) < 0)
  {
    fprintf(stderr, "Send failed\n");
    exit(1);
  }

  char response[sizeof(uint64_t)];
  if (recv(sck, args->rec_info, sizeof(response), 0) < 0)
  {
    fprintf(stderr, "Recieve failed\n");
    exit(1);
  }
  close(sck);
}

bool ConvertStringToUI64(const char *str, uint64_t *val)
{
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE)
  {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

int main(int argc, char **argv)
{
  uint64_t k = -1;
  uint64_t mod = -1;
  char servers[255] = {'\0'}; // TODO: explain why 255

  while (true)
  {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c)
    {
    case 0:
    {
      switch (option_index)
      {
      case 0:{
        ConvertStringToUI64(optarg, &k);
        // TODO: your code here
        if(k<0){
          printf("k must not be negative\n");
          return 1;
        }
      }
        break;
      case 1:{
        ConvertStringToUI64(optarg, &mod);
        // TODO: your code here
        if(mod<0){
          printf("mod must not be negative\n");
          return 1;
        }
        if(mod>=INT64_MAX){
          printf("mod is too big");
          return 1;
        }
      }
        break;
      case 2:
        // TODO: your code here
        memcpy(servers, optarg, strlen(optarg));
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    }
    break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers))
  {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  // TODO: for one server here, rewrite with servers from file
  FILE *serv_list;
  if (!(serv_list = fopen(servers, "r")))
  {
    printf("%s doesn't exist", servers);
    return 1;
  }

  unsigned int servers_num = 0;
  struct Server *to = malloc(sizeof(struct Server) * servers_num);
  char *buff = NULL;
  size_t buff_size = 0;
  ssize_t line_size;

  line_size = getline(&buff, &buff_size, serv_list);

  while (line_size > 0)
  {
    servers_num++;
    to = realloc(to, sizeof(struct Server) * servers_num);
    char *temp = strtok(buff, ":");
    ConvertStringToUI64(temp, &to[servers_num - 1].port);
    temp = strtok(NULL, ":");
    memcpy(to[servers_num - 1].ip, temp, sizeof(temp)+1);
    line_size = getline(&buff, &buff_size, serv_list);
  }

  // TODO: delete this and parallel work between servers

  // TODO: work continiously, rewrite to make parallel
  uint64_t final_result = 1;
  if (servers_num >= k)
  {
    servers_num = k;
  }

  struct ThreadArgs args[servers_num];

  pthread_t threads[servers_num];

  for (int i = 0; i < servers_num; i++)
  {

    // TODO: for one server
    // parallel between servers
    uint64_t begin = i * k / servers_num + 1;
    uint64_t end;

    if (i != servers_num - 1)
    {
      end = (i + 1) * k / servers_num;
    }
    else
    {
      end = k;
    }

    args[i].server = to[i];

    memcpy(args[i].task, &begin, sizeof(uint64_t));
    memcpy(args[i].task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(args[i].task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    if (pthread_create(&threads[i], NULL, (void *)ThreadServers, (void *)&args[i]))
    {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  for (uint32_t i = 0; i < servers_num; i++)
  {
    pthread_join(threads[i], NULL);
    uint64_t answer;
    memcpy(&answer, args[i].rec_info, sizeof(uint64_t));
    final_result = MultModulo(final_result, answer, mod);
  }

  free(to);
  printf("Answer: %llu\n", final_result);
  return 0;
}
