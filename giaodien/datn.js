// ============= LOAD SENSOR REALTIME =============

// Nhiệt độ chung
firebase.database().ref("smart_garden/sensor/vuon1/temp/live").on("value", s => {
  const v = s.val();
  document.getElementById("k1_temp").textContent =
    (v !== null ? parseInt(v) : "--") + "°C";
});

firebase.database().ref("smart_garden/sensor/vuon1/temp/live").on("value", s => {
  const v = s.val();
  document.getElementById("k2_temp").textContent =
    (v !== null ? parseInt(v) : "--") + "°C";
});

firebase.database().ref("smart_garden/sensor/vuon1/temp/live").on("value", s => {
  const v = s.val();
  document.getElementById("k3_temp").textContent =
    (v !== null ? parseInt(v) : "--") + "°C";
});




// Độ ẩm không khí chung 
firebase.database().ref("smart_garden/sensor/vuon1/humi/live").on("value", s => {
  const v = s.val();
  document.getElementById("k1_humi").textContent = (v !== null ? v : "--") + "%";
});
firebase.database().ref("smart_garden/sensor/vuon1/humi/live").on("value", s => {
  const v = s.val();
  document.getElementById("k2_humi").textContent = (v !== null ? v : "--") + "%";
});
firebase.database().ref("smart_garden/sensor/vuon1/humi/live").on("value", s => {
  const v = s.val();
  document.getElementById("k3_humi").textContent = (v !== null ? v : "--") + "%";
});

// Độ ẩm đất rieng
firebase.database().ref("smart_garden/sensor/vuon1/soil/live").on("value", s => {
  const v = s.val();
  document.getElementById("k1_soil").textContent = (v !== null ? v : "--") + "%";
});
firebase.database().ref("smart_garden/sensor/vuon2/soil/live").on("value", s => {
  const v = s.val();
  document.getElementById("k2_soil").textContent = (v !== null ? v : "--") + "%";
});
firebase.database().ref("smart_garden/sensor/vuon3/soil/live").on("value", s => {
  const v = s.val();
  document.getElementById("k3_soil").textContent = (v !== null ? v : "--") + "%";
});
// Ánh sáng chung
firebase.database().ref("smart_garden/sensor/vuon1/light/live").on("value", s => {
  const v = s.val();
  document.getElementById("k1_light").textContent = (v !== null ? v : "--") + "LUX";
});
firebase.database().ref("smart_garden/sensor/vuon1/light/live").on("value", s => {
  const v = s.val();
  document.getElementById("k2_light").textContent = (v !== null ? v : "--") + "LUX";
});
firebase.database().ref("smart_garden/sensor/vuon1/light/live").on("value", s => {
  const v = s.val();
  document.getElementById("k3_light").textContent = (v !== null ? v : "--") + "LUX";
});




// ============= ĐẾM TỔNG THIẾT BỊ ĐANG BẬT TOÀN HỆ THỐNG =============
function countAllDevicesOn() {
  const paths = [
    "smart_garden/control/vuon1",
    "smart_garden/control/vuon2",
    "smart_garden/control/vuon3"
  ];

  paths.forEach(path => {
    firebase.database().ref(path).on("value", snap => {
      let total = 0;

      paths.forEach(p => {
        firebase.database().ref(p).once("value", s => {
          const data = s.val();
          if (data) {
            Object.values(data).forEach(v => {
              if (Number(v) === 1) total++;
            });
          }
          document.getElementById("device").textContent = total;
        });
      });
    });
  });
}

// GỌI HÀM
countAllDevicesOn();


// ============= BẬT / TẮT TẤT CẢ (ĐÚNG THỰC TẾ) =============

// 🔴 TẮT HẾT
document.getElementById("all_off").addEventListener("click", () => {
  // Thiết bị dùng chung
  firebase.database().ref("smart_garden/control/vuon1").update({
    bulb: 0,
    spray: 0,
     fan: 0,
      pump: 0

  });

  // Thiết bị riêng từng vườn
  ["vuon1", "vuon2", "vuon3"].forEach(v => {
    firebase.database().ref(`smart_garden/control/${v}`).update({
      fan: 0,
      pump: 0
    });
  });
});

// 🟢 BẬT HẾT
document.getElementById("all_on").addEventListener("click", () => {
  // Thiết bị dùng chung
  firebase.database().ref("smart_garden/control/vuon1").update({
    bulb: 1,
    spray: 1,
     fan: 1,
      pump: 1
  });

  // Thiết bị riêng từng vườn
  ["vuon1", "vuon2", "vuon3"].forEach(v => {
    firebase.database().ref(`smart_garden/control/${v}`).update({
      fan: 1,
      pump: 1
    });
  });
});


