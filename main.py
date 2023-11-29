import pyrebase
import time

firebaseConfig = {
    'apiKey': "AIzaSyBXNk5F1i8BDfT4t8VPLSyvj-EpFkvIqSY",
    'authDomain': "desain-projek.firebaseapp.com",
    'databaseURL': "https://desain-projek-default-rtdb.asia-southeast1.firebasedatabase.app",
    'projectId': "desain-projek",
    'storageBucket': "desain-projek.appspot.com",
    'messagingSenderId': "213519052918",
    'appId': "1:213519052918:web:9567bd6462c94eeccb6f57",
    'measurementId': "G-93YVMEQCFQ"}

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

