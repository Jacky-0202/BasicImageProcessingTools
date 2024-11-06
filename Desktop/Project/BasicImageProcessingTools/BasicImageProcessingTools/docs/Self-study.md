# 自我學習與心得-紀錄

## 影像處理類別 - HiPointImageProcessor

`HiPointImageProcessor` 類別的每個方法將返回 OpenCV 的 `cv::Mat` 格式影像。

#### 類別結構

```cpp
#pragma once

#include <opencv2/opencv.hpp>
#include <QGraphicsPixmapItem>

class ImageProcessor
{
public:
    cv::Mat applyGrayscale(const cv::Mat& image);
    cv::Mat applyBinarization(const cv::Mat& image, int threshold);
    cv::Mat applyHSV(const cv::Mat& image);
    cv::Mat applyCanny(const cv::Mat& image, int lowThreshold, int highThreshold);

    QPixmap matToQPixmap(const cv::Mat& mat);
};
```

#### 成員函數概述

以下是這些影像處理方法的表格整理：

| 方法名稱            | 功能描述                  |
| ------------------- | ------------------------- |
| `applyGrayscale`    | 將影像轉換為灰階          |
| `applyBinarization` | 將影像進行二值化處理      |
| `applyHSV`          | 將影像轉換為 HSV 色彩空間 |
| `applyCanny`        | 進行 Canny 邊緣檢測       |
| `matToQPixmap`      | 將 Mat 轉換成 QPixmap     |

詳情請看 `ImageProcessor Class.md`



## 自定義的QGraphicsScene

透過修改 `QGraphicsScene` 類這個元件，不僅僅是閱覽影像而已，還可以加入zoom in、zoom out，透過滑鼠按住平移影像等等的功能，詳情請看 `Custom QGraphicsScene.md` 這份文件。



## 心得與整理

### 1. 如何使用影像處理函數

- **`.h` 、 `.cpp`**

  QT很特別函數的定義會寫在標頭檔.h，函數的實現會寫在.cpp，我覺得結構性很好，很容易就能找到類別以及函數的主要架構，很快的就能知道如何使用。

- **`HiPointImageProcessor`**

  這是一個很單純的類別

  ```cpp
  class HiPointImageProcessor
  ```

  畢竟只有套用Opencv庫的函數，所以並沒有繼承其他QT原生的類別。

- **如何使用**

  ```cpp
  public:
      cv::Mat applyGrayscale(const cv::Mat& image);
  	...
  };
  ```

  我使用public就是希望其他的類別可以訪問此方法

  ```cpp
  #include "HiPointImageProcessor.h"
  
  ...
      
  private:
      HiPointImageProcessor m_imageProcessor;
  ```

  在其他類別使用的時候，要先將整個類別物件化。
  
  ```cpp
  m_processedImage = m_imageProcessor.applyGrayscale(m_originalImage);
  ```
  
  使用的時候用 `.` 的方式呼叫

---

###  繼承 QObject

繼承自 `QObject` 的類別可以使用 Qt 的信號與槽機制，這是 Qt 中非常強大的功能，用於在物件之間進行異步通信。當一個事件發生時，可以發出信號，而其他物件可以通過槽來接收和處理這些信號。

---

###  信號與槽機制（signals and slots）

- **什麼是信號 (Signal)**
  訊號是一種特殊的成員函數，可以理解為「事件的通知」。當一個物件的某種事件（例如按鈕被按下、數值改變等）發生時，這個物件會「發射」（emit）一個訊號，通知其他物件這個事件已經發生。訊號本身不會包含任何行為，只是告知某事件已發生(包含參數)。

  ```cpp
  class HiPointFlowControl : public QObject
  {
      public:
      ...
      signals:
      	void imageLoaded(const QPixmap& pixmap);
      	...
      private:
      ...
  }
  ```

  先定義照片載入這個訊號

  ```cpp
  emit imageLoaded(pixmap);
  ```

  傳出載入照片這個訊號

  ```cpp
  connect(m_flowControl, &HiPointFlowControl::imageLoaded,
          this, &MainWindow::onImageLoaded);
  ```

  跟主程式中 `.cpp` 的槽函數做連結，`HiPointFlowControl` 這個類的 `imageLoaded` 信號會觸發 `MainWindow` 的 `onImageLoaded` 函數

- **什麼是槽函數**

  槽是一個可以接收訊號的常規函數，當接收到訊號時會執行相應的代碼。通過槽函數，我們可以定義當訊號發出時應該做什麼事情。槽函數可以是任意的函數，也可以是類中的成員函數。

  範例:

  ```cpp
  connect(senderObject, &SenderClass::signalName,
          receiverObject, &ReceiverClass::slotName);
  ```

  這段程式碼表示當 `senderObject` 發射 `signalName` 訊號時，`receiverObject` 會接收並執行 `slotName` 函數。

- **Lambda 表達式**

  ```cpp
  connect(ui.loadImageButton, &QPushButton::clicked, this, [this]() {
      if (m_flowControl->loadImage()) {
          setProcessingButtonsEnabled(true);
          }
      });
  ```

  Lambda 表達式的語法如下：

  ```cpp
  [捕獲列表](參數列表) { 函數體 }
  ```

  - `[this]` 表示捕獲當前物件指針，因此可以在 Lambda 表達式內部使用 `this` 指針訪問類的成員變數或方法。
  - `()` 表示該 Lambda 表達式不接受任何參數。
  - `{ ... }` 表示 Lambda 的函數體，即執行的具體代碼。

  完整的操作流程 : 

  - 當使用者點擊 `loadImageButton` 時，會發出 `clicked` 訊號。
  - `connect` 函數將這個訊號連接到一個 Lambda 表達式。
  - Lambda 表達式內部執行 `m_flowControl->loadImage()`。
  - 如果 `loadImage()` 成功加載圖像，則執行 `setProcessingButtonsEnabled(true)` 來啟用相關的按鈕。

---

###  指標(*)與參考(&)

以下是 `*` 和 `&` 在 C++ 中的彙整表格，包括範例、用法和簡單解釋：

| 符號 | 用法                  | 範例             | 簡單解釋                                |
| ---- | --------------------- | ---------------- | --------------------------------------- |
| `*`  | 定義指標              | `int* ptr = &a;` | 定義一個指標 `ptr`，存儲變數 `a` 的地址 |
| `*`  | 解引用（dereference） | `*ptr = 20;`     | 修改` *ptr`，相當於修改 `a`             |
| `&`  | 取地址                | `int* ptr = &a;` | 取得變數 `a` 的地址並將其存入指標 `ptr` |
| `&`  | 定義引用              | `int& ref = a;`  | 修改 `ref`，相當於修改 `a`              |

- **在類別上使用指標**

  通常在 Qt 或其他 C++ 應用中，物件指標的使用可以有效管理記憶體，特別是在需要動態創建、釋放或在不同類之間共享物件時。使用指標使得代碼更靈活，允許在不同的物件之間傳遞該指標，並讓程式更高效地管理物件的生命週期。

  ```cpp
  HiPointImageProcessor m_imageProcessor;
  HiPointFlowControl* m_flowControl;
  ```

  所以 `m_flowControlPointer`不能像 `imageProcessor` 一樣用使用物件 `.` 而是要用指標 `->` 去呼叫 ，直接使用物件的方法可以參考 [如何使用影像處理函數](#1. 如何使用影像處理函數) 。

  ```cpp
  m_flowControlPointer->saveImage();
  ```
