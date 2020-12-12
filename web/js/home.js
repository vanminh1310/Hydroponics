
  var firebaseConfig = {
      apiKey: "AIzaSyCNMHSlvUOgSTrFr-0HbGMaIE7IFRwbzjA",
      authDomain: "iot59tdh.firebaseapp.com",
      databaseURL: "https://iot59tdh-default-rtdb.firebaseio.com",
      projectId: "iot59tdh",
      storageBucket: "iot59tdh.appspot.com",
      messagingSenderId: "773261677026",
      appId: "1:773261677026:web:0a3207a929d48c44efe9bb",
      measurementId: "G-Z5YTNQ2CG2"
    };
    // Initialize Firebase
    firebase.initializeApp(firebaseConfig);


function showw(){
var read = document.getElementById('T').value
var name1 = document.getElementById('name')
var T = document.getElementById('T1')
var DA = document.getElementById('DA')
var AS = document.getElementById('AS')
var time = document.getElementById('Time')
var PH = document.getElementById('PH')


//
var reaname = firebase.database().ref().child("Strawberry/Name")
var readT = firebase.database().ref().child("Strawberry/T")
var readDA = firebase.database().ref().child("Strawberry/DA")
var readAS= firebase.database().ref().child("Strawberry/AS")
var readPH = firebase.database().ref().child("Strawberry/PH")
var readtime= firebase.database().ref().child("Strawberry/Time")





if(read=="DT"){
      console.log("tesst")
      reaname.on('value',function(reaname){
            name1.innerHTML=reaname.val();
            console.log(reaname.val());
            console.log("co chay")
            });
            readAS.on('value',function(readAS){
                  AS.innerHTML=readAS.val();
                  console.log(readAS.val());
                  });
                  readDA.on('value',function(readDA){
                        DA.innerHTML=readDA.val();
                        console.log(readDA.val());
                        });
                        readT.on('value',function(readT){
                              T.innerHTML=readT.val();
                              console.log(readT.val());
                              });
                              readtime.on('value',function(readtime){
                                    time.innerHTML=readtime.val();
                                    console.log(readtime.val());
                                    });
                                    readPH.on('value',function(readPH){
                                          PH.innerHTML=readPH.val();
                                          console.log(readPH.val());
                                          });
      

}
if(read =="t"){
      PH.innerHTML="none"   
}
      
}