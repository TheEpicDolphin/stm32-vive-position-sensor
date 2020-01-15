from direct.directbase import DirectStart
from panda3d.ode import OdeWorld, OdeBody, OdeMass
from panda3d.core import Quat
import numpy as np
import serial
import struct
import threading

# Load the smiley model which will act as our iron ball
sphere = loader.loadModel("smiley.egg")
sphere.reparentTo(render)
sphere.setPos(0, 0, 0)
sphere.setColor(0.7, 0.4, 0.4)

# Setup our physics world and the body
world = OdeWorld()
#world.setGravity(0, 0, -9.81)
world.setGravity(0, 0, 0)
body = OdeBody(world)
M = OdeMass()
M.setSphere(7874, 1.0)
body.setMass(M)
body.setPosition(sphere.getPos(render))
body.setQuaternion(sphere.getQuat(render))

# Set the camera position
base.disableMouse()
#base.camera.setPos(80, -20, 40)
base.camera.setPos(0, 0, 40)
base.camera.lookAt(0, 0, 0)

# Create an accumulator to track the time since the sim
# has been running
deltaTimeAccumulator = 0.0
# This stepSize makes the simulation run at 90 frames per second
stepSize = 1.0 / 90.0

ser = serial.Serial(
    port='COM5',\
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
        break;

x, y, z = (0, 0, 0)
def serial_reader(ser):
    global x, y, z
    while True:
        line = ser.read(12)
        #32 bit float
        #y and z are flipped
        x_s, y_s, z_s = struct.unpack(3 * 'f', line)
        x, y, z = x_s, -z_s, y_s
        print(x_s, y_s, z_s)
    ser.close()
        
serial_thread = threading.Thread(target=serial_reader, args=(ser,))
serial_thread.start()


# The task for our simulation
def simulationTask(task):
    global deltaTimeAccumulator
    # Set the force on the body to push it off the ridge
    #body.setForce(0, min(task.time**4 * 500000 - 500000, 0), 0)
    # Add the deltaTime for the task to the accumulator
    deltaTimeAccumulator += globalClock.getDt()
    while deltaTimeAccumulator > stepSize:
        # Remove a stepSize from the accumulator until
        # the accumulated time is less than the stepsize
        deltaTimeAccumulator -= stepSize
        # Step the simulation
        #world.quickStep(stepSize)
        body.setPosition(20 * x, 20 * y, 20 * z)
    # set the new positions
    sphere.setPosQuat(render, body.getPosition(), Quat(body.getQuaternion()))
    return task.cont

taskMgr.doMethodLater(1.0, simulationTask, "VIVE Position Visualizer")

base.run()

