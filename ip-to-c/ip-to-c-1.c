#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define DOCSIS_INF_NAME "lo"

void print_interface_ip_addresses(const char *interface_name) {
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in *addr;

    // Open a socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set interface name
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ);

    // Get IP addresses associated with the interface
    if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0) {
        perror("ioctl SIOCGIFADDR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    addr = (struct sockaddr_in *)&ifr.ifr_addr;
    printf("IP address of %s: %s\n", interface_name, inet_ntoa(addr->sin_addr));

    // Close the socket
    close(sockfd);
}
void flush_interface_ip_addresses(const char *interface_name) {
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in addr;

    // Open a socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set interface name
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ);

    // Set IP address to 0
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    memcpy(&ifr.ifr_addr, &addr, sizeof(addr));

    // Flush IP addresses associated with the interface
    if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0) {
        perror("ioctl SIOCSIFADDR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("IP addresses on %s flushed.\n", interface_name);

    // Close the socket
    close(sockfd);
}


int main() {
    // Print IP addresses of eth0
    print_interface_ip_addresses(DOCSIS_INF_NAME);

    printf("Before flushing:\n");
    // Add your code to flush IP addresses here
    // Flush IP addresses of eth0
    flush_interface_ip_addresses(DOCSIS_INF_NAME);

    // Print IP addresses of eth0 after flushing
    printf("After flushing:\n");
    //print_interface_ip_addresses(DOCSIS_INF_NAME);
    return 0;
}

