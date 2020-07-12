#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#define SADDR struct sockaddr

int main(int argc, char *argv[]) {
 
  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  int buf_size = -1;
  int port = -1;

  while (1) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"buf_size", required_argument, 0, 0},
                                      {"port", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        if ((buf_size = atoi(optarg)) == 0) {
          printf("Error: bad buf_size value\n");
          return -1;
        }
        break;
      case 1:
        if ((port = atoi(optarg)) == 0) {
          printf("Error: bad port value\n");
          return -1;
        }
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (buf_size == -1 || port == -1) {
    fprintf(stderr, "Using: %s --buf_size [NUMBER] --port [NUMBER]\n",
            argv[0]);
    return -1;
  }

  char buf[buf_size];

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  printf("Socket %d created\n", lfd);

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //Функция htonl() преобразует узловой порядок расположения байтов положительного целого hostlong в сетевой порядок расположения байтов.
  servaddr.sin_port = htons(port); //Функция htons() преобразует узловой порядок расположения байтов положительного короткого целого hostshort в сетевой порядок расположения байтов. 

  int opt_val = 1;
  setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)); 
//Переменная sockfd должна ссылаться на открытый дескриптор сокета. Переменная level определяет, каким кодом должен интерпретироваться параметр: общими программами обработки сокетов или зависящими от протокола программами (например, IPv4, IPv6, TCP или SCTP). Optval — это указатель на переменную, из которой извлекается новое значение параметра с помощью функции setsockopt или в которой сохраняется текущее значение параметра с помощью функции getsockopt. 

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }


  printf("Socket %d bound to %d:%d\n", lfd, servaddr.sin_addr.s_addr, ntohs(servaddr.sin_port));

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }


  printf("Socket %d is listening\n", lfd);


  while (1) {
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      printf("Accept error: %d\n", errno);
      exit(1);
    }


    printf("Connection established, client: %d [%d:%d]\n", cfd, ntohl(cliaddr.sin_addr.s_addr), ntohs(cliaddr.sin_port));

   
    while ((nread = read(cfd, buf, buf_size)) > 0) {
      write(1, &buf, nread);
    }

    if (nread == -1) {
      perror("read");
      exit(1);
    }
    close(cfd);
  }
  close(lfd);
}

