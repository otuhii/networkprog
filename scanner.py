#simple shitty network scanner
#!/bin/python3

import socket
import sys
import ipaddress
import argparse
import threading



# Global list to store opened ports
opened_ports = []
lock = threading.Lock()  # Lock to protect access to opened_ports


def valid_ip(ip):
    try:
        # Check if it's a valid IP address (IPv4 or IPv6)
        return str(ipaddress.ip_address(ip))
    except ValueError:
        raise argparse.ArgumentTypeError(f"Invalid IP address: {ip}")



def check_port(target,port):
    try:
        #AF_INET - host ip, SOCK_STREAM - port
        print(f"Checking port {port}")
        sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        socket.setdefaulttimeout(1) #time for open port detection
        result = sok.connect_ex((target, port))
        if result==0:
            print(f"Port {port} is open")
            with lock:
                opened_ports.append(port)
        sok.close()
    except socket.error as e:
        print(f"Error checking port {port}: {e}")




try:
    parser = argparse.ArgumentParser()
        
    parser.add_argument('-i', '--ipaddress', type=valid_ip, help="Specify hostname")
    parser.add_argument('-p', '--port', type=int,nargs="+", help="Pass port that you want to scan")
    
    # Parse the arguments
    args = parser.parse_args()

    if not args.ipaddress:
        print("Please specify a host ip")
        print("Syntax ex: python3 scanner.py -i 0.0.0.0 -p [i, n] or -p int")
        sys.exit()
    match len(args.port):
        case 1:
            ports = range(0, args.port[0])
        case 2:
            ports = range(args.port[0], args.port[1]+1)
        case _:
            raise ValueError("Too much values for port argument")


    print('-'*50)
    print(f"Scanning target {args.ipaddress} on ports {ports}")
    print('-'*50) 

    threads = []
        
    for port in ports:
        thread = threading.Thread(target=check_port, args=(args.ipaddress, port))
        threads.append(thread)
        thread.start()
    
    for thread in threads:
        thread.join()


    print(f"Opened ports: {opened_ports}")


except KeyboardInterrupt:
    print("\nExiting")
    sys.exit()

except socket.gaierror:
    print("Hostname could not be resolved(")
    sys.exit()

except socket.error:
    print("Couldn't connect to server")
    sys.exit()

except argparse.ArgumentTypeError as e:
    print(e)
    sys.exit()

except ValueError as e:
    print(e)
    sys.exit()

