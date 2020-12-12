
  var firebaseConfig = {
      apiKey: "AIzaSyArTE_49kNYvXSVeI3S54mkwPA-P4ZvWrM",
      authDomain: "hydroponic-7b19c.firebaseapp.com",
      projectId: "hydroponic-7b19c",
      storageBucket: "hydroponic-7b19c.appspot.com",
      messagingSenderId: "279353770869",
      appId: "1:279353770869:web:d85be35a8e7d81ab4fc93c",
      measurementId: "G-ND5XK9RQ6J"
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
       })
     
      }