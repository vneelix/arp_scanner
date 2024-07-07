## ARP subnet scanner
Simple [ARP](https://en.wikipedia.org/wiki/Address_Resolution_Protocol) proto subnet scanner. Program generate ARP request packet for each subnet [member](https://en.wikipedia.org/wiki/Classless_Inter-Domain_Routing) and waiting ARP reply packets. Allows to detect active network members.

## Build
In the project directory
```bash
mkdir build && cd build
cmake ..
make
```

## Usage
```bash
./arp_scanner --help                                                       ✔ 
Usage: arp_scanner [OPTION...]
ARP subnet scanner. Send ARP probe packet to all addresses in subnet and wait
ARP reply packets

  -b, --batch=32             Addresses batch size
  -i, --sending_interval=4   Pause before sending batch (sec)
  -?, --help                 Give this help list
      --usage                Give a short usage message

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.
```
Can be run without arguments `./arp_scanner`

#### Configured parameters:
- batch size - batch of IPv4 addresses for ARP probe sending
- sending interval - Pause between batch sending

#### Selecting interface
In the interface selection menu, select the network-connected interface. Selecting keys - `UP`, `DOWN` arrows, select - `ENTER`
```bash
Available interfaces:
  lo
* eno1
  wlp4s0
```

#### Output format
In streaming mode recieved ARP responses are printed (`2 another network members discovered`)
```bash
Sending started: subnet size 254, total probe requests 253
ip: 192.168.0.1     mac: 5c:xx:xx:xx:xx:b8
ip: 192.168.0.113   mac: ca:xx:xx:xx:xx:e3
```
When all addresses will be processed, scanner shutdown with `8 sec` delay
