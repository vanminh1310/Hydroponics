
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
         showErrorToast()
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

setGaugeValue(gaugeElement, 10);


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
    title: "Hãy đăng nhập để tiếp tục!",
    message: "Có lỗi xảy ra, vui lòng liên hệ quản trị viên.",
    type: "error",
    duration: 5000
  });
}