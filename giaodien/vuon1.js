// ==================== FIREBASE CONFIG ====================
const firebaseConfig = {
  apiKey: "AIzaSyBGm7yyK1MKP2F17ZBszG68D5ppJTvfCnU",
  authDomain: "smartgarden-85ec4.firebaseapp.com",
  databaseURL: "https://smartgarden-85ec4-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "smartgarden-85ec4",
  storageBucket: "smartgarden-85ec4.appspot.com",
  appId: "1:875575541081:web:04a89cced5140c4a5ba048"
};

firebase.initializeApp(firebaseConfig);
const database = firebase.database();

// ================= DOM ELEMENTS =================
const btnBulb  = document.getElementById("bulb_button");
const btnFan   = document.getElementById("fan_but");
const btnPump  = document.getElementById("pump_button");
const btnSpray = document.getElementById("phun_button");

const autoBtn   = document.getElementById("auto-btn");
const manualBtn = document.getElementById("manual-btn");

// ================= FIREBASE PATH =================
const CTRL   = "smart_garden/control/vuon1";
const SENSOR = "smart_garden/sensor/vuon1";
const MODE = "smart_garden/mode/vuon1";

const MODE1 = "smart_garden/mode/vuon1";
const MODE2 = "smart_garden/mode/vuon2";
const MODE3 = "smart_garden/mode/vuon3";

let mode1 = 0, mode2 = 0, mode3 = 0;

// ================= IMAGE PATH =================

const CAPTURE_REQ = "/smart_garden/camera/capture";

const RAW  = "/smart_garden/image/vuon1";
const CROP = "/smart_garden/image_crop/vuon1";
const CLOUD_NAME = "djqp5gxru";
const UPLOAD_PRESET = "garden1";                 // preset của bạn
const CLOUD_FOLDER = "garden1_crop";             // ✅ folder lưu ảnh crop



// =====================================================
// ================= CONTROL ===========================
// =====================================================

let bulbState  = 0;
let fanState   = 0;
let pumpState  = 0;
let sprayState = 0;

// -------- BULB --------
btnBulb.onclick = () => {
  database.ref(`${CTRL}/bulb`).set(bulbState ? 0 : 1);
};

database.ref(`${CTRL}/bulb`).on("value", s => {
  bulbState = Number(s.val() ?? 0);
  document.getElementById("bulb").src = bulbState ? "./img/ledon.png" : "./img/ledoff.png";
  document.getElementById("bulb_but").src = bulbState ? "./img/swfan-on.png" : "./img/swfan-off.png";
});

// -------- FAN --------
btnFan.onclick = () => {
  database.ref(`${CTRL}/fan`).set(fanState ? 0 : 1);
};

database.ref(`${CTRL}/fan`).on("value", s => {
  fanState = Number(s.val() ?? 0);

  const fanImg = document.getElementById("fan");
  fanImg.classList.remove("spin1", "spinStop");

  if (fanState) {
    document.getElementById("fan_but").src = "./img/swfan-on.png";
    fanImg.classList.add("spin1");
  } else {
    document.getElementById("fan_but").src = "./img/swfan-off.png";
    fanImg.classList.add("spinStop");
  }
});

// -------- PUMP --------
btnPump.onclick = () => {
  database.ref(`${CTRL}/pump`).set(pumpState ? 0 : 1);
};

database.ref(`${CTRL}/pump`).on("value", s => {
  pumpState = Number(s.val() ?? 0);
  document.getElementById("pump").src = pumpState ? "./img/pumpon.png" : "./img/pumpoff.png";
  document.getElementById("pump_but").src = pumpState ? "./img/swfan-on.png" : "./img/swfan-off.png";
});

// -------- SPRAY --------
btnSpray.onclick = () => {
  database.ref(`${CTRL}/spray`).set(sprayState ? 0 : 1);
};

database.ref(`${CTRL}/spray`).on("value", s => {
  sprayState = Number(s.val() ?? 0);
  document.getElementById("phun").src = sprayState ? "./img/phunon.png" : "./img/phunoff.png";
  document.getElementById("phun_but").src = sprayState ? "./img/swfan-on.png" : "./img/swfan-off.png";
});

// ====================== AUTO/MANUAL ======================
autoBtn.onclick   = () => database.ref(MODE).set(1);
manualBtn.onclick = () => database.ref(MODE).set(0);