// ============= ĐI ĐẾN VƯỜN =============
document.getElementById("garden_1").addEventListener("click", () => {
  window.location.href = "vuon1.html";
});
// ============= ĐI ĐẾN VƯỜN =============
document.getElementById("garden_2").addEventListener("click", () => {
  window.location.href = "vuon2.html";
});
// ============= ĐI ĐẾN VƯỜN =============
document.getElementById("garden_3").addEventListener("click", () => {
  window.location.href = "vuon3.html";
});

// ===================== toggle bell ======================
const bell = document.getElementById("notify-bell");
const box  = document.getElementById("notify-box");
const badge = document.getElementById("notify-count");


// ẩn popup mặc định
box.classList.add("hidden");

// Khi user bấm chuông xem => reset unread
bell.addEventListener("click", ()=>{
    box.classList.toggle("hidden");

    // lưu thời điểm user CLICK xem
    localStorage.setItem("notify_last_read", Date.now() / 1000);

    badge.style.display = "none";
});


// ================= Firebase WARNING logic =================
const warnRef  = firebase.database().ref("/smart_garden/warning/list");

// convert timestamp -> VN
function formatTime(ts){
    if(!ts) return "";
    const d = new Date(ts * 1000);

    const day   = ("0"+d.getDate()).slice(-2);
    const month = ("0"+(d.getMonth()+1)).slice(-2);
    const year  = d.getFullYear();
    const hour  = ("0"+d.getHours()).slice(-2);
    const min   = ("0"+d.getMinutes()).slice(-2);

    return `${day}/${month}/${year} ${hour}:${min}`;
}



// ==================== HIỂN THỊ THÔNG BÁO =====================
function displayGardenName(id){
  if(id === "system") return "Toàn khu vườn";
  if(id === "vuon1") return "Vườn 1";
  if(id === "vuon2") return "Vườn 2";
  if(id === "vuon3") return "Vườn 3";
  return id;
}

function getUnitFromMsg(msg){
  msg = msg.toLowerCase();

  if (msg.includes("nhiệt")) return "°C";
  if (msg.includes("độ ẩm đất")) return "%";
  if (msg.includes("độ ẩm")) return "%";
  if (msg.includes("ánh sáng")) return "LUX";

  return "";
}

function renderWarning(data){
    const list = document.getElementById("notify-list");

    // timestamp lần cuối user xem
    const lastRead = parseFloat(localStorage.getItem("notify_last_read") || 0);

    // nếu không có dữ liệu
    if(!data){
        list.innerHTML = `<li>Không có cảnh báo nào.</li>`;
        badge.style.display = "none";
        return;
    }

let arr = Object.values(data);

// 🔽 mới nhất lên trước
arr.sort((a,b)=> b.time - a.time);

// ✅ CHỈ LẤY 20 TIN GẦN NHẤT
arr = arr.slice(0, 10);


    // render danh sách
 list.innerHTML = arr.map(item => `
    <li>
      
<b>${displayGardenName(item.garden)}</b> — ${item.msg}


        <div class="time">${formatTime(item.time)}</div>
    </li>
`).join("");


    // ======= TÍNH SỐ UNREAD =======
    let unread = arr.filter(item => item.time > lastRead).length;

    if(unread > 0 && box.classList.contains("hidden")){
        badge.style.display = "block";
        badge.textContent = unread > 9 ? "9+" : unread;
    } else {
        badge.style.display = "none";
    }
}


// ===================== LẮNG NGHE CẢNH BÁO ======================
warnRef.on("value", snap => {
    renderWarning(snap.val());
});
function getGreeting() {
    const hour = new Date().getHours();
    let greet = "Xin chào";

    if (hour >= 5 && hour < 11) {
      greet = "Chào buổi sáng";
    } else if (hour >= 11 && hour < 13) {
      greet = "Chào buổi trưa";
    } else if (hour >= 13 && hour < 18) {
      greet = "Chào buổi chiều";
    } else {
      greet = "Chào buổi tối";
    }

    document.getElementById("greetingText").innerHTML =
      `${greet}, <b>Quốc Bảo</b> 👋`;
  }

  getGreeting();
  // ================= MỰC NƯỚC: ĐẦY / THẤP / CẠN =================
firebase.database()
.ref("smart_garden/water_level/live")
.on("value", snap => {
  const v = snap.val();


  const card = document.getElementById("water_card");
  const text = document.getElementById("water_text");

  if (v === null) {
    text.innerText = "--";
    return;
  }

  if (v >= 65) {
    text.innerText = "ĐẦY";
    text.classList.remove("blink");
    card.className = "metric-item water full";
  }
  else if (v > 35) {
    text.innerText = "THẤP";
    text.classList.remove("blink");
    card.className = "metric-item water low";
  }
else {
  text.innerText = "CẠN ⚠️";
  text.classList.add("blink");
  card.className = "metric-item water empty";
}

});
document.getElementById("status").textContent = "ĐANG PHÁT TRIỂN";
