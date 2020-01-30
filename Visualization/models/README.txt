Take the desired .x files and place them in the directory printed out by the following python commands:

import panda3d
print(os.path.join(os.path.dirname(panda3d.__file__), "models"))