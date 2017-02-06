from scapy.all import *
import sys

if len(sys.argv) != 11:
	print "Usage: ./reflector --interface eth0 --victim-ip <your_victim_ip>  --victim-ethernet <your_victim_ethernet> --reflector-ip <your_reflector_ip> --reflector-ethernet <your_reflector_ethernet>"
	exit(1)

for i in range(1, 11):
	#print sys.argv[i]
	if sys.argv[i] == '--interface':
		interface = sys.argv[i+1]
		print interface
	elif sys.argv[i] == '--victim-ip':
		victim_ip = sys.argv[i+1]
		print victim_ip
	elif sys.argv[i] == '--victim-ethernet':
		victim_ethernet = sys.argv[i+1]
		print victim_ethernet
	elif sys.argv[i] == '--reflector-ip':
		reflector_ip = sys.argv[i+1]
		print reflector_ip
	elif sys.argv[i] == '--reflector-ethernet':
		reflector_ethernet = sys.argv[i+1]
		print reflector_ethernet


while True:
	pkts = sniff(filter='(ip or tcp or udp) and (dst 192.168.1.10 or dst 192.168.1.20)', iface=interface, count=1,  prn = lambda x:x.summary())

	for pkt in pkts:        #packet sent to victim or reflector
		if pkt[IP].dst == victim_ip:	#packet sent to victim
			pkt[Ether].dst = pkt[Ether].src
			#print pkt[Ether].dst
			pkt[Ether].src = reflector_ethernet    #reflector's MAC
			#print packet[Ether].src
			#print packet[Ether].dst
			pkt[IP].dst = pkt[IP].src
			pkt[IP].src = reflector_ip    #reflector's IP
			print pkt[IP].src
			#print packet[IP].dst
		elif pkt[IP].dst == reflector_ip:	#packet sent to reflector
			pkt[Ether].dst = pkt[Ether].src
			#print pkt[Ether].dst
			pkt[Ether].src = victim_ethernet    #victim's MAC
			#print packet[Ether].src
			#print packet[Ether].dst
			pkt[IP].dst = pkt[IP].src
			pkt[IP].src = victim_ip    #victim's IP
			print pkt[IP].src
			#print packet[IP].dst
		sendp(pkt)





