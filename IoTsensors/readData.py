import re
import requests

allTemps = []
curr_temp = 0

allHeartRate = []
currHeartRate = 0

allOx = []
currOx = 0
URL = ""


# Reads line by line
def read_data():
    biostats = open(r"biostats.txt", "r")
    stats = biostats.readlines()
    for i in stats:
        if "Obj Temp" in i:
            obj = re.findall(r'\d+', i)
            allTemps.append(obj[0])
            curr_temp = obj[0]

        elif "Heart Rate" in i:
            rate = re.findall(r'\d+', i)
            allHeartRate.append(rate[0])
            currHeartRate = rate[0]

        elif "Oxygen" in i:
            ox = re.findall(r'\d+', i)
            allOx.append(ox[0])
            currOx = ox[0]
    biostats.close()


def averages():
    temp_avg = sum(allTemps) / len(allTemps)
    hr_avg = sum(allHeartRate) / len(allHeartRate)
    ox_avg = sum(allOx) / len(allOx)

    return [temp_avg, hr_avg, ox_avg]


read_data()
avg = averages()

data = {'avg_THO': averages(),
        'temp': curr_temp,
        'heartrate': currHeartRate,
        'oxygen': currOx}

r = requests.post(url=URL, data=data)
