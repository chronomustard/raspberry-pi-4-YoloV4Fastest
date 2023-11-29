import pyrebase
import time

# Change the config files!

firebaseConfig = {
    'apiKey': "API",
    'authDomain': "AUTH",
    'databaseURL': "DBURL",
    'projectId': "IDPROJ",
    'storageBucket': "BUCKER",
    'messagingSenderId': "SENDERID",
    'appId': "APPID",
    'measurementId': "MEASUREMNENT ID"}

firebase = pyrebase.initialize_app(firebaseConfig)

db = firebase.database()
auth = firebase.auth()

user = auth.sign_in_with_email_and_password("rickypapudi@gmail.com", "despro123")

while(True): 
    humans = open('humans.txt','r').read()
    print(humans)

    capacity = open('capacity.txt', 'r').read()
    print(capacity)

    ut = str(int(time.time()))
    print(ut)

    data = {"humans":humans, "capacity":capacity, "time":ut}
    db.child(ut).push(data)
    
    time.sleep(40)

