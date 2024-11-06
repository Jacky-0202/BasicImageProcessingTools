# Basic image processing tools 開發紀錄文件

## 概述

這是我自己的QT專案練習，設計一個影像處理工具提供基礎的影像閱覽及處理平台，支持灰階轉換、二值化、HSV 轉換、Canny 邊緣檢測等基本功能，並允許使用者將處理後的影像保存。該工具基於 C++ 開發，使用 Qt 進行 UI 架構，並利用 OpenCV 實現影像處理功能。

也有紀載一些我學習的心得...參考 `Self-study.md`



## 版本信息

- **開發環境**：Visual Studio 2022
- **程式語言** : C++
- **框架**：Qt 6.8.0
- **影像處理庫**：OpenCV 4.6.0
- **開發日期**：2024/10



## 設計需求

1. **影像載入** : 通過 UI 加載影像，並顯示在瀏覽畫面區域中。
2. **影像處理** : (只能)選擇一個處理功能（如灰階或 Canny），UI 呼叫對應的 `ImageProcessor` 方法進行處理。
3. **影像儲存** : 選擇保存功能後，程式會將處理後的影像儲存到指定路徑。



## 檔案結構

```
BasicImageProcessingTools/
├── main.cpp
├── MainWindow.h
├── MainWindow.cpp
├── MainWindow.ui
├── HiPointFlowControl.h
├── HiPointFlowControl.cpp
├── HiPointImageProcessor.h
├── HiPointImageProcessor.cpp
├── HiPointGraphicsScene.h
├── HiPointGraphicsScene.cpp
├── docs/
│   ├── Readme.md
│   ├── Self-study.md
│   ├── Custom QGraphicsScene.md
│   ├── ImageProcessor Class.md
├── resources/
│   └──   
├── samples/
│   └── images/
│   	└── lena.jpg
└── .gitignore
```



## UI-設計

| 元件                 | 識別符號                    |
| -------------------- | --------------------------- |
| **主畫面**           | ImgProcessingUI             |
| **載入圖片按鈕**     | loadImageButton             |
| **儲存圖片按鈕**     | saveImageButton             |
| **灰階轉換按鈕**     | grayscaleButton             |
| **HSV 按鈕**         | hsvButton                   |
| **二值化閾值滑桿**   | binarizationThresholdSlider |
| **二值化閾值標籤**   | binarizationThresholdLabel  |
| **邊緣檢測上限滑桿** | cannyUpperThresholdSlider   |
| **邊緣檢測下限滑桿** | cannyLowerThresholdSlider   |
| **邊緣檢測上限標籤** | cannyUpperThresholdLabel    |
| **邊緣檢測下限標籤** | cannyLowerThresholdLabel    |

