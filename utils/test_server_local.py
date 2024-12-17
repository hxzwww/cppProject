import requests
import wget
import json

# Server URL
url = "http://localhost:80"

numbers = [3, 1, 7, 4, 6, 1, 8]

data = '{"numbers": ' + f'{numbers}}}'

response = requests.post(url + '/upload', headers={"Content-Type": "application/json"}, data=data)

print("Response Code:", response.status_code)

filename = response.json()['file']

filename = filename[
    filename.find('tmp') + 3 : -2
]

link = url + '/downloads' + filename


name = wget.download(link)

with open(name, 'r') as f:
    data = json.load(f)

server_sorted_nums = data['numbers']
real_srted_nums = sorted(numbers)

print('\nResult:', server_sorted_nums == real_srted_nums)