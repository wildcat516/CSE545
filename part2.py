from scapy.all import *
import sys



while True:
	pkts = sniff(filter='(ip or tcp or udp) and (dst 192.168.1.10 or dst 192.168.1.20)', iface='eth0', count=1,  prn = lambda x:x.summary())

	for pkt in pkts:        #packet sent to victim or reflector
		if pkt[IP].dst == '192.168.1.10':	#packet sent to victim
			pkt[Ether].dst = pkt[Ether].src
			#print pkt[Ether].dst
			pkt[Ether].src = "38:45:E3:89:B5:56"    #reflector's MAC
			#print packet[Ether].src
			#print packet[Ether].dst
			pkt[IP].dst = pkt[IP].src
			pkt[IP].src = "192.168.1.20"    #reflector's IP
			print pkt[IP].src
			#print packet[IP].dst
		elif pkt[IP].dst == '192.168.1.20':	#packet sent to reflector
			pkt[Ether].dst = pkt[Ether].src
			#print pkt[Ether].dst
			pkt[Ether].src = "31:16:A9:63:FF:83"    #victim's MAC
			#print packet[Ether].src
			#print packet[Ether].dst
			pkt[IP].dst = pkt[IP].src
			pkt[IP].src = "192.168.1.10"    #victim's IP
			print pkt[IP].src
			#print packet[IP].dst
		sendp(pkt)




