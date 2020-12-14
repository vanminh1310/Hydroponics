
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

    function logout(){
      firebase.auth().signOut();
      window.open('index.html','_self')
    }
  firebase.auth().onAuthStateChanged(function(user) {
    if (user) {
      // User is signed 
      var user = firebase.auth().currentUser;
  
      if(user != null){
  
        var email_id = user.email;
        console.log(email_id)
       
        document.getElementById('modal_ecall').style.display='none'
      }
  
    } else {
         document.getElementById('modal_ecall').style.display='block'
      // No user is signed in.
  
   
  
    }
  });

// box
const gaugeElement = document.querySelector(".gauge");

function setGaugeValue(gauge, value) {
  if (value < 0 || value > 200) {
    return;
  }

  gauge.querySelector(".gauge__fill").style.transform = `rotate(${
    value/200
  }turn)`;
  
  
  gauge.querySelector(".gauge__cover").textContent = `${(value)}%`;
}

setGaugeValue(gaugeElement, 60.5);
