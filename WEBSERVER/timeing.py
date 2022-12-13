from datetime import datetime

timestamps = [
    1670939976108,
    1670941777897,
    1670943579620,
    1670945381419,
    1670947183230,
    1670948984960,
    1670950786810,
    1670952588706,
    1670954390394,
    1670956190737,
    1670957992449,
    1670959794461
]

for i in timestamps:

    tajm = (i / 1000) + 3600
    print(datetime.utcfromtimestamp(tajm))
