from datetime import datetime
import pandas as pd
import boto3

from secret import *

class Database:

    def __init__(self) -> None:
        self.dynamodb = boto3.resource('dynamodb', region_name='eu-north-1')

    def get_data(self):

        table = self.dynamodb.Table('WATERPLANT')
        response = table.scan()
        data = response['Items']
        moist_array = []
        moist_raw_array = []
        time_array = []

        for msr in data:
            moist_raw = msr['moisture']['moisture']
            moist_raw_array.append(moist_raw)
            #moist = float("%.2f" % float(100 - ((moist_raw/1023)*100))) # get percentage
            moist = self._map(moist_raw, 5, 600, 0, 100)
            tajm_raw = int((msr['sample_time'])/1000)+3600
            tajm = datetime.utcfromtimestamp(tajm_raw)
            moist_array.append(moist)
            time_array.append(tajm)

        df = pd.DataFrame({"Datetime": time_array, "Moisture": moist_array, "Raw": moist_raw_array})   
        df = df.sort_values(by='Datetime')    
        return df

    #  Prominent Arduino map function :)
    def _map(self, x, in_min, in_max, out_min, out_max):
        return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
	


if __name__ == "__main__":
    foo = Database()
    data = foo.get_data_test()
    print(data)