import requests
import random
import time
import json


#dep = 211
#dst = 183
while True:
    time.sleep(2)
    aa = {}
    for i in range(1000):
        dep = -1
        dst = -1
        while dep == dst:
            dep = random.randint(0, 211)
            dst = random.randint(0, 211)
        order_dict = {"weight": 10, "first_hub": str(dep), "last_hub": str(dst)}
        aa[str(i)] = order_dict
        query = json.dumps(aa)
        print(query)
        r = requests.post('http://localhost:10000', data = query)
    print(r)
