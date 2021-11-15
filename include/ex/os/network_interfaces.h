#pragma once

#include <algorithm>
#include <array>
#include <iterator>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if_dl.h>
#endif
#if __linux__
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>
#endif
#ifdef _WIN32
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <codecvt>
#include <iphlpapi.h>
#include <locale>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#endif

namespace ex {
namespace os {
enum ip_family { all = 0, ipv4 = AF_INET, ipv6 = AF_INET6 };

enum network_interface_filter { has_ip, has_ipv4, has_ipv6, has_ipv4_and_ipv6 };

struct ip_t {
  ip_family family;
  std::string addr;
  uint8_t cidr;
};

struct network_interface_t {
  std::string name;
  std::array<uint8_t, 6> mac;
  std::vector<ip_t> ips;
};

inline std::vector<network_interface_t>
filter_network_interfaces(const std::vector<network_interface_t> &nis,
                          network_interface_filter nif, ip_family iff) {
  std::vector<os::network_interface_t> filtered;
  std::copy_if(
      nis.begin(), nis.end(), std::back_inserter(filtered),
      [&nif, &iff](os::network_interface_t x) {
        if (nif == os::network_interface_filter::has_ip)
          return x.ips.size() > 0;
        if (nif == os::network_interface_filter::has_ipv4)
          return std::find_if(x.ips.begin(), x.ips.end(), [](os::ip_t ip) {
                   return ip.family == os::ip_family::ipv4;
                 }) != x.ips.end();
        if (nif == os::network_interface_filter::has_ipv6)
          return std::find_if(x.ips.begin(), x.ips.end(), [](os::ip_t ip) {
                   return ip.family == os::ip_family::ipv6;
                 }) != x.ips.end();
        if (nif == os::network_interface_filter::has_ipv4_and_ipv6)
          return std::find_if(x.ips.begin(), x.ips.end(),
                              [](os::ip_t ip) {
                                return ip.family == os::ip_family::ipv4;
                              }) != x.ips.end() &&
                 std::find_if(x.ips.begin(), x.ips.end(), [](os::ip_t ip) {
                   return ip.family == os::ip_family::ipv6;
                 }) != x.ips.end();
        return false;
      });

  if (iff != os::ip_family::all)
    std::transform(
        filtered.begin(), filtered.end(), filtered.begin(),
        [&iff](os::network_interface_t x) -> os::network_interface_t {
          std::vector<os::ip_t> ips;
          std::copy_if(x.ips.begin(), x.ips.end(), std::back_inserter(ips),
                       [&iff](os::ip_t ip) { return ip.family == iff; });
          x.ips = std::move(ips);
          return x;
        });
  return filtered;
}

inline std::vector<network_interface_t> network_interfaces() {
  std::vector<network_interface_t> nis;

#ifdef _WIN32
  PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
  IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = nullptr;
  ULONG outBufLen = 0;
  DWORD dwRetVal = 0;
  char buff[100];
  DWORD bufflen = 100;
  int i;

  GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen);

  pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);

  if ((dwRetVal = GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses,
                                       &outBufLen)) == NO_ERROR) {

    while (pAddresses) {
      std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
      auto name = converter.to_bytes(std::wstring(pAddresses->FriendlyName));
      auto ni =
          std::find_if(nis.begin(), nis.end(), [&name](network_interface_t x) {
            return x.name == name;
          });
      if (ni == nis.end()) {
        std::array<uint8_t, 6> mac = {
            pAddresses->PhysicalAddress[0], pAddresses->PhysicalAddress[1],
            pAddresses->PhysicalAddress[2], pAddresses->PhysicalAddress[3],
            pAddresses->PhysicalAddress[4], pAddresses->PhysicalAddress[5]};
        network_interface_t n{name, mac, {}};
        nis.push_back(n);
        ni = nis.end() - 1;
      }

      PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pAddresses->FirstUnicastAddress;

      for (i = 0; pUnicast != NULL; i++) {
        if (pUnicast->Address.lpSockaddr->sa_family == ip_family::ipv4) {
          sockaddr_in *sa_in = (sockaddr_in *)pUnicast->Address.lpSockaddr;
          auto addr =
              inet_ntop(ip_family::ipv4, &(sa_in->sin_addr), buff, bufflen);
          ni->ips.push_back(
              {ip_family::ipv4, addr, pUnicast->OnLinkPrefixLength});
        } else if (pUnicast->Address.lpSockaddr->sa_family == ip_family::ipv6) {
          sockaddr_in6 *sa_in6 = (sockaddr_in6 *)pUnicast->Address.lpSockaddr;
          auto addr =
              inet_ntop(ip_family::ipv6, &(sa_in6->sin6_addr), buff, bufflen);
          ni->ips.push_back(
              {ip_family::ipv6, addr, pUnicast->OnLinkPrefixLength});
        }
        pUnicast = pUnicast->Next;
      }
      pAddresses = pAddresses->Next;
    }
  }

  free(pAddresses);
