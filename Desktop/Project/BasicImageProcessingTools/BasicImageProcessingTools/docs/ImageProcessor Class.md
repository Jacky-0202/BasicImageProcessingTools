# 影像處理類別-ImageProcessor

`HiPointImageProcessor` 類別提供一組影像處理功能，包括灰階轉換、二值化、HSV 色彩空間轉換及使用 Canny 演算法進行邊緣偵測。此外，還包含一個將 OpenCV 圖像（`cv::Mat`）轉換為 Qt 的 `QPixmap` 的工具方法。

## 完整程式碼

```cpp
#include "HiPointImageProcessor.h"

cv::Mat HiPointImageProcessor::applyGrayscale(const cv::Mat& image) {
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    return grayImage;
}

cv::Mat HiPointImageProcessor::applyBinarization(const cv::Mat& image, int threshold) {
    cv::Mat grayImage;
    if (image.channels() == 3) {
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    }
    else {
        grayImage = image;
    }

    cv::Mat binaryImage;
    cv::threshold(grayImage, binaryImage, threshold, 255, cv::THRESH_BINARY);
    return binaryImage;
}

cv::Mat HiPointImageProcessor::applyHSV(const cv::Mat& image) {
    cv::Mat colorImage;
    if (image.channels() == 1) {
        cv::cvtColor(image, colorImage, cv::COLOR_GRAY2BGR);
    }
    else {
        colorImage = image;
    }

    cv::Mat hsvImage;
    cv::cvtColor(colorImage, hsvImage, cv::COLOR_BGR2HSV);
    return hsvImage;
}


cv::Mat HiPointImageProcessor::applyCanny(const cv::Mat& image, int lowThreshold, int highThreshold) {
    cv::Mat grayImage;
    if (image.channels() == 3) {
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    }
    else {
        grayImage = image;
    }

    cv::Mat cannyImage;
    cv::Canny(grayImage, cannyImage, lowThreshold, highThreshold);
    return cannyImage;
}


QPixmap HiPointImageProcessor::matToQPixmap(const cv::Mat& mat) {
    QImage::Format format;

    if (mat.type() == CV_8UC1) {
        format = QImage::Format_Grayscale8;
    }
    else if (mat.type() == CV_8UC3) {
        format = QImage::Format_RGB888;
    }
    else {
        throw std::invalid_argument("Unsupported cv::Mat format");
    }

    QImage image(mat.data, mat.cols, mat.rows, mat.step, format);

    if (mat.type() == CV_8UC3) {
        image = image.rgbSwapped();
    }

    return QPixmap::fromImage(image);
}
```



## 方法說明

`applyGrayscale`

**說明**
將輸入的彩色圖像（`cv::Mat`）轉換為灰階圖像。

**函式原型**

```cpp
cv::Mat applyGrayscale(const cv::Mat& image);
```

**參數**

- `image`: 輸入的 BGR 彩色圖像。

**回傳值**
回傳單通道的灰階圖像（`cv::Mat`）。

**程式碼片段**

```cpp
cv::Mat ImageProcessor::applyGrayscale(const cv::Mat& image) {
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    return grayImage;
}
```

------

`applyBinarization`

**說明**
對輸入圖像進行二值化處理。如果圖像為彩色，會先將其轉換為灰階。

**函式原型**

```cpp
cv::Mat applyBinarization(const cv::Mat& image, int threshold);
```

**參數**

- `image`: 輸入的圖像，可以是灰階或 BGR 格式。
- `threshold`: 用於二值化的閾值。

**回傳值**
回傳二值化的圖像（`cv::Mat`），像素值為 0 或 255。

**程式碼片段**

```cpp
cv::Mat ImageProcessor::applyBinarization(const cv::Mat& image, int threshold) {
    cv::Mat grayImage;
    if (image.channels() == 3) {
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        grayImage = image;
    }

    cv::Mat binaryImage;
    cv::threshold(grayImage, binaryImage, threshold, 255, cv::THRESH_BINARY);
    return binaryImage;
}
```

