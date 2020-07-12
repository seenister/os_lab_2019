#include <arpa/inet.h>
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
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
  int fd;
  int nread;
  int buf_size = -1;
  int port = -1;
  char ip[16] = {'\0'};
  struct sockaddr_in servaddr;

  while (1) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"buf_size", required_argument, 0, 0},
                                      {"ip", required_argument, 0, 0},
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
        strcpy(ip, optarg);
        break;
      case 2:
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

  if (buf_size == -1 || port == -1 || strlen(ip) == 0) {
    fprintf(stderr, "Using: %s --buf_size [NUMBER] --port [NUMBER] --ip [IPADDR]\n",
            argv[0]);
    return -1;
  }


  printf("buf=%d port=%d ip=%s\n", buf_size, port, ip);

  char buf[buf_size];

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }


  printf("Socket %d created\n", fd);


  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  servaddr.sin_port = htons(port); //Функция htons() преобразует узловой порядок расположения байтов положительного короткого целого hostshort в сетевой порядок расположения байтов. 

  if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) { //Файловый дескриптор sockfd должен ссылаться на сокет. Если сокет имеет тип SOCK_STREAM то данный системный вызов попытается установить соединение с другим сокетом. Другой сокет задан параметром serv_addr, являющийся адресом длиной addrelen в пространстве коммуникации сокета. Каждое пространство коммуникации интерпретирует параметр serv_addr по-своему. 
    perror("connect");
    printf("Connect error: %d\n", errno);
    exit(1);
  }

  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, buf_size)) > 0) { //read() пытается записать count байтов файлового описателя fd в буфер, адрес которого начинается с buf. 
     
    if (write(fd, buf, nread) < 0) { //write записывает до count байтов из буфера buf в файл, на который ссылается файловый описатель fd 
      perror("write");
      exit(1);
    }
  }

  close(fd);
  exit(0);
}
