import pyrebase
import time

# Change the config files!
firebaseConfig = {
}

firebase = pyrebase.initialize_app(firebaseConfig)

db = firebase.database()
auth = firebase.auth()

user = auth.sign_in_with_email_and_password("rickypapudi@gmail.com", "despro123")

while(True): 
    humans = open('humans.txt','r').read()
    print(humans)

    capacity = open('capacity.txt', 'r').read()
    print(capacity)
    uid = str(user.get("localId"))
    ut = str(int(time.time()))
    print(ut)
    print(str(uid))
    data = {"humans":humans, "capacity":capacity, "time":ut}
    db.child("pushtime").push(ut)
    db.child("UsersData").child(uid).child("readings").child(ut).set(data)
    db.child("UsersData").child(uid).child("charts").set({"range":"1000"})
    
    time.sleep(20)

