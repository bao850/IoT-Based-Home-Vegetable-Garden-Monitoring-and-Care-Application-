// ==================== Firebase Config ===================
const firebaseConfig = {
  apiKey: "AIzaSyBGm7yyK1MKP2F17ZBszG68D5ppJTvfCnU",
  authDomain: "smartgarden-85ec4.firebaseapp.com",
  databaseURL: "https://smartgarden-85ec4-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "smartgarden-85ec4",
  storageBucket: "smartgarden-85ec4.appspot.com",
  appId: "1:875575541081:web:04a89cced5140c4a5ba048"
};

firebase.initializeApp(firebaseConfig);

// ===================== DOM ======================
const emailInput = document.getElementById("email");
const passInput = document.getElementById("password");
const loginBtn  = document.getElementById("login-btn");
const errorBox  = document.getElementById("error-box");

// ===================== UI ERROR ======================
function showError(msg) {
  errorBox.innerText = msg;
  errorBox.style.display = "block";
}
function hideError() {
  errorBox.style.display = "none";
}

// ===================== LOGIN ======================
loginBtn.addEventListener("click", () => {

  hideError();

  const user = emailInput.value.trim();
  const pass = passInput.value.trim();

  if (!user || !pass) {
    showError("⚠️ Vui lòng nhập email và mật khẩu");
    return;
  }

  loginBtn.disabled = true;
  loginBtn.innerText = "Đang đăng nhập...";

  firebase.auth().signInWithEmailAndPassword(user, pass)
    .then(() => {
      sessionStorage.setItem("smartgarden_login", "ok");
      window.location.href = "datn.html";
    })
    .catch(err => {
      showError("🚫 Sai email hoặc mật khẩu");
      loginBtn.disabled = false;
      loginBtn.innerText = "Đăng nhập";
    });
});
