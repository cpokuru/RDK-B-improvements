#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/sockios.h>
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
    strncpy(ifr.ifr_name, "loo", IFNAMSIZ);

    // Check if the interface exists
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
        perror("ioctl(SIOCGIFFLAGS)");
        close(sockfd);
        return 1;
    }

    if ((ifr.ifr_flags & IFF_UP) == 0) {
        printf("Interface 'loo' is already down.\n");
        close(sockfd);
        return 0;
    }

    // Bring the interface down
    ifr.ifr_flags &= ~IFF_UP;  // Clear the IFF_UP flag to bring the interface down

    if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) {
        perror("ioctl(SIOCSIFFLAGS)");
        close(sockfd);
        return 1;
    }

    // Set the new interface name to "lo"
    strncpy(ifr.ifr_newname, "lo", IFNAMSIZ);

    // Issue the ioctl request to rename the interface
    if (ioctl(sockfd, SIOCSIFNAME, &ifr) < 0) {
        perror("ioctl(SIOCSIFNAME)");
        close(sockfd);
        return 1;
    }

    // Bring the interface back up
    ifr.ifr_flags |= IFF_UP;  // Set the IFF_UP flag to bring the interface up

    if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) {
        perror("ioctl(SIOCSIFFLAGS)");
        close(sockfd);
        return 1;
    }

    // Close the socket
    close(sockfd);

    printf("Interface 'lo' renamed to 'loo' and brought up successfully.\n");

    return 0;
}

