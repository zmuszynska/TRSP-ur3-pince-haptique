
from urx_modif.urrobot import RobotException, URRobot  # noqa

__version__ = "0.9.0"

try:
    from urx_modif.robot import Robot
except ImportError as ex:
    print("Exception while importing math3d base robot, disabling use of matrices", ex)
    Robot = URRobot
