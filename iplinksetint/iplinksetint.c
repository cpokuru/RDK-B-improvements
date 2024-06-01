#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <errno.h>

int main() {
    int sockfd;
    struct ifreq ifr;

    // Create a socket for ioctl operations
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // Set the interface name to "loo"
    const char* interface_name = "loo";
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ);

    // Retrieve the current interface flags
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
        perror("ioctl(SIOCGIFFLAGS)");
        close(sockfd);
        return 1;
    }

    // Set the IFF_UP flag to bring the interface up
    ifr.ifr_flags |= IFF_UP;
    if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) {
        perror("ioctl(SIOCSIFFLAGS)");
        close(sockfd);
        return 1;
    }

    // Close the socket
    close(sockfd);

    printf("Interface '%s' brought up successfully.\n", interface_name);

    return 0;
}

