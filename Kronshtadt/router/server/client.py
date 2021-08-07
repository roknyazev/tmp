import urllib.parse
from http.client import *

params = urllib.parse.urlencode({'@number': 12524, '@type': 'issue', '@action': 'show'})
headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
conn = HTTPConnection('localhost:80')
conn.request("POST", "", "{\"weight\": 2, \"first_hub\": 1, \"last_hub\": 120}", headers)
resp = conn.getresponse()
data = (resp.read())

print(data)
