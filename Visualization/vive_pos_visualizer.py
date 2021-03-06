from direct.directbase import DirectStart
from panda3d.ode import OdeWorld, OdeBody, OdeMass
from panda3d.core import Quat, LineSegs, Mat4
import numpy as np
import serial
import struct
import threading
import colorsys
from lights_controller import *


fixture_positions =np.array([[1.5, -3.0],
                             [1.5, -2.8],
                             [1.5, -2.5],
                             [1.4, -2.1],
                             [1.37, -0.8],
                             [1.35, -00.4],
                             
                             [-0.5, -2.3],
                             [-0.5, -2.1],
                             [-0.5, -1.4],
                             [-0.5, -1.2],
                             [-0.5, -0.5],
                             [-0.5, -0.25],

                             [0.68, -1.9],
                             [1, -2.25],
                             [1.23, -2.44],
                             [1.35, -2.23],

                             [-0.07, -1.27],
                             [0.027, -1.15],
                             [0.21, -1.14],
                             [0.48, -1.3]])


client = init_client("lights.media.mit.edu", 10002)
set_lights_active(client)

def update_fixtures(cur_pos):
    dp = fixture_positions - cur_pos
    distances = np.linalg.norm(dp, axis=1)
    intensity = (0.5 - np.clip(distances, 0.0, 0.5)) / 0.5
    
    for i in range(fixture_positions.shape[0]):
        set_color(client, i + 1, colorsys.hsv_to_rgb(0.5, 1.0, intensity[i]))    


#Draw axes
axes_segs = LineSegs("axes");
#x axis
axes_segs.setColor(217/255, 67/255, 56/255, 1)
axes_segs.moveTo(-1000, 0, 0)
axes_segs.drawTo(1000, 0, 0)

#y axis
axes_segs.setColor(56/255, 67/255, 217/255, 1)
axes_segs.moveTo(0, -1000, 0)
axes_segs.drawTo(0, 1000, 0)

#z axis
axes_segs.setColor(56/255, 217/255, 56/255, 1)
axes_segs.moveTo(0, 0, -1000)
axes_segs.drawTo(0, 0, 1000)
segsnode = axes_segs.create(False);
render.attachNewNode(segsnode);


# Load the smiley model which will act as our iron ball
sphere = loader.loadModel("smiley.egg")
sphere.reparentTo(render)
sphere.setPos(0, 0, 0)
sphere.setColor(0.7, 0.4, 0.4)
sphere.setColor(1, 1, 1, 1)

base_station_0 = loader.loadModel("cylinder.x")
base_station_0.reparentTo(render)
base_station_0.setHpr(0,90,0)
base_station_0.setColor(0, 0, 0, 1)

base_station_1 = loader.loadModel("cylinder.x")
base_station_1.reparentTo(render)
base_station_1.setHpr(0,90,0)
base_station_1.setColor(0, 0, 0, 1)

mat = Mat4(camera.getMat())
mat.invertInPlace()
base.mouseInterfaceNode.setMat(mat)
base.enableMouse()

#base.camera.setPos(0, 0, 40)
#base.camera.lookAt(0, 0, 0)
base.trackball.node().setPos(0, 20, 0)

# Create an accumulator to track the time since the sim
# has been running
deltaTimeAccumulator = 0.0
# This stepSize makes the simulation run at 90 frames per second
stepSize = 1.0 / 90.0

x, y, z = (0, 0, 0)

ser = serial.Serial(
    #port='COM5',\
    port='COM8',
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=None)

print("connected to: " + ser.portstr)

#Wait for start message before reading print statements

while True:
    line = ser.readline()
    if(line[-6:].decode() == "start\n"):
        print("started")
        break;

bs_0_xyz = ser.read(12)
bs_0_x, bs_0_y, bs_0_z = struct.unpack(3 * 'f', bs_0_xyz)
base_station_0.setPos(100 * bs_0_x, 100 * -bs_0_z, 100 * bs_0_y/2)
base_station_0.setScale(1, 100 * bs_0_y/2, 1)

bs_1_xyz = ser.read(12)
bs_1_x, bs_1_y, bs_1_z = struct.unpack(3 * 'f', bs_1_xyz)
base_station_1.setPos(100 * bs_1_x, 100 * -bs_1_z, 100 * bs_1_y/2)
base_station_1.setScale(1, 100 * bs_1_y/2, 1)
print("Base station coords")
print(bs_0_x, bs_0_y, bs_0_z)
print(bs_1_x, bs_1_y, bs_1_z)



def serial_reader(ser):
    global x, y, z
    while True:
        line = ser.read(12)
        #32 bit float
        #y and z are flipped
        x_s, y_s, z_s = struct.unpack(3 * 'f', line)
        update_fixtures(np.array([x_s, z_s]))
        x, y, z = 100 * x_s, 100 * -z_s, 100 * y_s
        print(x_s, y_s, z_s)
    ser.close()
        
serial_thread = threading.Thread(target=serial_reader, args=(ser,))
serial_thread.start()


# The task for our simulation
def simulationTask(task):
    global deltaTimeAccumulator
    # Add the deltaTime for the task to the accumulator
    deltaTimeAccumulator += globalClock.getDt()
    while deltaTimeAccumulator > stepSize:
        # Remove a stepSize from the accumulator until
        # the accumulated time is less than the stepsize
        deltaTimeAccumulator -= stepSize
    sphere.setPos(x, y, z)
    return task.cont

taskMgr.doMethodLater(1.0, simulationTask, "VIVE Position Visualizer")

base.run()

