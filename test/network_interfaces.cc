#include <cstdio>
#include <ex/os.h>
#include <iostream>

int main(int argc, char **argv)
{
  auto nis = ex::os::network_interfaces();
  auto hasIp = ex::os::filter_network_interfaces(nis, ex::os::network_interface_filter::has_ip, ex::os::ip_family::all);
  for (auto it = hasIp.begin(); it != hasIp.end(); ++it)
  {
    std::printf("%s %02x:%02x:%02x:%02x:%02x:%02x\n", it->name.c_str(), it->mac[0], it->mac[1], it->mac[2], it->mac[3], it->mac[4], it->mac[5]);
    for (auto itr = it->ips.begin(); itr != it->ips.end(); ++itr)
    {
      std::string family = "unknown";
      switch (itr->family)
      {
      case ex::os::ip_family::ipv4:
        family = "ipv4";
        break;
      case ex::os::ip_family::ipv6:
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