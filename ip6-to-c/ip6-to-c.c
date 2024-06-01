#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/sockios.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ifaddrs.h>

// Define the in6_ifreq structure
struct in6_ifreq {
    struct in6_addr ifr6_addr;
    uint32_t ifr6_prefixlen;
    unsigned int ifr6_ifindex;
};


void flush_ipv6_addresses(const char *ifname) {
    int sockfd;
    struct ifaddrs *ifaddr, *ifa;
    struct in6_ifreq ifr6;

    // Create a socket for ioctl operations
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    // Get the list of network interfaces
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        close(sockfd);
        return;
    }

    // Iterate through the list of network interfaces
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET6)
            continue;

        // Check if the interface name matches
        if (strcmp(ifa->ifa_name, ifname) == 0) {
            // Initialize the in6_ifreq structure
            memset(&ifr6, 0, sizeof(ifr6));
            ifr6.ifr6_prefixlen = 128;  // Prefix length for a single address
            memcpy(&ifr6.ifr6_addr, &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr, sizeof(struct in6_addr));

            // Set the interface index
            ifr6.ifr6_ifindex = if_nametoindex(ifname);
            if (ifr6.ifr6_ifindex == 0) {
                perror("if_nametoindex");
                continue;
            }

            // Perform the ioctl to delete the IPv6 address
            if (ioctl(sockfd, SIOCDIFADDR, &ifr6) < 0) {
                perror("ioctl SIOCDIFADDR");
            } else {
                char addr_str[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &ifr6.ifr6_addr, addr_str, sizeof(addr_str));
                printf("IPv6 address %s flushed from interface %s\n", addr_str, ifname);
            }
        }
    }

    // Free the memory allocated by getifaddrs
    freeifaddrs(ifaddr);

    // Close the socket
    close(sockfd);
}

int main() {
    const char *interface_name = "lo";  // Replace with your interface name

    flush_ipv6_addresses(interface_name);

    return 0;
}

