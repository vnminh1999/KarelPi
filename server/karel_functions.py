import sensor_functions as sensors

def move():
    return 1

def turn_left():
    return 2

def turn_right():
    return 3

def wall_in_front():
    return 4

def wall_to_left():
    return 5

def wall_to_right():
    return 6

def facing_north():
    is_north = 1 if (sensors.get_direction() == 'N') else 0
    return is_north

def facing_east():
    is_east = 1 if (sensors.get_direction() == 'E') else 0
    return is_east

def facing_south():
    is_south = 1 if (sensors.get_direction() == 'S') else 0
    return is_south

def facing_west():
    is_west = 1 if (sensors.get_direction() == 'W') else 0
    return is_west