------

`applyHSV`

**說明**
將輸入圖像轉換為 HSV 色彩空間。如果輸入圖像為灰階，則會先轉換為 BGR 彩色格式。

**函式原型**

```
cv::Mat applyHSV(const cv::Mat& image);
```

**參數**

- `image`: 輸入的圖像，可以是灰階或 BGR 格式。

**回傳值**
回傳 HSV 色彩空間的圖像（`cv::Mat`）。

**程式碼片段**

```cpp
cv::Mat ImageProcessor::applyHSV(const cv::Mat& image) {
    cv::Mat colorImage;
    if (image.channels() == 1) {
        cv::cvtColor(image, colorImage, cv::COLOR_GRAY2BGR);
    } else {
        colorImage = image;
    }

    cv::Mat hsvImage;
    cv::cvtColor(colorImage, hsvImage, cv::COLOR_BGR2HSV);
    return hsvImage;
}
```

------

`applyCanny`

**說明**
對輸入圖像進行 Canny 邊緣偵測。如果輸入圖像為彩色，會先將其轉換為灰階。

**函式原型**

```cpp
cv::Mat applyCanny(const cv::Mat& image, int lowThreshold, int highThreshold);
```

**參數**

- `image`: 輸入的圖像，可以是灰階或 BGR 格式。
- `lowThreshold`: 低閾值，用於雙閾值過濾。
- `highThreshold`: 高閾值，用於雙閾值過濾。

**回傳值**
回傳經過邊緣偵測的二值化圖像（`cv::Mat`）。

**程式碼片段**

```cpp
cv::Mat ImageProcessor::applyCanny(const cv::Mat& image, int lowThreshold, int highThreshold) {
    cv::Mat grayImage;
    if (image.channels() == 3) {
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        grayImage = image;
    }

    cv::Mat cannyImage;
    cv::Canny(grayImage, cannyImage, lowThreshold, highThreshold);
    return cannyImage;
}
```

------

### `matToQPixmap`

**說明**
將 OpenCV 圖像（`cv::Mat`）轉換為 Qt 的 `QPixmap` 格式，以便在 Qt 應用程式中顯示。支援單通道灰階圖像和三通道彩色圖像。

**函式原型**

```
QPixmap matToQPixmap(const cv::Mat& mat);
```

**參數**

- `mat`: 輸入的圖像，可以是灰階或彩色（BGR）。

**回傳值**
回傳 `QPixmap` 格式的圖像。

**程式碼片段**

```cpp
QPixmap ImageProcessor::matToQPixmap(const cv::Mat& mat) {
    QImage::Format format;

    if (mat.type() == CV_8UC1) {  // 單通道灰階
        format = QImage::Format_Grayscale8;
    } else if (mat.type() == CV_8UC3) {  // 三通道 BGR
        format = QImage::Format_RGB888;
    } else {
        throw std::invalid_argument("Unsupported cv::Mat format");
    }

    QImage image(mat.data, mat.cols, mat.rows, mat.step, format);

    if (mat.type() == CV_8UC3) {
        image = image.rgbSwapped();  // 將 BGR 轉換為 RGB
    }

    return QPixmap::fromImage(image);
}
```

------

#### 程式碼說明

- 在影像處理中，`RGB888` 是一種圖像格式，它表示一張圖像的每個像素使用 **24位元 (bits)** 來儲存 RGB 顏色資訊，每個顏色通道 (R、G、B) 各佔 **8位元 (bits)**。

  - **8 bits for Red (R)**
  - **8 bits for Green (G)**
  - **8 bits for Blue (B)**

  這意味著每個顏色通道的取值範圍是 0 到 255（2⁸ = 256），因此這個格式總共可以表示 16,777,216 種顏色（256 * 256 * 256）。RGB888 格式經常在顏色圖像中使用，因為它能夠表現出豐富的色彩細節。