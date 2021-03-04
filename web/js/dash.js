
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

  // read du lieu firebase 

  var nhietdo = firebase.database().ref().child("Sensor/T1")
  nhietdo.on('value',function(nhietdo){
    // AS.innerHTML=nhietdo.val();
    setGaugeValuend(gaugeElementnd, nhietdo.val());
    console.log(nhietdo.val());
    });
  // ph
    var doph = firebase.database().ref().child("Sensor/PH")
  doph.on('value',function(doph){
    // AS.innerHTML=doph.val();
    setGaugeValueph(gaugeElementph, doph.val());
    console.log(doph.val());
    });
//do am
var doam = firebase.database().ref().child("Sensor/DA")
doam.on('value',function(doam){
  // AS.innerHTML=doam.val();
  setGaugeValueda(gaugeElementda, doam.val());

  console.log(doam.val());
});
// anh sang
var anhsang = firebase.database().ref().child("Sensor/AS")
anhsang.on('value',function(anhsang){
  // AS.innerHTML=anhsang.val();
  setGaugeValueas(gaugeElementas, anhsang.val());

  console.log(anhsang.val());
});
// nhiet do nuoc
var ndnuoc = firebase.database().ref().child("Sensor/NDN")
ndnuoc.on('value',function(ndnuoc){
  // AS.innerHTML=ndnuoc.val();
  setGaugeValuendn(gaugeElementndn,  ndnuoc.val());
  

  console.log(ndnuoc.val());
});
// box
const gaugeElement = document.querySelector(".gauge");



function setGaugeValue1(gauge, value) {
  if (value < 0 || value > 200) {
    return;
  }

  gauge.querySelector(".gauge__fill").style.transform = `rotate(${
    value/200
  }turn)`;
  
  
  gauge.querySelector(".gauge__cover").textContent = `${(value)}%`;
}
setGaugeValue1(gaugeElement, 10);



// 

const gaugeElement1 = document.querySelector(".gauge1");

function setGaugeValue(gauge, value) {
  if (value < 0 || value > 200) {
    return;
  }

  gauge.querySelector(".gauge__fill1").style.transform = `rotate(${
    value/200
  }turn)`;

  gauge.querySelector(".gauge__cover1").textContent = `${(value)}°C`;


}
setGaugeValue(gaugeElement1, 15);
// ph
const gaugeElementph = document.querySelector(".gauge_ph");

function setGaugeValueph(gauge, value) {
  if (value < 0 || value > 200) {
    return;
  }

  gauge.querySelector(".gauge__fill_ph").style.transform = `rotate(${
    value/200
  }turn)`;

  gauge.querySelector(".gauge__cover_ph").textContent = `${(value)}%`;


}
setGaugeValueph(gaugeElementph, 5.5);
// da
const gaugeElementda = document.querySelector(".gauge_da");

function setGaugeValueda(gauge, value) {
  if (value < 0 || value > 200) {
    return;
  }

  gauge.querySelector(".gauge__fill_da").style.transform = `rotate(${
    value/200
  }turn)`;

  gauge.querySelector(".gauge__cover_da").textContent = `${(value)}%`;


}
setGaugeValueda(gaugeElementda, 15);
// anh sang
const gaugeElementas = document.querySelector(".gauge_as");

function setGaugeValueas(gauge, value) {
  if (value < 0 || value > 200) {
    return;
  }

  gauge.querySelector(".gauge__fill_as").style.transform = `rotate(${
    value/200
  }turn)`;

  gauge.querySelector(".gauge__cover_as").textContent = `${(value)} lux`;


}
setGaugeValueas(gaugeElementas, 25);
// nhiet do nuoc
const gaugeElementndn = document.querySelector(".gauge_ndn");

function setGaugeValuendn(gauge, value) {
  if (value < 0 || value > 200) {
    return;
  }

  gauge.querySelector(".gauge__fill_ndn").style.transform = `rotate(${
    value/200
  }turn)`;

  gauge.querySelector(".gauge__cover_ndn").textContent = `${(value)}°C`;


}
setGaugeValuendn(gaugeElementndn, 80);
// nhiet do 
const gaugeElementnd = document.querySelector(".gauge_nd");

