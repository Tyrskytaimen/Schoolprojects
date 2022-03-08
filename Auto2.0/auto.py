"""
COMP.CS.100 Programming 1
Student ID: H299784                     050250033
Name:       Katariina Tenhunen          Miika Isberg
Email:      katariina.tenhunen@tuni.fi  miika.isberg@tuni.fi

Class Car: Implements a car that moves to given coordinates and
whose gas tank can be filled.
"""
from math import sqrt


class Car:
    def __init__(self, name, tank_size, gas_consumption):
        """
        :param name: str, the name of the car.
        :param tank_size: float, the size of this car's tank.
        :param gas_consumption: float, how much gas this car consumes
                   when it drives a 100 kilometers
        """

        self.__tank_volume = tank_size
        self.__consumption = gas_consumption
        self.__gas = 0
        self.__odometer = 0
        self.__name = name

        self.__x = 0.0  # Current X coordinate of the car
        self.__y = 0.0  # Current Y coordinate of the car

    def get_gas(self):
        return self.__gas

    def get_odometer(self):
        return self.__odometer

    def get_name(self):
        return self.__name

    def get_tankvolume(self):
        return self.__tank_volume

    def get_consumption(self):
        return self.__consumption

    def get_coordinates(self):
        xy = [self.__x, self.__y]
        return xy

    def zeroing(self):
        """
        Sets all cars values to 0.
        """
        self.__gas = 0
        self.__odometer = 0
        self.__x = 0.0
        self.__y = 0.0

    def fill(self, to_fill):
        """
        Fills the tank.
        :param to_fill: float, how much gas to fill.
        """
        if self.__gas + to_fill >= self.__tank_volume:
            self.__gas = self.__tank_volume
        else:
            self.__gas += to_fill

    def drive(self, x, y):
        """
        Drives the car.
        :param x: The destination x coordinate
        :param y: The destination y coordinate
        """
        distance = sqrt((x-self.__x)**2 + (y-self.__y)**2)
        gas_used = distance * (self.__consumption / 100)

        if gas_used >= self.__gas:
            distance2 = (100 * self.__gas) / self.__consumption
            s = distance2 / distance

            self.__x += s * (x - self.__x)
            self.__y += s * (y - self.__y)

            self.__odometer += distance2
            self.__gas = 0
        else:
            self.__x = x
            self.__y = y
            self.__odometer += distance
            self.__gas -= gas_used
