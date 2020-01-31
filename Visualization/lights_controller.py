from pythonosc import osc_message_builder, udp_client
import argparse
import time
import numpy as np


def set_color(client, fixture, rgb):
    """
    fixture : int, representing the id of the fixture
    rgb : [r, g, b], where r, g, and b are floats
    """
    client.send_message("/sr{}/rgb".format(fixture),rgb)

def init_client(ip, port):
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", default=ip, help="OSC server ip")
    parser.add_argument("--port", type=int, default=port, help="OSC server port")
    args = parser.parse_args()
    client = udp_client.SimpleUDPClient(args.ip, args.port)
    return client

def set_lights_active(client):
    client.send_message("/set_active",1)
    time.sleep(1)

if(__name__ == "__main__"):
    client = init_client("lights.media.mit.edu", 10002)

    #Light values are controlled manually
    client.send_message("/set_inactive",1)
    time.sleep(1)

    #Light values are controlled by osc commands
    #client.send_message("/set_active",1)
    #time.sleep(1)

    
    i = 0
    while(i < 20):
        set_color(client, i + 1, [64/255,224/255,208/255])
        time.sleep(0.1)
        i += 1
    
