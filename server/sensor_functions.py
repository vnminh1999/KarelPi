from sense_hat import SenseHat

def get_direction():
    sense = SenseHat()
    heading = sense.get_compass()

    if heading < 45 or heading >= 315:
        return 'N'
    elif heading < 135:
        return 'E'
    elif heading < 225:
        return 'S'
    else:
        return 'W'