// ===== MODE VƯỜN 1 =====
database.ref(MODE1).on("value", s => {
  mode1 = Number(s.val() ?? 0);
  window.autoMode = mode1;

  autoBtn.classList.toggle("active",   mode1 === 1);
  manualBtn.classList.toggle("active", mode1 === 0);

  updateLockGarden1();
});

// ===== MODE VƯỜN 2 =====
database.ref(MODE2).on("value", s => {
  mode2 = Number(s.val() ?? 0);
  updateLockGarden1();
});

// ===== MODE VƯỜN 3 =====
database.ref(MODE3).on("value", s => {
  mode3 = Number(s.val() ?? 0);
  updateLockGarden1();
});


// ==============================================================
// ======================= FORMAT CHO HIỂN THỊ ==================
// ==============================================================

// ⭐⭐ NHIỆT ĐỘ: CHỈ HIỂN THỊ SỐ NGUYÊN ⭐⭐
function fmtTemp(x){
  if(x === undefined || x === null) return "--";
  return Math.floor(Number(x));
}


// ⭐⭐ Humi / Light / Soil → số nguyên ⭐⭐
function fmtInt(x){
  if(x === undefined || x === null) return "--";
  return parseInt(Number(x));
}

// ==============================================================
// ======================= CHART SECTION =========================
// ==============================================================

let temptChart = [];
let humidChart = [];
let lightChartData = [];
let soilChartData = [];

// map dữ liệu live + 1h..9h
function mapChart(data) {
  data = data || {};
  return [
    Number(data.live ?? 0),
    Number(data["1h"] ?? 0),
    Number(data["2h"] ?? 0),
    Number(data["3h"] ?? 0),
    Number(data["4h"] ?? 0),
    Number(data["5h"] ?? 0),
    Number(data["6h"] ?? 0),
    Number(data["7h"] ?? 0),
    Number(data["8h"] ?? 0),
    Number(data["9h"] ?? 0),
  ];
}

// =============== BIỂU ĐỒ 1 — DUY NHẤT ===============
window.currentChart = "temp";

const mainChart = new ApexCharts(document.querySelector("#area-chart"), {
  series: [
    { name: "Nhiệt độ", data: temptChart },
    { name: "Độ ẩm", data: humidChart },
  ],
  chart: { height: 200, type: "area", toolbar: { show: false }, fontFamily: "Segoe UI, sans-serif"},
  stroke: { curve: "smooth" },
  dataLabels: { enabled: false },

  // màu line
  colors: ["#e76f51", "#2a65ff"],

  // gradient mờ cho giống ảnh 2
  fill: {
    type: "gradient",
    gradient: {
      shadeIntensity: 0.3,
      opacityFrom: 0.4,
      opacityTo: 0,
      stops: [0, 90, 100]
    }
  },

  xaxis: {
    categories: ["Live","1h","2h","3h","4h","5h","6h","7h","8h","9h"]
  },

  tooltip: { shared: true, intersect: false },

  // 🔥 2 TRỤC Y — TEMP LEFT / HUMI RIGHT
  yaxis: [
  {
    seriesName: "Nhiệt độ",
    title: {
      text: "Nhiệt độ (°C)",
      style: {
        fontSize: "12px",
      },
    },
    min: 0,
    max: 50,
  },
  {
    seriesName: "Độ ẩm",
    opposite: true,
    title: {
      text: "Độ ẩm (%)",
      style: {
        fontSize: "12px",
      },
    },
    min: 0,
    max: 100,
      decimalsInFloat: 0  // humi chỉ số nguyên
  }
]

});
mainChart.render();

// ============== CẬP NHẬT SENSOR ==================
database.ref(SENSOR).on("value", snap => {
  const data = snap.val() || {};

  // LIVE hiển thị
document.getElementById("tempt").innerText = fmtTemp(data.temp?.live) + "°C";
document.getElementById("humi").innerText  = fmtInt(data.humi?.live) + "%";
document.getElementById("soil").innerText  = fmtInt(data.soil?.live) + "%";
document.getElementById("light").innerText = fmtInt(data.light?.live) + "Lux";


  // RAW data cho chart
  temptChart     = mapChart(data.temp);
  humidChart     = mapChart(data.humi);
  lightChartData = mapChart(data.light);
  soilChartData  = mapChart(data.soil);

  if (window.currentChart === "temp") {
    mainChart.updateSeries([
      { name: "Nhiệt độ", data: temptChart },
      { name: "Độ ẩm", data: humidChart },
    ]);
  }

  if (window.currentChart === "light") {
    mainChart.updateSeries([{ name: "Ánh sáng", data: lightChartData }]);
  }

  if (window.currentChart === "soil") {
    mainChart.updateSeries([{ name: "Độ ẩm đất", data: soilChartData }]);
  }
});

