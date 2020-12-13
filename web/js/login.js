
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
     var rootRef = firebase.database().ref();
      const auth = firebase.auth();
      
      function  signIn(){
        var gmail= document.getElementById("user");
        var pass = document.getElementById("pass");
        const login =auth.signInWithEmailAndPassword(gmail.value, pass.value);
         
        // login.catch(e => alert(e.message))
        login.catch(e=> document.getElementById('qmk').innerHTML = e.message)
        
       login.then(function (_result) {
         window.location.replace('home.html');
         console.log("tesst")
       })
     
      }
      