import cv2
import numpy as np
import requests
import firebase_admin
from firebase_admin import credentials, db
import time

# ================== FIREBASE CONFIG ==================
cred = credentials.Certificate("serviceAccountKey.json")  # file json firebase
firebase_admin.initialize_app(cred, {
    "databaseURL": "https://smartgarden-85ec4-default-rtdb.asia-southeast1.firebasedatabase.app"
})

# ✅ Đọc URL ĐÃ CẮT (web ghi vào /smart_garden/image_crop/vuon1)
IMAGE_PATH = "/smart_garden/image_crop/vuon1"
AI_PATH    = "/smart_garden/garden_health/vuon1"

# ================== HÀM XỬ LÝ ẢNH ==================

def analyze_image(url):
    print("⬇ Đang tải ảnh...")

    try:
        resp = requests.get(url + "?t=" + str(time.time()), timeout=10)
        img_array = np.frombuffer(resp.content, np.uint8)
        img = cv2.imdecode(img_array, cv2.IMREAD_COLOR)
    except Exception as e:
        print("❌ Lỗi tải ảnh:", e)
        return None, None

    if img is None:
        print("❌ Không đọc được ảnh")
        return None, None

    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    lower_green = np.array([35, 40, 40])
    upper_green = np.array([85, 255, 255])
    mask = cv2.inRange(hsv, lower_green, upper_green)

    green_result = cv2.bitwise_and(img, img, mask=mask)

    cv2.imshow("ANH GOC", img)
    cv2.imshow("ANH HSV", hsv)
    cv2.imshow("ANH MASK", mask)
    cv2.imshow("ANH XANH", green_result)

  

    key = cv2.waitKey(30) 

    total_pixels = mask.size
    green_pixels = cv2.countNonZero(mask)
    green_ratio = (green_pixels / total_pixels) * 100.0

    status = "TỐT" if green_ratio > 20 else "KHÔNG TỐT"
    print(f"🌿 Tỷ lệ xanh: {green_ratio:.2f}% | 🤖 {status}")

    return round(green_ratio, 2), status



# ================== VÒNG LẶP TỰ ĐỘNG ==================
print("✅ AI Garden đang chạy... (đọc URL đã cắt)")
last_url = ""

while True:
    try:
        url = db.reference(IMAGE_PATH).get()
     


        # Chỉ xử lý khi có URL mới
        if url and url != last_url:
            print("📸 Ảnh mới phát hiện!")
            last_url = url   # ✅ BẮT BUỘC PHẢI CÓ
            

            green_ratio, status = analyze_image(url)

            if green_ratio is not None:
                db.reference(AI_PATH).set({
                    "green_ratio": green_ratio,
                    "status": status,
                    "time": int(time.time())
                })
                print("✅ Đã gửi kết quả về Firebase!")

        time.sleep(3)

    except KeyboardInterrupt:
        print("\n⏹ Dừng bởi người dùng (Ctrl+C).")
        cv2.destroyAllWindows()
        break
    except Exception as e:
        print("❌ Lỗi:", e)
        time.sleep(5)