// =============== TAB SWITCH ====================
function showChart(evt, name) {
  window.currentChart = name;

  document.querySelectorAll(".tab-btn").forEach(x => x.classList.remove("active"));
  evt.currentTarget.classList.add("active");

  if (name === "temp") {
    mainChart.updateSeries([
      { name: "Nhiệt độ", data: temptChart },
      { name: "Độ ẩm", data: humidChart },
    ]);

    // quay lại chế độ 2 trục
    mainChart.updateOptions({
        colors: ["#e76f51", "#2a65ff"],   // ✅ TRẢ LẠI ĐÚNG 2 MÀU
      yaxis: [
        {
          seriesName: "Nhiệt độ",
          title: { text: "Nhiệt độ (°C)" },
          min: 0,
          max: 50,
        },
        {
          seriesName: "Độ ẩm",
          opposite: true,
          title: { text: "Độ ẩm (%)" },
          min: 0,
          max: 100,
            decimalsInFloat: 0  // humi chỉ số nguyên
        }
      ]
    });
  }

if (name === "light") {
  mainChart.updateSeries([{ name: "Ánh sáng", data: lightChartData }]);

  mainChart.updateOptions({
    colors: ["#f4c430"],   // ✅ VÀNG CHO ÁNH SÁNG
    yaxis: [{
      title: { text: "Ánh sáng (Lux)" },
      min: 0,
      max: 1000,
      decimalsInFloat: 0
    }]
  });
}


if (name === "soil") {
  mainChart.updateSeries([{ name: "Độ ẩm đất", data: soilChartData }]);

  mainChart.updateOptions({
    colors: ["#8B5E3C"],   // ✅ XANH LÁ CHO ĐỘ ẨM ĐẤT
    yaxis: [{
      title: { text: "Độ ẩm đất (%)" },
      min: 0,
      max: 100,
      decimalsInFloat: 0
    }]
  });
}


  setTimeout(() => mainChart.resize(), 100);
}
// ========== ẢNH GỐC → CROP → HIỂN THỊ → LƯU FIREBASE → UPLOAD CLOUD ==========
database.ref(RAW).on("value", async (snap) => {
  const rawUrl = snap.val();
  if (!rawUrl) return;

  // ✅ Tạo URL crop từ ảnh gốc
  const cropUrl = rawUrl.replace(
    "/upload/",
    "/upload/c_crop,fl_relative,w_0.4,h_1,x_0.32,y_0.6/"
  );

  // ✅ Hiển thị ảnh crop trên web
  document.getElementById("gardenImage").src =
    cropUrl + "?t=" + Date.now();

  // ✅ Lưu URL crop về Firebase
  database.ref(CROP).set(cropUrl);

  // ✅ Upload ảnh crop lên Cloudinary folder garden1_crop
  const formData = new FormData();
  formData.append("file", cropUrl);
  formData.append("upload_preset", UPLOAD_PRESET);
  formData.append("folder", CLOUD_FOLDER);

  const res = await fetch(
    `https://api.cloudinary.com/v1_1/${CLOUD_NAME}/image/upload`,
    { method: "POST", body: formData }
  );

  const data = await res.json();
  console.log("✅ Đã lưu ảnh crop về Cloudinary:", data.secure_url);
});







// Click yêu cầu ESP chụp ảnh
document.getElementById("updateImage").addEventListener("click", ()=>{
  database.ref(CAPTURE_REQ).set(Date.now());
  alert("📸 Yêu cầu chụp ảnh đã gửi — chờ vài giây…");
});
function updateLockGarden1() {
  const anyAuto = (mode1 === 1 || mode2 === 1 || mode3 === 1);

  // 🔒 ĐÈN + PHUN (dùng chung → khóa khi CÓ AUTO)
  [btnBulb, btnSpray].forEach(btn => {
    btn.style.pointerEvents = anyAuto ? "none" : "auto";
    btn.style.opacity       = anyAuto ? "0.6"  : "1";
  });

  // 🔒 QUẠT + BƠM VƯỜN 1 (riêng vườn 1)
  const lockGarden1 = (mode1 === 1);
  [btnFan, btnPump].forEach(btn => {
    btn.style.pointerEvents = lockGarden1 ? "none" : "auto";
    btn.style.opacity       = lockGarden1 ? "0.6"  : "1";
  });
}
updateLockGarden1();

