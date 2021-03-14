
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
    document.getElementById('slider_d_min').style.display='none'
    document.getElementById('slider_a_min').style.display='none'
    document.getElementById('slider_n_min').style.display='none'
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
    document.getElementById('slider_d_min').style.display='block'
    document.getElementById('slider_a_min').style.display='block'
    document.getElementById('slider_n_min').style.display='block'
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
  // max slider 
  var slider = document.getElementById("myRange");
  var s_h = document.getElementById("slider_h");
  var s_nd_n = document.getElementById("slider_nd_n");
  var s_nd = document.getElementById("slider_n_d");
  var s_as = document.getElementById("slider_s");
  // SLIDER ndnlmt
  var ndnlmt = document.getElementById("slider_n_pin")
  var output_nlmt = document.getElementById("demo_ndn_pin")
  output_nlmt.innerHTML = ndnlmt.value

  ndnlmt.oninput = function() {
    output_nlmt.innerHTML = this.value;
    firebase.database().ref().child("man/ndpin").set(this.value);
  
  }
// min slider
var min_da = document.getElementById('slider_d_a_min')
var min_as = document.getElementById('slider_a_s_min')
var min_nd = document.getElementById('slider_n_d_min')

// max innerHTML
var output = document.getElementById("demo");
var output_da = document.getElementById("demo_da");
var output_nd = document.getElementById("demo_nd");
var output_ndn = document.getElementById("demo_ndn");
var output_as = document.getElementById("demo_as");
// min innerHTML
var output_min_as = document.getElementById("demo_as_min")
var output_min_nd = document.getElementById("demo_nd_min")
var output_min_da = document.getElementById("demo_da_min")

output_min_as.innerHTML = min_as.value
output_min_da.innerHTML = min_da.value
output_min_nd.innerHTML= min_nd.value

output.innerHTML = slider.value;
output_da.innerHTML = s_h.value
output_nd.innerHTML = s_nd.value
output_ndn.innerHTML = s_nd.value
output_as.innerHTML = s_as.value



 var doph_man = firebase.database().ref().child("man/ph")
  doph_man.on('value',function(doph_man){
    // AS.innerHTML=doph_man.val();
    var test = doph_man.val();
    output.innerHTML = doph_man.val()
    slider = doph_man.val();
    console.log(doph_man.val());
    });




var as_man = firebase.database().ref().child("man/max_as")
    as_man.on('value',function(as_man){
      // AS.innerHTML=as_man.val();
      var test = as_man.val();
      output_as.innerHTML = as_man.val()
      console.log(as_man.val());
      });

      var nd_n = firebase.database().ref().child("man/ndn")
      nd_n.on('value',function(nd_n){
        // AS.innerHTML=nd_n.val();
        var test = nd_n.val();
        output_ndn.innerHTML = nd_n.val()
        
        console.log(nd_n.val());
        });

        var nd_t = firebase.database().ref().child("man/max_t")
    nd_t.on('value',function(nd_t){
      // AS.innerHTML=nd_t.val();
      var test = nd_t.val();
      output_nd.innerHTML = nd_t.val()
      
      console.log(nd_t.val());
      });

      var dam = firebase.database().ref().child("man/max_da")
    dam.on('value',function(dam){
      // AS.innerHTML=dam.val();
      var test = dam.val();
      output_da.innerHTML = dam.val()
      
      console.log(dam.val());
      });

      var dam_min = firebase.database().ref().child("man/min_da")
      dam_min.on('value',function(dam_min){
        // AS.innerHTML=dam_min.val();
        var test = dam_min.val();
        output_min_da.innerHTML = dam_min.val()
        
        console.log(dam_min.val());
        });

        var min_nd1 = firebase.database().ref().child("man/min_t")
        min_nd1.on('value',function(min_nd1){
          // AS.innerHTML=min_nd1.val();
          var test = min_nd1.val();
          output_min_nd.innerHTML = min_nd1.val()
          
          console.log(min_nd1.val());
          });

          var min_as1 = firebase.database().ref().child("man/min_as")
          min_as1.on('value',function(min_as1){
            // AS.innerHTML=min_as1.val();
            var test = min_as1.val();
            output_min_as.innerHTML = min_as1.val()
            
            console.log(min_as1.val());
            });




slider.oninput = function() {
  output.innerHTML = this.value;
  firebase.database().ref().child("man/ph").set(this.value);

}


s_as.oninput = function() {
  output_as.innerHTML = this.value;
  firebase.database().ref().child("man/max_as").set(this.value);

}
s_nd.oninput = function() {
  output_nd.innerHTML = this.value;
  firebase.database().ref().child("man/max_t").set(this.value);

}
s_nd_n.oninput = function() {
  output_ndn.innerHTML = this.value;
  firebase.database().ref().child("man/ndn").set(this.value);

}
s_h.oninput = function() {
  output_da.innerHTML = this.value;
  firebase.database().ref().child("man/max_da").set(this.value);

}

min_da.oninput = function() {
  output_min_da.innerHTML = this.value;
  firebase.database().ref().child("man/min_da").set(this.value);

}
min_nd.oninput = function() {
  output_min_nd.innerHTML = this.value;
 firebase.database().ref().child("man/min_t").set(this.value);

}
min_as.oninput = function() {
  output_min_as.innerHTML = this.value;
  firebase.database().ref().child("man/min_as").set(this.value);

}





new Chart(document.getElementById("line-chart"), {
  type: 'line',
  data: {
    labels:[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19],
    datasets: [{ 
        data: [86,114,106,106,107,111,133,221,783,2478],
        label: "Nhiệt độ",
        borderColor: "#3e95cd",
        fill: false
      }, { 
        data: [282,350,411,502,635,809,947,1402,3700,5267],
        label: "Độ ẩm",
        borderColor: "#8e5ea2",
        fill: false
      }, { 
        data: [168,170,178,190,203,276,408,547,675,734],
        label: "Điện Năng",
        borderColor: "#3cba9f",
        fill: false
      }
    ]
  },
  options: {
    title: {
      display: true,
      text: 'World population per region (in millions)'
    }
  }
});