function setGaugeValuend(gauge, value) {
  if (value < 0 || value > 200) {
    return;
  }

  gauge.querySelector(".gauge__fill_nd").style.transform = `rotate(${
    value/200
  }turn)`;

  gauge.querySelector(".gauge__cover_nd").textContent = `${(value)}°C`;


}
setGaugeValuend(gaugeElementnd, 30.5);









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
// switch 
// document.addEventListener('DOMContentLoaded', function () {
//   var checkbox = document.querySelector('input[type="checkbox"]');

//   checkbox.addEventListener('change', function () {
//     if (checkbox.checked) {
//       // do this
//       console.log('Auto');
//       firebase.database().ref().child("auto_man").set("1");
//     } else {
//       // do that
//       console.log('Not checked');
//       firebase.database().ref().child("auto_man").set("0");
//     }
//   });
// });
// switch 
var automan = firebase.database().ref().child("auto_man")
var tes1 =document.getElementById('t_auto_man1')
automan.on('value',function(automan){
  // AS.innerHTML=automan.val();
  
  if(automan.val()==1){
    console.log("auto")
     tes1.innerText= "Auto"
     tes1.style.color= "white"
    document.getElementById('btn_auto_man').style.backgroundColor=' #4391f0'
    document.getElementById('chedo').innerText="Auto"
    document.getElementById('slider_nd').style.display='none'
    document.getElementById('slider_ndn').style.display='none'
    document.getElementById('slider_da').style.display='none'
    document.getElementById('slider_as').style.display='none'
    document.getElementById('slider_ph').style.display='none'
  }
  if(automan.val()==0){
    console.log("man")

    tes1.innerText= "Manual"
    tes1.style.color= "back"
    document.getElementById('btn_auto_man').style.backgroundColor='white'
    document.getElementById('chedo').innerText="Manual"
    document.getElementById('slider_nd').style.display='block'
    document.getElementById('slider_ndn').style.display='block'
    document.getElementById('slider_da').style.display='block'
    document.getElementById('slider_as').style.display='block'
    document.getElementById('slider_ph').style.display='block'
  }
  });

 
 
  function btn_man_auto(){
    if (tes1.innerHTML==="Manual"){
       tes1.innerText= "Auto"
       tes1.style.color= "white"
     document.getElementById('btn_auto_man').style.backgroundColor=' #4391f0'
     firebase.database().ref().child("auto_man").set("1");
    }
    
    else{
      tes1.innerText= "Manual"
      tes1.style.color= "back"
      document.getElementById('btn_auto_man').style.backgroundColor='white'
      firebase.database().ref().child("auto_man").set("0");
    }
  }
  // 
  var slider = document.getElementById("myRange");
var output = document.getElementById("demo");
var output_da = document.getElementById("demo_da");
var output_nd = document.getElementById("demo_nd");
var output_ndn = document.getElementById("demo_ndn");
var output_as = document.getElementById("demo_as");

output.innerHTML = slider.value;
output_da.innerHTML=slider.value
output_nd.innerHTML=slider.value
output_ndn.innerHTML=slider.value
output_as.innerHTML =slider.value

slider.oninput = function() {
  output.innerHTML = this.value;
  output_da.innerHTML=this.value
  output_nd.innerHTML=this.value
  output_ndn.innerHTML=this.value
  output_as.innerHTML =this.value
}



var ctx = document.getElementById('myChart12').getContext('2d');
var myChart = new Chart(ctx, {
    type: 'bar',
    data: {
        labels: ['Red', 'Blue', 'Yellow', 'Green', 'Purple', 'Orange'],
        datasets: [{
            label: '# of Votes',
            data: [12, 19, 3, 5, 2, 3],
            backgroundColor: [
                'rgba(255, 99, 132, 0.2)',
                'rgba(54, 162, 235, 0.2)',
                'rgba(255, 206, 86, 0.2)',
                'rgba(75, 192, 192, 0.2)',
                'rgba(153, 102, 255, 0.2)',
                'rgba(255, 159, 64, 0.2)'
            ],
            borderColor: [
                'rgba(255, 99, 132, 1)',
                'rgba(54, 162, 235, 1)',
                'rgba(255, 206, 86, 1)',
                'rgba(75, 192, 192, 1)',
                'rgba(153, 102, 255, 1)',
                'rgba(255, 159, 64, 1)'
            ],
            borderWidth: 1
        }]
    },
    options: {
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true
                }
            }]
        }
    }
});