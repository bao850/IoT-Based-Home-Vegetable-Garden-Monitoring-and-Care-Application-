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

// ================= MAP INPUT =================
// Thứ tự input trong .threshold-panel (16 input)
const inputs = document.querySelectorAll(".threshold-panel input");

// ================= LOAD NGƯỠNG =================
function loadThreshold() {
  database.ref("smart_garden/setting").once("value").then(snap => {
    if (!snap.exists()) return;
    const d = snap.val();

    // 🌍 COMMON
    fill(0, d.common?.light);
    fill(2, d.common?.humi);

    // 🌱 VƯỜN 1
    fill(4, d.vuon1?.temp);
    fill(6, d.vuon1?.soil);

    // 🌿 VƯỜN 2
    fill(8, d.vuon2?.temp);
    fill(10, d.vuon2?.soil);

    // 🌾 VƯỜN 3
    fill(12, d.vuon3?.temp);
    fill(14, d.vuon3?.soil);
  });
}

function fill(startIndex, obj) {
  if (!obj) return;
  inputs[startIndex].value     = obj.min ?? "";
  inputs[startIndex + 1].value = obj.max ?? "";
}

// ================= SAVE NGƯỠNG =================
document.querySelector(".threshold-save button").addEventListener("click", () => {

  const data = {
    common: {
      light: read(0),
      humi:  read(2)
    },
    vuon1: {
      temp: read(4),
      soil: read(6)
    },
    vuon2: {
      temp: read(8),
      soil: read(10)
    },
    vuon3: {
      temp: read(12),
      soil: read(14)
    }
  };

database.ref("smart_garden/setting").set(data)
  .then(() => {
    // 🔥 báo cho ESP biết có ngưỡng mới
    database.ref("smart_garden/setting/update").set(1);
    alert("✅ Đã lưu ngưỡng thành công!");
  })
  .catch(() => alert("❌ Lỗi lưu ngưỡng"));

});

function read(startIndex) {
  return {
    min: Number(inputs[startIndex].value),
    max: Number(inputs[startIndex + 1].value)
  };
}

// ================= START =================
loadThreshold();