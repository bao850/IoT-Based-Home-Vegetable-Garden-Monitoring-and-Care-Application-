# IoT-Based-Home-Vegetable-Garden-Monitoring-and-Care-Application-
IoT-based smart garden application using STM32, ESP32-CAM, and sensors to monitor temperature, humidity, soil moisture, light, and water level. Automatically controls the fan, pump, LED lights, and mist sprayer, with a web interface, Firebase integration, and Python/OpenCV plant image analysis.
<p align="center">
  <img width="400"  alt="image" src="https://github.com/user-attachments/assets/35ee251c-626f-45d9-be75-a033c423dcc4" />
    <br>
  Image 1.1. Front view of the model.
</p>
The figure shows the front view of the home vegetable garden monitoring and control system model. The model consists of a main growing area divided into different sections to simulate separate vegetable-growing zones. A glass window is installed at the front, allowing users to observe the plants and equipment inside the model. The control box is located at the lower left and contains the control circuits, an LCD display, and control buttons for operating the system. On the right side, a water tank is provided to store water for the irrigation system. These components are arranged to simulate the actual operation of a smart home vegetable garden system.
<p align="center">
  <img width="400"  alt="image" src="https://github.com/user-attachments/assets/29698663-3436-4530-8a86-906d23bd4f6d" />
    <br>
  Image 1.2. View through the front glass window of the model.
</p>
The figure shows the inside view of the vegetable-growing area of the model. The growing area is divided into three separate zones, with each zone containing soil and young vegetable plants. Each zone is equipped with a drip irrigation pipe to provide water directly to the plants. Cooling fans are installed at the back of the model to improve air circulation and help regulate the temperature. The three zones are separated by partitions, allowing different types of vegetables to be grown and monitored independently.
<p align="center">
 <img width="400" alt="image" src="https://github.com/user-attachments/assets/671629be-f546-4620-abc4-280bcd6d639c" />
    <br>
  Image 1.3. The pump tubing system on the right side and the misting pump tubing at the back of the model.
</p>
The figure shows the pump and misting system installed at the back of the model. The water pump is located on the right side and is connected to several water tubes that supply water to the different growing zones. The misting system is installed above the pump and is connected to the tubing system to spray a fine mist inside the model. These components work together to provide water and maintain suitable humidity for the vegetables.
<p align="center">
 <img width="400" alt="image" src="https://github.com/user-attachments/assets/4afdfb71-2537-4450-b88f-66d6108091b3" />
    <br>
  Image 1.4. Arrangement of the sensor devices viewed through the front glass window.
</p>
The figure shows the arrangement of the sensor and control devices viewed through the front glass window of the model. At the top, the misting nozzle is installed to spray fine water droplets into the growing area. A temperature and humidity sensor is placed below the misting nozzle to monitor the environmental conditions inside the model. Several circulation fans are installed at the back to improve air circulation and regulate the temperature. In each growing zone, irrigation pipes are positioned near the soil to supply water directly to the plants. These devices work together to monitor and maintain suitable environmental conditions for the vegetables.
<p align="center">
 <img width="400"  alt="image" src="https://github.com/user-attachments/assets/f7995772-3e67-43f0-8ca3-b1a164bd0542" />
    <br>
  Image 1.5. Arrangement of the sensor devices viewed from the right side.
</p>
The figure shows the arrangement of the sensor and electronic devices viewed from the right side of the model. The light sensor is installed at the top to measure the light intensity inside the growing area. The LED lighting system is mounted along the upper part of the model to provide additional light for the plants. The ESP32-CAM module is positioned on the left side and is used to capture images of the growing area for monitoring purposes. These devices support the monitoring and lighting functions of the smart vegetable garden system.
<p align="center">
 <img width="400" alt="image" src="https://github.com/user-attachments/assets/74d99e7f-734b-4c25-b503-e3120f9f3517" />
    <br>
  Image 1.6. The 20×4 LCD displays environmental parameters and system status.
</p>
<p align="center">
 <img width="400"  alt="image" src="https://github.com/user-attachments/assets/64b4f6b8-ac6a-458d-a0f0-ee923733d3aa" />
    <br>
  Image 1.7. The devices are turned on.
</p>
<p align="center">
<img width="400" alt="image" src="https://github.com/user-attachments/assets/d2968aa5-e377-49c8-a464-f364a54b3ef8" />
    <br>
  Image 1.8. Turn devices on/off via the web interface.
</p>
Figure 1.8 illustrates the system’s device control interface on the web platform. Through this interface, users can turn the garden devices on or off, including lighting, ventilation fans, the water pump, and the misting system. The status of each device is displayed visually on the interface, allowing users to easily monitor and control the system’s operation. In addition to manual control, the interface also supports automatic operation, allowing the devices to operate based on predefined environmental conditions, thereby improving the flexibility and efficiency of the vegetable garden management process.
<p align="center">
<img width="400"  alt="image" src="https://github.com/user-attachments/assets/8a0fdca8-70ec-479c-9b99-757c55e882d6" />
    <br>
  Image 1.9. Image library for storing vegetable garden images on Cloudinary.
</p>
Figure 1.9 illustrates the image library management interface of the system on the Cloudinary platform. Here, images captured by the camera are stored and categorized according to each vegetable growing zone. After being cropped appropriately for each zone, the images are synchronized with the Firebase database and displayed on the web interface, allowing users to monitor and track the condition of the vegetable garden over time.
<p align="center">
<img width="400"  alt="image" src="https://github.com/user-attachments/assets/7ef68cc7-7fbc-45b5-b695-0c3e469fb0f4" />
    <br>
  Image 1.10. Vegetable garden images after being cropped and synchronized with the web interface.
</p>
Figure 1.10 illustrates the web interface displaying images of the vegetable garden after being cropped by the system according to each growing zone. The images captured by the camera are initially separated into appropriate sections for each garden zone and then synchronized with the web interface, allowing users to clearly and visually monitor the condition of each vegetable growing zone.

