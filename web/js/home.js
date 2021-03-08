
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
        showSuccessToast();
        document.getElementById('modal_ecall').style.display='none'
      }
  
    } else {
         document.getElementById('modal_ecall').style.display='block'
         showErrorToast();
      // No user is signed in.
  
   
  
    }
  });





function showw(){
var read = document.getElementById('T').value
var name1 = document.getElementById('name')
var T = document.getElementById('T1')
var DA = document.getElementById('DA')
var AS = document.getElementById('AS')
var time = document.getElementById('Time')
var PH = document.getElementById('PH')


// doc du lieu firebase 
var reaname = firebase.database().ref().child("Strawberry/Name")
var readT = firebase.database().ref().child("Strawberry/T")
var readDA = firebase.database().ref().child("Strawberry/DA")
var readAS= firebase.database().ref().child("Strawberry/AS")
var readPH = firebase.database().ref().child("Strawberry/PH")
var readtime= firebase.database().ref().child("Strawberry/Time")





if(read=="DT"){
      console.log("tesst")
      firebase.database().ref().child("man/as").set();
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




function toast({ title = "", message = "", type = "info", duration = 3000 }) {
  const main = document.getElementById("toast");
  if (main) {
    const toast = document.createElement("div");

    // Auto remove toast
    const autoRemoveId = setTimeout(function () {
      main.removeChild(toast);
    }, duration + 1000);

    // Remove toast when clicked
    toast.onclick = function (e) {
      if (e.target.closest(".toast__close")) {
        main.removeChild(toast);
        clearTimeout(autoRemoveId);
      }
    };

    const icons = {
      success: "fas fa-check-circle",
      info: "fas fa-info-circle",
      warning: "fas fa-exclamation-circle",
      error: "fas fa-exclamation-circle"
    };
    const icon = icons[type];
    const delay = (duration / 1000).toFixed(2);

    toast.classList.add("toast", `toast--${type}`);
    toast.style.animation = `slideInLeft ease .3s, fadeOut linear 1s ${delay}s forwards`;

    toast.innerHTML = `
                    <div class="toast__icon">
                        <i class="${icon}"></i>
                    </div>
                    <div class="toast__body">
                        <h3 class="toast__title">${title}</h3>
                        <p class="toast__msg">${message}</p>
                    </div>
                    <div class="toast__close">
                        <i class="fas fa-times"></i>
                    </div>
                `;
    main.appendChild(toast);
  }
}

function showSuccessToast() {
  toast({
    title: "Đăng nhập Thành công!",
    message: "Bạn đã đăng ký thành công tài khoản.",
    type: "success",
    duration: 5000
  });
}

function showErrorToast() {
  toast({
    title: "Thất bại!",
    message: "Có lỗi xảy ra, vui lòng liên hệ quản trị viên.",
    type: "error",
    duration: 5000
  });
}
// function autobtn(){
//   firebase.database().ref().child("id").set("3245");
  
 
// }
x= document.getElementById('btnauto')

function sukien1(){
  console.log("oke da gui")
  firebase.database().ref().child("auto_man").set("1");
}
function sukien2(){
  window.open('dashboad.html','_self')
}
function sukien3(){
  window.open('dashboad.html','_self')
}
function sukien4(){
  firebase.database().ref().child("auto_man").set("0");
}

//  gui du lieu lenj set 
x.addEventListener("click", sukien1);
//x.addEventListener("click", sukien2);
// document.getElementById('btnauto').onclick=function writeUserData(name) {
//   firebase.database().ref().child("id").set("aas");
//   //setTimeout(alert("After 5 seconds!"),2000);
  
 
// }