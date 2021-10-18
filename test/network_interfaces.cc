#include <cstdio>
#include <os/network_interfaces.h>
#include <iostream>

int main(int argc, char **argv)
{
  auto nis = os::network_interfaces();
  auto hasIp = os::filter_network_interfaces(nis, os::network_interface_filter::has_ip, os::ip_family::all);
  for (auto it = hasIp.begin(); it != hasIp.end(); ++it)
  {
    std::printf("%s %02x:%02x:%02x:%02x:%02x:%02x\n", it->name.c_str(), it->mac[0], it->mac[1], it->mac[2], it->mac[3], it->mac[4], it->mac[5]);
    for (auto itr = it->ips.begin(); itr != it->ips.end(); ++itr)
    {
      std::string family = "unknown";
      switch (itr->family)
      {
      case os::ip_family::ipv4:
        family = "ipv4";
        break;
      case os::ip_family::ipv6:
        family = "ipv6";
        break;
      default:
        break;
      }
      std::cout << "\t" << family << " " << itr->addr << " " << (int)itr->cidr << std::endl;
    }
  }

  return 0;
}