#else
  struct ifaddrs *ifa = nullptr;
  auto rc = getifaddrs(&ifa);

  if (!rc) {
    for (auto ifEntry = ifa; ifEntry != nullptr; ifEntry = ifEntry->ifa_next) {
      auto ni = std::find_if(nis.begin(), nis.end(),
                             [&ifEntry](network_interface_t x) {
                               return x.name == ifEntry->ifa_name;
                             });
      if (ni == nis.end()) {
        network_interface_t n{ifEntry->ifa_name, {0}, {}};
        nis.push_back(n);
        ni = nis.end() - 1;
      }

      if (ifEntry->ifa_addr == nullptr)
        continue;

      if (ifEntry->ifa_addr->sa_family == ip_family::ipv4) {
        auto ptr = &((struct sockaddr_in *)ifEntry->ifa_addr)->sin_addr;
        char buffer[INET_ADDRSTRLEN];
        const char *addr = inet_ntop(ifEntry->ifa_addr->sa_family, ptr, buffer,
                                     INET_ADDRSTRLEN);
        auto n =
            ((struct sockaddr_in *)(ifEntry->ifa_netmask))->sin_addr.s_addr;
        uint8_t cidr = 0;
        while (n > 0) {
          if (n & 1)
            cidr++;
          n = n >> 1;
        }
        if (addr)
          ni->ips.push_back({ip_family::ipv4, addr, cidr});
      } else if (ifEntry->ifa_addr->sa_family == ip_family::ipv6) {
        auto ptr = &((struct sockaddr_in6 *)ifEntry->ifa_addr)->sin6_addr;
        char buffer[INET6_ADDRSTRLEN];
        const char *addr = inet_ntop(ifEntry->ifa_addr->sa_family, ptr, buffer,
                                     INET6_ADDRSTRLEN);
        auto c =
            ((struct sockaddr_in6 *)(ifEntry->ifa_netmask))->sin6_addr.s6_addr;
        int i = 0;
        uint8_t cidr = 0;
        unsigned char n = 0;
        while (i < 16) {
          n = c[i];
          while (n > 0) {
            if (n & 1)
              cidr++;
            n = n / 2;
          }
          i++;
        }
        if (addr)
          ni->ips.push_back({ip_family::ipv6, addr, cidr});
      }
#ifdef __APPLE__
      else if (ifEntry->ifa_addr->sa_family == AF_LINK) {
        auto ptr =
            (unsigned char *)LLADDR((struct sockaddr_dl *)ifEntry->ifa_addr);
        for (int i = 0; i < 6; ++i)
          ni->mac[i] = ptr[i];
      }
#endif
#if __linux__
      else if (ifEntry->ifa_addr->sa_family == AF_PACKET) {
        auto s = (struct sockaddr_ll *)(ifEntry->ifa_addr);
        for (int i = 0; i < 6; i++)
          ni->mac[i] = s->sll_addr[i];
      }
#endif
    }
  }
  freeifaddrs(ifa);
#endif
  return nis;
}
} // namespace os
} // namespace ex